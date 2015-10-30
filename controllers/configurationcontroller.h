#ifndef CONFIGURATIONCONTROLLER_H
#define CONFIGURATIONCONTROLLER_H

#include <models/configuration.h>

#include <services/configurationservice.h>



class ConfigurationController
{
public:
    ConfigurationController();

    /**
     * @brief getConfig
     * @return
     */
    Configuration *getConfig();
    /**
     * @brief storeConfig
     * @param config
     * @return
     */
    bool storeConfig(const Configuration &config);

private:
    ConfigurationService configService;
};

#endif // CONFIGURATIONCONTROLLER_H
