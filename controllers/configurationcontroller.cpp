#include "configurationcontroller.h"

#include <services/configurationservice.h>


ConfigurationController::ConfigurationController():
configService(){

}

Configuration *ConfigurationController::getConfig(){
    return configService.loadConfig();
}

bool ConfigurationController::storeConfig(Configuration const &config){
    return configService.storeConfig(config);
}
