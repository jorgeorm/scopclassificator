#include "featureservice.h"
#include "predictivemodelservice.h"
#include "float.h"

#include <QFile>
#include <QString>

PredictiveModelService::PredictiveModelService(){
    _entry = NULL;
    _model = NULL;
    _distance = COSINE;
    _classTag = "";
}

PredictiveModelService::~PredictiveModelService(){

    if(_entry != NULL) delete _entry;
}

PredictiveModel *PredictiveModelService::generateModel(QList<SCOPEntry *> entries,
                                                       Matrix<float> *repFeatures,
                                                       FeatureDefinition *featDef){
    PredictiveModel *model = new PredictiveModel();
    model->setRepresentativeFeatures(repFeatures);
    model->setFeatureDefinition(featDef);

    float *entryProfile = NULL;

    foreach(SCOPEntry *entry, entries){
        entryProfile = getProfile(repFeatures, entry, featDef);
        model->addProfile(entry->scss(), entryProfile);
    }
    //scaleProfiles(model);

    return model;
}

QString PredictiveModelService::classify(PredictiveModel *model, SCOPEntry *entry){
    QString classification = "";

    if (model != NULL && entry != NULL){
        FeatureService featGenerator;
        FeatureDefinition *featDef = model->featureDefinition();
        Matrix<float> *lclFeatures = featGenerator.localFeaturesMatrix(featDef,
                                                                        entry);
        if (model->representativeFeatures()->scaled()) {
            Matrix<float> *scaledLocalFeat = featGenerator.scaleMatrixByNumber(lclFeatures);
            delete lclFeatures;
            lclFeatures = scaledLocalFeat;
        }
        //qDebug << "Features Matrix: ";
        //qDebug() << *lclFeatures;

        classification = classify(model, lclFeatures);
        delete lclFeatures;
    }

    return classification;
}

QString PredictiveModelService::classify(PredictiveModel *model, Matrix<float> *localFeatures){
    QString classification = "";
    QString profile_str = "";

    if (model != NULL && localFeatures != NULL){
        float *profile = getProfile(model->representativeFeatures(), localFeatures);
        if(model->hasScaledProfile()){
            scaleProfile(profile, model->scaleValuesByFeat(), model->profileLength());
        }
        classification = classify(model, profile);
        delete profile;
    }
    return classification;
}

QString PredictiveModelService::classify(PredictiveModel *model, float *profile){
    QString assignedClassTag = "";
    float distance = 0;
    float smallerDistance = FLT_MAX;

    if (model != NULL && profile != NULL){

        QMap<QString, float *> profiles = model->profiles();

        foreach (QString classTag, profiles.keys()) {
            switch (_distance) {
            case EUCLIDEAN:
                distance = euclideanDistance(profiles.value(classTag), profile, model->profileLength());
                break;
            case COSINE:
                distance = 1.0 - cosineSimilarity(profiles.value(classTag), profile, model->profileLength());
                break;
            }

            if(distance < smallerDistance){
                smallerDistance = distance;
                assignedClassTag = classTag;
            }
        }
    }

    return assignedClassTag;
}

float *PredictiveModelService::getProfile(Matrix<float> *representativeFeatures,
                                        SCOPEntry *entry,
                                        FeatureDefinition *featDef){
    FeatureService featGenerator;
    Matrix<float> *localFeatures = featGenerator.localFeaturesMatrix(featDef,entry);
    if (representativeFeatures->scaled()) {
        Matrix<float> *scaledLocalFeat = featGenerator.scaleMatrixByNumber(localFeatures);
        delete localFeatures;
        localFeatures = scaledLocalFeat;
    }
    return getProfile(representativeFeatures, localFeatures);
}

float *PredictiveModelService::getProfile(Matrix<float> *representativeFeatures,
                                        Matrix<float> *localFeatures){
    if(representativeFeatures == NULL || localFeatures == NULL) return NULL;

    unsigned profileLength = representativeFeatures->rows();
    unsigned numLocalFeat = localFeatures->rows();
    float *profile = NULL;


    if (profileLength > 0 && numLocalFeat > 0){
        float distance = 0;
        float minDistance = FLT_MAX;
        unsigned mostAlkComnFeat = profileLength;
        float *commonFeature, *localFeature;
        FeatureService featHandler;
        profile = new float[profileLength];


        for(unsigned i = 0; i < profileLength; ++i) profile[i] = 0;

        for (unsigned iLclFeat = 0; iLclFeat < numLocalFeat; ++iLclFeat){
            localFeature = localFeatures->row(iLclFeat);

            for (unsigned jComnFeat = 0; jComnFeat < profileLength; ++jComnFeat){
                commonFeature = representativeFeatures->row(jComnFeat);
                distance = featHandler.euclideanDistance(localFeature, commonFeature, localFeatures->cols());
                if (distance < minDistance) {
                    minDistance = distance;
                    mostAlkComnFeat = jComnFeat;
                }
            }
            ++profile[mostAlkComnFeat];
            minDistance = FLT_MAX;
            mostAlkComnFeat = profileLength;
        }

    }
    return profile;
}

void PredictiveModelService::scaleProfiles(PredictiveModel *model, float *scaleValues){
    unsigned profileLength = model->representativeFeatures()->rows();
    unsigned numProfiles = model->profiles().size();

    if (model != NULL) {

        //If no scale is provided, RMS is calculated and used as scale value
        if (scaleValues == NULL){
            scaleValues = new float[profileLength];
            for (unsigned i = 0; i < profileLength; ++i) scaleValues[i] = 0;
            float *profile;

            //SUM OF SQUARES
            foreach(QString classTag, model->profiles().keys()){
                profile = model->profile(classTag);
                for (unsigned i = 0; i < profileLength; ++i) scaleValues[i] += pow((double)profile[i], 2.0);
            }

            //SUM OF SQUARES SQRT(SUMSQ/(N-1))
            for (unsigned i = 0; i < profileLength; ++i)
                scaleValues[i] = sqrt(scaleValues[i] / (float)(numProfiles - 1));
        }

        // Scales profiles in model
        float *profileTmp = NULL;
        foreach(QString classTag, model->profiles().keys()){
            profileTmp = model->profile(classTag);
            scaleProfile(profileTmp, scaleValues, profileLength);
        }
        model->setProfileScaleFactors(scaleValues);
    }

}

void PredictiveModelService::saveModel(PredictiveModel *model, QString file){
    if(model == NULL || file.isEmpty()) return;
    QFile outputFile(file);
    if(outputFile.open(QIODevice::WriteOnly)){
        QTextStream ostream(&outputFile);

        unsigned profileLength = model->representativeFeatures()->rows();
        float *profile;

        ostream << "==FeatureDefinition" << endl;
        ostream << "Feature Size: " << model->featureDefinition()->sqrtSize() << endl;
        ostream << "Feature Type: " << model->featureDefinition()->method() << endl;
        ostream << "==Representative Features" << endl;
        ostream << "Features: " << model->representativeFeatures()->rows() << endl;
        ostream << "Is Scaled?: " << model->representativeFeatures()->scaled() << endl;
        ostream << "Maximum Value: " << model->representativeFeatures()->maxVal() << endl;
        ostream << "Data Matrix: " << endl;
        Matrix<float> *repFeats = model->representativeFeatures();
        for (unsigned i = 0; i < repFeats->rows(); ++i){
            for(unsigned j = 0; j < repFeats->cols(); ++j){
                ostream << repFeats->operator ()(i,j);
                if(j < repFeats->cols() - 1) ostream << ", ";
            }
            if (i < repFeats->rows()) ostream << endl;
        }
        ostream << "==Profiles" << endl;
        ostream << "Is Scaled (RMS)?: " << model->hasScaledProfile() << endl;
        if (model->hasScaledProfile()){
            profile = model->scaleValuesByFeat();
            ostream << "Scale Values: ";
            for(unsigned i = 0; i < profileLength; ++i){
                ostream << profile[i];
                if (i < profileLength - 1) ostream << ",";
            }
            ostream << endl;
        }
        ostream << "Classtag, Profile (csv)" << endl;
        foreach (QString classTag, model->profiles().keys()){
            ostream << classTag;
            ostream << ", ";
            profile = model->profile(classTag);
            for(unsigned i = 0; i < profileLength; ++i){
                ostream << profile[i];
                if (i < (profileLength - 1)) ostream << ",";
            }
            ostream << endl;
        }

        outputFile.close();
    }
}

float PredictiveModelService::cosineSimilarity(float *profile1,
                                               float *profile2,
                                               unsigned profileSize) const{
    double dotProduct, magnitude1, magnitude2;
    if (profile1 != NULL && profile2 != NULL){
        dotProduct = 0;
        magnitude1 = 0;
        magnitude2 = 0;

        for(unsigned i = 0; i < profileSize; ++i){
            dotProduct += profile1[i] * profile2[i];
            magnitude1 += pow(profile1[i], 2.0);
            magnitude2 += pow(profile2[i], 2.0);
        }

        magnitude1 = sqrt(magnitude1);
        magnitude2 = sqrt(magnitude2);

        return (float) (dotProduct / (magnitude1 * magnitude2));
    }
    else return 0;
}

void PredictiveModelService::scaleProfile(float *profile, float *scaleValues, unsigned profileLength){
    for(unsigned i = 0; i < profileLength; ++i) {
        profile[i] = profile[i] / scaleValues[i];
    }
}

QString PredictiveModelService::classTag() const
{
    return _classTag;
}

PredictiveModel *PredictiveModelService::getModel() const
{
    return _model;
}

void PredictiveModelService::setModel(PredictiveModel *model)
{
    _model = model;
}

void PredictiveModelService::runClassification(){
    _classTag = classify(_model,_entry);
    emit entryClassified(_entry->sid());
}

float PredictiveModelService::euclideanDistance(float *profile1, float *profile2, unsigned profileSize) const{
    double squareDif = 0;

    for(unsigned i = 0; i < profileSize; ++i){
        squareDif += pow(profile1[i] - profile2[i], 2.0);
    }

    return sqrt(squareDif);
}

SCOPEntry *PredictiveModelService::getEntry() const
{
    return _entry;
}

void PredictiveModelService::setEntry(SCOPEntry *entry)
{
    _entry = entry;
}

PredictiveModelService::DISTANCETYPE PredictiveModelService::getDistance() const
{
    return _distance;
}

void PredictiveModelService::setDistance(const DISTANCETYPE &distance)
{
    _distance = distance;
}




