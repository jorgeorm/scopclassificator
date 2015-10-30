#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include <QString>
#include <QDir>

#include <models/configuration.h>

class ConfigurationService
{
public:
    ConfigurationService();
    virtual ~ConfigurationService();

    /**
     * @brief setDefaultValuesToConfig
     * @param config
     */
    void setDefaultValuesToConfig(Configuration *config);
    /**
     * @brief storeConfig
     * @param _newConfig
     * @return
     */
    bool storeConfig(const Configuration &_newConfig);

    /**
     * @brief loadConfig
     * @return
     */
    Configuration *loadConfig();
signals:

public slots:
private:
    QString static _pathConfigStore;
    Configuration *_actualConfig;

    /**
     * @brief getConfigFromFile
     * @param configFile
     * @return
     */
    Configuration *getConfigFromFile(QFile &configFile);
};

#endif // CONFIGURATIONSERVICE_H
