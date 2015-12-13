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
        if(profile == NULL) {
            qDebug() << "Empty profile!!!";
            return classification;
        }
        if(model->hasScaledProfile()){
            scaleProfile(profile, model->scaleValuesByFeat(), model->profileLength());
        }

        classification = classify(model, profile);
        delete profile;
    }
    return classification;
}

QString PredictiveModelService::classify(PredictiveModel *model, float *profile){
    QString assignedClassTag;
    float distance = 0;
    float smallerDistance = FLT_MAX;
    float *modelProfile;
    float *mostSimilarProfile = NULL;

    if (model != NULL && profile != NULL){

        QMap<QString, float *> profiles = model->profiles();

        foreach (QString classTag, profiles.keys()) {

            foreach (modelProfile, profiles.values(classTag)){
                switch (_distance) {
                case EUCLIDEAN:
                    distance = euclideanDistance(modelProfile, profile, model->profileLength());
                    break;
                case COSINE:
                    distance = 1.0 - cosineSimilarity(modelProfile, profile, model->profileLength());
                    break;
                }

                if(distance < smallerDistance){
                    smallerDistance = distance;
                    assignedClassTag = classTag;
                    mostSimilarProfile = modelProfile;
                }
            }
        }
//        qDebug() << "=============== Classification: ";
        qDebug() << _entry->sid() << "; " << _entry->scss() << "; " << assignedClassTag ;
//        qDebug() << "=============== Most similar profile: ";
//        QDebug dbg = qDebug();
//        for(unsigned i = 0; i < model->profileLength(); ++i){
//            dbg << mostSimilarProfile[i] << ";";
//        }

        profiles.clear();
    }

    return assignedClassTag;
}

float *PredictiveModelService::getProfile(Matrix<float> *representativeFeatures,
                                        SCOPEntry *entry,
                                        FeatureDefinition *featDef){
    FeatureService featGenerator;
    qDebug() << "Feature method: "<< featDef->method();
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

float *PredictiveModelService::getRMSScale(PredictiveModel *model)const{
    if (model == NULL) return NULL;

    unsigned numProfiles = model->profiles().size();
    unsigned profileLength = model->profileLength();
    float *scaleValues = new float[profileLength];
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

    return scaleValues;
}

void PredictiveModelService::scaleProfiles(PredictiveModel *model, float *scaleValues){
    unsigned profileLength = model->profileLength();

    if (model != NULL) {

        //If no scale is provided, RMS is calculated and used as scale value
        if (scaleValues == NULL){
            scaleValues = getRMSScale(model);
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
        ostream << "Maximum Value: " << model->featureDefinition()->treshold() << endl;
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
        QList<float *> profilesByClass;
        foreach (QString classTag, model->profiles().keys()){
            profilesByClass = model->profiles().values(classTag);

            foreach(profile, profilesByClass){
                ostream << classTag;
                ostream << ", ";

                for(unsigned i = 0; i < profileLength; ++i){
                    ostream << profile[i];
                    if (i < (profileLength - 1)) ostream << ",";
                }
                ostream << endl;
            }
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
            magnitude1 += profile1[i] * profile1[i];
            magnitude2 += profile2[i] * profile2[i];
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

QString PredictiveModelService::classTag() const{
    return _classTag;
}

PredictiveModel *PredictiveModelService::loadModel(QString pathModel){
    PredictiveModel *model = NULL;
    QFile modelFile(pathModel);
    QString line;
    QStringList fileSegments;
    fileSegments << "==FeatureDefinition";
    fileSegments << "==Representative Features";
    fileSegments << "==Profiles";
    int idSegment;

    if(modelFile.open(QIODevice::ReadOnly)){
        QTextStream istream(&modelFile);
        model = new PredictiveModel();

        while(! istream.atEnd()){
            line = istream.readLine();
            if(line.contains("==")) {
                idSegment = fileSegments.indexOf(line);

                switch(idSegment) {
                case 0:
                    loadFeatureDefinition(&istream, model);
                    break;
                case 1:
                    loadRepresentativeFeatures(&istream, model);
                    break;
                case 2:
                    loadProfiles(&istream, model);
                    break;
                }
            }
        }

        modelFile.close();
    }

    return model;
}

float *PredictiveModelService::getMagnitudeScale(PredictiveModel *model) const{
    if (model == NULL) return NULL;

    unsigned profileLength = model->profileLength();
    float *scaleValues = new float[profileLength];
    for (unsigned i = 0; i < profileLength; ++i) scaleValues[i] = 0;
    float *profile;

    //SUM OF SQUARES
    foreach(QString classTag, model->profiles().keys()){
        profile = model->profile(classTag);
        for (unsigned i = 0; i < profileLength; ++i) scaleValues[i] += pow((double)profile[i], 2.0);
    }

    //SUM OF SQUARES SQRT(SUMSQ/(N-1))
    for (unsigned i = 0; i < profileLength; ++i)
        scaleValues[i] = sqrt(scaleValues[i]);

    return scaleValues;
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

void PredictiveModelService::loadFeatureDefinition(QTextStream *istream, PredictiveModel *model){
    QString line;
    QStringList splitedLine;

    FeatureDefinition *fdef = NULL;

    line = istream->readLine();
    splitedLine = line.split(": ");

    fdef = new FeatureDefinition();
    QString size = splitedLine.at(1);
    fdef->setSqrtSize(size.toInt());

    line = istream->readLine();
    splitedLine = line.split(": ");
    QString value = splitedLine.at(1);
    FeatureDefinition::GeneratorMethod method = (FeatureDefinition::GeneratorMethod) value.toInt();
    fdef->setMethod(method);

    model->setFeatureDefinition(fdef);
}

void PredictiveModelService::loadRepresentativeFeatures(QTextStream *istream, PredictiveModel *model){
    QString lineFeatures = istream->readLine();
    unsigned numFeat = lineFeatures.split(": ").at(1).toUInt();
    QString lineScaled = istream->readLine();
    bool scaled = (bool) lineScaled.split(": ").at(1).toInt();
    QString linemaxVal = istream->readLine();
    FeatureDefinition * featDef = model->featureDefinition();
    featDef->setTreshold(linemaxVal.split(": ").at(1).toFloat());
    istream->readLine();

    unsigned featSize = model->featureDefinition()->calculatedSize();

    QString featLine;
    QStringList featValues;
    Matrix<float> *repFeat = new Matrix<float>(numFeat, featSize);
    for (unsigned i = 0; i < numFeat; ++i) {
        featLine = istream->readLine();
        featValues = featLine.split(",");
        for (unsigned j = 0; j < featSize; ++j) {
            repFeat->setValue(i, j, featValues.at(j).toFloat());
        }
    }

    repFeat->setScaled(scaled);
    model->setRepresentativeFeatures(repFeat);
}

void PredictiveModelService::loadProfiles(QTextStream *istream, PredictiveModel *model){
    QString rmsLine = istream->readLine();
    bool rmsScaled = (bool) rmsLine.split(": ").at(1).toInt();
    unsigned profileSize = model->representativeFeatures()->rows();

    if(rmsScaled){
        QString scaleValsLine = istream->readLine();
        QStringList scaleValues = scaleValsLine.split(": ").at(1).split(",");
        float *scaleVals = new float[profileSize];
        for(unsigned i = 0; i < profileSize; ++i){
            scaleVals[i] = scaleValues.at(i).toFloat();
        }
        model->setProfileScaleFactors(scaleVals);
    }

    QString profileLine;
    QStringList profile;
    float *profileTmp;
    istream->readLine(); //Read header profile
    while(! istream->atEnd()){
        profileLine = istream->readLine();
        profile = profileLine.split(",");
        profileTmp = new float[profileSize];
        for (unsigned i = 0; i < profileSize; ++i){
            profileTmp[i] = profile.at(i+1).toFloat();
        }
        model->addProfile(profile.at(0), profileTmp);
    }
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




