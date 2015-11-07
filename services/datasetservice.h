#ifndef DATASETSERVICE_H
#define DATASETSERVICE_H

#include "scopentryservice.h"

#include <QObject>

#include <models/dataset.h>

class DatasetService : public QObject
{
    Q_OBJECT

public:
    explicit DatasetService(QObject *parent = 0);
    /**
     * @brief proteins Gets the scop entries inside of the dataset
     * @param dataset
     * @return QList<SCOPEntry*>
     */
    QList<SCOPEntry*> proteins(Dataset const &dataset);

    /**
     * @brief proteinsMetadata Gets the scop entries loading only metadata
     * @param dataset
     * @return
     */
    QList<SCOPEntry *> proteinsMetadata(Dataset const &dataset);

    /**
     * @brief sampledProteins Gets the scop entries inside of the dataset's sample
     * @param dataset
     * @return QList<SCOPEntry*>
     */
    QList<SCOPEntry*> sampledProteins(Dataset const &dataset);

    /**
     * @brief sampledProteinsMetadata Gets the scop entries metadata inside of the dataset's sample
     * @param dataset
     * @return
     */
    QList<SCOPEntry *> sampledProteinsMetadata(Dataset const &dataset);
    
    /**
     * @brief loadAstralFileIntoDataset handles dataset seting up and file management
     * @param dataset
     * @param pathToId
     * @param pathToEntFolder
     */
    void loadAstralFileIntoDataset(Dataset *dataset,
                                   QString pathToId,
                                   QString pathToEntFolder);
    /**
     * @brief loadDirectoryIntoDataset handles dataset seting up with folderar of files
     * @param dataset
     * @param pathToEnts
     */
    void loadDirectoryIntoDataset(Dataset *dataset, QString pathToEnts);

    /**
     * @brief sampleDataset
     * @param dataset
     * @param sampleSize
     */
    void sampleDataset(Dataset *dataset,
                       int sampleSize);
    /**
     * @brief sampleByCriteria
     * @param dataset
     * @param sampleSize
     */
    void sampleByCriteria(Dataset *dataset, unsigned sampleSize);


    /**
     * @brief getSampleFromFileList
     * @param filesInDirectory
     * @param sampleSize
     * @return
     */
    QMap<QString, QString> getSampleFromFileList(QStringList filesInDirectory, int sampleSize);

    void setSizeFilter(unsigned minProteinSize, unsigned maxProteinSize);

    void setClassFilter(SCOPEntry::SCOPCLASSLEVEL classLevel);
    
    /**
     * @brief getParamSampleFromFileList
     * @param sampleSize
     * @param filesInDirectory
     * @return
     */
    QMap<QString, QString> getParamSampleFromFileList(unsigned sampleSize,
                                                      QStringList filesInDirectory);

    /**
     * @brief scanFolderForEntFiles
     * @param folderPath
     * @return
     */
    QStringList scanFolderForEntFiles(QString folderPath);

signals:

    void notifyTask(QString nameOfTask);
    void notifyProgress(int progress);

protected:
    /**
     * @brief filteredAstralFiles
     * @param pathToId
     * @param pathToEntFolder
     * @return
     */
    QStringList filteredAstralFiles(
            QString pathToId,
            QString pathToEntFolder);
    /**
     * @brief generatePathToSample
     * @return
     */
    QString generatePathToSample();
    /**
     * @brief copyFilesIntoPath
     * @param files
     * @param newPath
     * @return
     */
    bool copyFilesIntoPath(QStringList files, QString newPath);

    /**
     * @brief loadFilesInPathAsEntries
     * @param pathToFiles
     * @return
     */
    QList<SCOPEntry *> loadFilesInPathAsEntries(QString pathToFiles);


    QList<SCOPEntry *> loadFilesInpathAsEntriesMetadata(QString pathTofiles);

    void initializeSampleByLevel(SCOPEntryService &entLoader, const QStringList &filesInDirectory);

private:
    bool isClassSampleFullfilled(QString className);
    void generateProgressSignal(unsigned int dx, unsigned int total);
    unsigned _minProtSize, _maxProtSize;
    SCOPEntry::SCOPCLASSLEVEL _scopLevel;
    bool _filterBySize, _runSampleByScopLevel;
    QMap<QString, unsigned> _sampleByScopLvl;
    QMap<QString, unsigned> _totalByScopLvl;
};

#endif // DATASETSERVICE_H
