#include "datasetservice.h"
#include "loggerservice.h"
#include "scopentryservice.h"

#include <QDirIterator>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>

#include <lib/randomgenerator.h>

//PUBLIC METHODS
DatasetService::DatasetService(QObject *parent) : QObject(parent) {
    _runSampleByScopLevel = false;
    _filterBySize = false;
    _maxProtSize = false;
    _minProtSize = false;
    _scopLevel = SCOPEntry::FAMILY;
}

QList<SCOPEntry *> DatasetService::proteins(const Dataset &dataset) {
    QString pathFiles = dataset.pathProteins();

    return loadFilesInPathAsEntries(pathFiles);
}

QList<SCOPEntry *> DatasetService::proteinsMetadata(const Dataset &dataset){
    QString pathFiles = dataset.pathProteins();

    return loadFilesInpathAsEntriesMetadata(pathFiles);
}

QList<SCOPEntry *> DatasetService::loadFilesInPathAsEntries(QString pathToFiles){
    QStringList entFiles = scanFolderForEntFiles(pathToFiles);

    QList< SCOPEntry *> entObjects;
    SCOPEntryService loader;
    unsigned int fileCounter = 0;

    foreach (const QString &entPath, entFiles) {
        entObjects << loader.loadEntFile(entPath);
        generateProgressSignal(++fileCounter, entFiles.size());
    }

    return entObjects;
}

QList<SCOPEntry *> DatasetService::loadFilesInpathAsEntriesMetadata(QString pathTofiles){

    QStringList entFiles = scanFolderForEntFiles(pathTofiles);

    QList<SCOPEntry *> entObjects;
    SCOPEntryService loader;
    unsigned int fileCounter = 0;

    foreach (const QString &entPath, entFiles) {
        entObjects << loader.loadOnlyMetaData(entPath);
        generateProgressSignal(++fileCounter, entFiles.size());
    }

    return entObjects;

}

void DatasetService::generateProgressSignal(unsigned int dx, unsigned int total){
    float progressTmp = (float) dx / (float) total;
    progressTmp *= 100.0;

    emit notifyProgress((int) floor(progressTmp));
}

QList<SCOPEntry *> DatasetService::sampledProteins(const Dataset &dataset) {
    QString pathFiles = dataset.pathSampledProteins();

    return loadFilesInPathAsEntries(pathFiles);
}

QList<SCOPEntry *> DatasetService::sampledProteinsMetadata(const Dataset &dataset){
    QString pathFiles = dataset.pathSampledProteins();

    return loadFilesInpathAsEntriesMetadata(pathFiles);
}

void DatasetService::loadAstralFileIntoDataset(
        Dataset *dataset,
        QString pathToId,
        QString pathToEntFolder) {

    QStringList astralFiles = filteredAstralFiles(pathToId,
                                                  pathToEntFolder);
    ConfigurationService confService;
    QFileInfo astralFileInfo (pathToId);

    QString pathAstralDir = confService.loadConfig()->workdirPath();
    pathAstralDir += "/astral_subsets/";
    QString safeDateText = QDateTime::currentDateTime().toString().simplified();
    safeDateText.replace(" ", "_");
    safeDateText.replace(".", "");
    safeDateText.replace(":", "_");
    pathAstralDir += safeDateText;
    pathAstralDir += "_";
    pathAstralDir += astralFileInfo.completeBaseName();

    QDir filtedAstralFolder(pathAstralDir);

    if (! filtedAstralFolder.exists()){
        bool folderMade = filtedAstralFolder.mkpath(pathAstralDir);
        if (! folderMade) {
            LoggerService log;
            log.logError("No pude crear directorio para archivos astral.",
                         QStringList()<<pathAstralDir);
            return;
        }
    }

    emit notifyTask("Copying astral");
    this->copyFilesIntoPath(astralFiles, pathAstralDir);

    dataset->setPathProteins(pathAstralDir);
}

void DatasetService::loadDirectoryIntoDataset(
        Dataset *dataset,
        QString pathToEnts) {
    dataset->setPathProteins(pathToEnts);
}

QMap<QString, QString> DatasetService::getSampleFromFileList(QStringList filesInDirectory, int sampleSize)
{
    QMap<QString, QString> sampledFiles;

    RandomGenerator rand;
    do {
        int position = rand.randomIntBetween(0, filesInDirectory.size());
        QString pathSelectedFile = filesInDirectory.at(position);
        QFileInfo infoSelected(pathSelectedFile);

        if (sampledFiles.contains(infoSelected.completeBaseName())){
            continue;
        }

        sampledFiles.insert(infoSelected.completeBaseName(),
                            pathSelectedFile);
    } while (sampledFiles.values().size() < sampleSize);

    return sampledFiles;
}

void DatasetService::setSizeFilter(unsigned minProteinSize,
                                   unsigned maxProteinSize) {

    _minProtSize = minProteinSize;
    _maxProtSize = maxProteinSize;
    _filterBySize = true;
}

void DatasetService::setClassFilter(SCOPEntry::SCOPCLASSLEVEL classLevel) {
    _scopLevel = classLevel;
    _runSampleByScopLevel = true;
}

void DatasetService::sampleDataset(
        Dataset *dataset,
        int sampleSize) {

    QString pathToSample = this->generatePathToSample();
    QStringList filesInDirectory = scanFolderForEntFiles(dataset->pathProteins());

    QMap<QString, QString> sampledFiles = getSampleFromFileList(filesInDirectory, sampleSize);

    emit notifyTask("Copying sample");
    this->copyFilesIntoPath(sampledFiles.values(), pathToSample);

    dataset->setSampleSize(sampleSize);
    dataset->setPathSampledProteins(pathToSample);
}

void DatasetService::initializeSampleByLevel(SCOPEntryService &entLoader, const QStringList &filesInDirectory)
{
    emit notifyTask("Looking for classes");
    generateProgressSignal(0, 1);

    if(! (_sampleByScopLvl.empty() || _totalByScopLvl.empty())){
        _sampleByScopLvl.clear();
        _totalByScopLvl.clear();
    }

    QString className;
    SCOPEntry *protein;
    unsigned elements, fileCounter, proteinSize;

    fileCounter = 0;


    foreach(QString file, filesInDirectory){

        protein = entLoader.loadEntFile(file);
        className = protein->scss(_scopLevel);
        proteinSize = protein->numResidues();
        delete protein;


        if(_filterBySize) {

            if(proteinSize < _minProtSize ||
                    proteinSize > _maxProtSize) {

                generateProgressSignal(++fileCounter, filesInDirectory.size());
                continue;
            }
        }


        if(! _sampleByScopLvl.contains(className)){
            _sampleByScopLvl.insert(className, 0);

            _totalByScopLvl.insert(className, 1);
        } else{

            elements = _totalByScopLvl[className];
            _totalByScopLvl.insert(className, ++elements);

        }

        generateProgressSignal(++fileCounter, filesInDirectory.size());

    }
}

QMap<QString, QString> DatasetService::getParamSampleFromFileList(
        unsigned sampleSize,
        QStringList filesInDirectory) {
    RandomGenerator rand;
    bool continueVal = true;
    QMap<QString, QString> sampledFiles;
    SCOPEntryService entLoader;
    QString classTag;
    unsigned protsInClass, totalInClass, residuesInProt, expectedSampSize;

    qDebug() << "Files to sample: " << filesInDirectory.size();

    if(filesInDirectory.isEmpty()) return sampledFiles;

    if(_runSampleByScopLevel) {
        initializeSampleByLevel(entLoader, filesInDirectory);
        expectedSampSize = _sampleByScopLvl.size() * sampleSize;
    } else {
        expectedSampSize = sampleSize;
    }

    emit notifyTask("Generating Sample");
    generateProgressSignal(0, 1);
    do{
        int position = rand.randomIntBetween(0, filesInDirectory.size());
        QString pathSelectedFile = filesInDirectory.at(position);
        QFileInfo infoSelected(pathSelectedFile);

        SCOPEntry *scopProtein = entLoader.loadEntFile(pathSelectedFile);

        if (sampledFiles.contains(infoSelected.completeBaseName())){
            delete scopProtein;
            continue;
        }

        if (_filterBySize){
            residuesInProt = scopProtein->numResidues();
            if(residuesInProt < _minProtSize ||
                    residuesInProt > _maxProtSize){
                delete scopProtein;
                continue;
            }
        }

        classTag = scopProtein->scss(_scopLevel);
        delete scopProtein;

        if(_runSampleByScopLevel){
            protsInClass = _sampleByScopLvl.value(classTag);
            totalInClass = _totalByScopLvl.value(classTag);
            if(protsInClass == sampleSize || totalInClass == protsInClass){
                continue;
            } else {
                _sampleByScopLvl.insert(classTag, ++protsInClass);
            }

            //Just gonna use unused variables declared before to look if
            // I should continue with the sample
            classTag = "";
            protsInClass = 0;
            continueVal = false;

            foreach (classTag, _sampleByScopLvl.keys()) {
                protsInClass = _sampleByScopLvl.value(classTag);
                totalInClass = _totalByScopLvl.value(classTag);

                if(protsInClass < sampleSize &&
                        protsInClass < totalInClass) {
                    continueVal = true;
                    break;
                }
            }


        }

        sampledFiles.insert(infoSelected.completeBaseName(),
                            pathSelectedFile);

        if(! _runSampleByScopLevel) {
            continueVal = sampledFiles.values().size() < sampleSize;
        }

        generateProgressSignal(sampledFiles.size(), expectedSampSize);

    } while (continueVal);

    generateProgressSignal(1, 1);
    return sampledFiles;
}

void DatasetService::sampleByCriteria( Dataset *dataset, unsigned sampleSize){
    QString pathToSample = this->generatePathToSample();
    QStringList filesInDirectory = this->scanFolderForEntFiles(dataset->pathProteins());

    QMap<QString, QString> sampledFiles = getParamSampleFromFileList(sampleSize, filesInDirectory);

    copyFilesIntoPath(sampledFiles.values(), pathToSample);

    dataset->setHasSample(true);

    dataset->setSampleSize(sampledFiles.size());

    dataset->setPathSampledProteins(pathToSample);
}

//PROTECTED METHODS
QStringList DatasetService::filteredAstralFiles(
        QString pathToId,
        QString pathToEntFolder){

    if (pathToId.isEmpty() || pathToEntFolder.isEmpty()){
        LoggerService log;
        log.logError("No hay path a Id o Ent folder.");
        return QStringList();
    }
    QMap<QString, QString> entFilesMap;
    QDirIterator directoryIterator(
                pathToEntFolder,QDirIterator::Subdirectories);
    // Mapeo los archivos por nombre de proteína
    while (directoryIterator.hasNext()) {
        directoryIterator.next();
        QFileInfo infoFile(directoryIterator.filePath());

        if (infoFile.isFile() &&
                ("ent" == infoFile.suffix() ||
                 "pdb" == infoFile.suffix())) {
            entFilesMap [infoFile.completeBaseName()] = infoFile.filePath();
        }
    }


    QFile idFile(pathToId);
    QFileInfo fileInfoIdFile(pathToId);
    QStringList unexistingFiles, astralFiles;

    // Remove afterwards
//    SCOPEntry *se;
//    SCOPEntryService loader;
//    QStringList approvedClasses;
//    approvedClasses<< "a" << "b" << "c" << "d" << "e";
    if (idFile.open(QIODevice::ReadOnly)) {

        QTextStream in(&idFile);

        while (!in.atEnd())
        {
            QString id;
            QString line = in.readLine().trimmed();

            if ("id" == fileInfoIdFile.suffix()) {
                id = line;
            } else if ("fa" == fileInfoIdFile.suffix()
                       && line.startsWith(">")) {
                id = line.split(" ").first().remove(">");
            } else{
                continue;
            }

            // Si la clave en el archivo existe en el hash,
            // añado archivo a la lista

            if (entFilesMap.contains(id)){
//                se = loader.loadEntFile(entFilesMap.value(id));
//                QString classTag = se->scss(SCOPEntry::CLASS);
//                delete se;
//                if(approvedClasses.contains(classTag)) astralFiles << entFilesMap.value(id);
                astralFiles << entFilesMap.value(id);
            }else {
                unexistingFiles << id+".ent";
            }
        }
        idFile.close();

    }else {
        LoggerService log;
        log.logError("No puedo abrir archivo de Id");
    }

    if (! unexistingFiles.empty()){


        QString mensaje = "Archivos inexistentes:";
        LoggerService log;


        log.logError(mensaje, unexistingFiles);


    }
    return astralFiles;
}

bool DatasetService::copyFilesIntoPath(QStringList files, QString newPath){

    QStringList copiesWithErros;

    unsigned int contFile = 0;
    foreach (QString pathToFile, files) {
        QFileInfo infoEntFile(pathToFile);
        QString newFile = newPath+"/"+infoEntFile.fileName();
        QFile originalFile(pathToFile);
        if (! originalFile.copy(newFile)){
            copiesWithErros<<pathToFile+tr(" to ")+newPath;
        }

        generateProgressSignal(++contFile, files.size());
    }

    if (! copiesWithErros.empty()) {
        qDebug() << "There were errors during the copy of files";
        LoggerService log;
        log.logError(tr("No pude crear copiar archivos a ")+
                     tr("folder especificado."),
                     copiesWithErros);
        return false;
    }

    return true;
}

QString DatasetService::generatePathToSample(){
    ConfigurationService confService;
    QString pathToSample = confService.loadConfig()->workdirPath();
    pathToSample += "/sampled_datasets/";
    QString safeDateText = QDateTime::currentDateTime().toString().simplified();
    safeDateText.replace(" ", "_");
    safeDateText.replace(".", "");
    safeDateText.replace(":", "_");
    pathToSample += safeDateText;

    QDir sampleDir(pathToSample);

    if (! sampleDir.exists()){
        bool folderMade = sampleDir.mkpath(pathToSample);
        if (! folderMade) {
            LoggerService log;
            log.logError("No pude crear directorio para archivos muestra.",
                         QStringList()<<pathToSample);
            return "";
        }
    }

    return pathToSample;
}

QStringList DatasetService::scanFolderForEntFiles(QString folderPath){
    QStringList filesInDir;

    QDirIterator directoryIterator(
                folderPath,QDirIterator::Subdirectories);

    // Mapeo los archivos por nombre de proteína
    while (directoryIterator.hasNext()) {
        directoryIterator.next();
        QFileInfo infoFile(directoryIterator.filePath());

        if (infoFile.isFile() &&
                ("ent" == infoFile.suffix() ||
                 "pdb" == infoFile.suffix())) {
            filesInDir << infoFile.filePath();
        }
    }

    return filesInDir;
}
