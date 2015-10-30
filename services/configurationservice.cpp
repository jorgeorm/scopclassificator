#include "configurationservice.h"
#include "loggerservice.h"

#include <models/configuration.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>

QString ConfigurationService::_pathConfigStore = QString ("%1/.SCOPClassificator").arg (QDir::homePath());

ConfigurationService::ConfigurationService(){
    if(! QDir(_pathConfigStore).exists()) QDir().mkdir(_pathConfigStore);
}

ConfigurationService::~ConfigurationService()
{
    delete _actualConfig;
}

Configuration *ConfigurationService::loadConfig (){

    QFile configFile(QStringLiteral("%1/config.json").arg(_pathConfigStore));
    if(configFile.exists()){


        _actualConfig = this->getConfigFromFile(configFile);
    }else{


        _actualConfig = new Configuration();
        this->setDefaultValuesToConfig(_actualConfig);


        this->storeConfig(*_actualConfig);


    }

    return _actualConfig;
}

Configuration *ConfigurationService::getConfigFromFile(QFile &configFile){
    if(! configFile.open (QIODevice::ReadOnly)){
        LoggerService logger;
        logger.logError("No se pudo abrir archivo de configuración");
        return NULL;
    }

    QByteArray datosConfig = configFile.readAll();
    QJsonObject configObject(QJsonDocument::fromJson(datosConfig).object());

    Configuration *configuration = new Configuration (
            configObject["ipRserve"].toString(),
            configObject["workdirPath"].toString(),
            configObject["ffWorkdirPath"].toString(),
            configObject["maxBytesFF"].toVariant().toLongLong(),
            configObject["maxBytesFFBatch"].toVariant().toLongLong(),
            configObject["maxResNumberInMemory"].toInt(),
            configObject["maxSimThreadNumber"].toInt());
    return configuration;
}

void ConfigurationService::setDefaultValuesToConfig(Configuration *config){
    config->setMaxBytesFF(1024*1024*128);
    config->setMaxBytesFBatch(1024L*1024L*2048L);
    config->setMaxResiduesInLocalMem(900);
    config->setIpRserve("127.0.0.1:6311");
    config->setFfWorkdirPath(QString("%1/ff").arg (QDir::tempPath()));
    config->setWorkdirPath(QString ("%1/tmp").arg (_pathConfigStore));
    config->setMaxSimThread(2);

    if(! QDir(config->ffWorkdirPath()).exists()){
        QDir().mkdir(config->ffWorkdirPath());
    }

    if(! QDir(config->workdirPath()).exists()){
        QDir().mkdir(config->workdirPath());
    }
}

bool ConfigurationService::storeConfig (const Configuration &_newConfig){
    QFile configFile(QStringLiteral("%1/config.json").arg(_pathConfigStore));

    if( !configFile.open(QIODevice::WriteOnly) ){
        LoggerService logger;
        logger.logError("No se pudo abrir archivo de configuración");
        return false;
    }

    QJsonObject configObject;
    configObject["ipRserve"] = _newConfig.ipRserve();
    configObject["workdirPath"] = _newConfig.workdirPath();
    configObject["ffWorkdirPath"] = _newConfig.ffWorkdirPath();
    configObject["maxBytesFF"] = QJsonValue::fromVariant (QVariant (_newConfig.maxBytesFF()));
    configObject["maxBytesFFBatch"] = QJsonValue::fromVariant (QVariant (_newConfig.maxBytesFBatch()));
    configObject["maxResNumberInMemory"] = _newConfig.maxResiduesInLocalMem();
    configObject["maxSimThreadNumber"] = _newConfig.maxSimThread();

    QJsonDocument configDoc(configObject);
    configFile.write(configDoc.toJson());

    configFile.flush();
    configFile.close();

    return true;
}
