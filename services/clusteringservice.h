#ifndef CLUSTERINGSERVICE_H
#define CLUSTERINGSERVICE_H

#include <QObject>
#include <QString>

#include <models/featuredefinition.h>
#include <models/scopentry.h>

#include <models/utils/matrix.h>



class ClusteringService: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit ClusteringService(QObject *parent = 0);
    virtual ~ClusteringService();

    QString name() const;
    void setName(const QString &name);

    SCOPEntry *entry() const;
    void setEntry(SCOPEntry *entry);

    Matrix<float> *matrix() const;
    void setMatrix(Matrix<float> *matrix);

    QString preAlgorithm() const;
    void setPreAlgorithm(const QString &preAlgorithm);

    QString postAlgorithm() const;
    void setPostAlgorithm(const QString &postAlgorithm);

    QString algorithm() const;
    void setAlgorithm(const QString &algorithm);

    FeatureDefinition *featDef() const;
    void setFeatDef(FeatureDefinition *featDef);

    QString pathDataFile() const;
    void setPathDataFile(const QString &pathDataFile);

protected:
    /**
     * @brief parseRscript parses R script with the matrix and the algorithm provided
     * @return
     */
    QString parseRscript(QString dataPath = "");

    void resetDataFlags();

public slots:
    /**
     * @brief findCommonFeatures Starts clustering process configured
     */
    void findCommonFeatures();

signals:
    /**
     * @brief commonFeaturesFound
     * @param entryName
     */
    void commonFeaturesFound(QString entryName);
    /**
     * @brief nameChanged
     */
    void nameChanged();
    /**
     * @brief errorOnClustering
     * @param clusteringScript
     */
    void errorOnClustering(QString clusteringScript);

private:
    QString _name, _algorithm, _preAlgorithm, _postAlgorithm, _pathDataFile;
    SCOPEntry *_entry;
    FeatureDefinition *_featDef;
    Matrix<float> *_matrix;
    bool _useEntry, _useMatrix, _useFile;

};

#endif // CLUSTERINGSERVICE_H
