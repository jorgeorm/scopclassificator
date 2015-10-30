#ifndef PREDICTIVEMODEL_H
#define PREDICTIVEMODEL_H

#include <QMap>
#include <QObject>

#include <models/utils/matrix.h>

class PredictiveModel
{
public:
    PredictiveModel();

    void setProfiles(const QMap<QString, int *> &profiles);
    QMap<QString, int *> getProfiles() const;


    Matrix<float *> getRepresentativeFeatures() const;
    void setRepresentativeFeatures(const Matrix<float *> &representativeFeatures);


protected:
    QMap<QString, int *> _profiles;
    Matrix<float *> _representativeFeatures;
};

#endif // PREDICTIVEMODEL_H
