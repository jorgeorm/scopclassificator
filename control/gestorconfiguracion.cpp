#include "control/gestorconfiguracion.h"
#include <QString>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

QString GestorConfiguracion::qstr_pathConfig = QString( "%1/SCOPP_Files" ).arg( QDir::homePath() );


GestorConfiguracion::GestorConfiguracion(){

    QFile configFile(QStringLiteral("%1/config.json").arg(qstr_pathConfig));
    if(configFile.exists()){
        cargarConfig();
    }else{
        config = new Configuracion();
        configurarDefecto(config);
    }

}

GestorConfiguracion::~GestorConfiguracion(){
    delete config;
}

GestorConfiguracion &GestorConfiguracion::getInstancia(){

    // La unica instancia del gestor
    // Garantiza inicialización perezosa
    // Garantiza que será destruido adecuadamente
    static GestorConfiguracion instancia;

    return instancia;
}

bool GestorConfiguracion::guardarConfig(){
    QFile configFile(QStringLiteral("%1/config.json").arg(qstr_pathConfig));

    if( !configFile.open(QIODevice::WriteOnly) ){
        qWarning("(EE) GestorConfiguiracion(guardarConfig): No se pudo abrir archivo de configuración.");
        return false;
    }

    QJsonObject configObject;
    config->escribirConfig(configObject);
    QJsonDocument configDoc(configObject);
    configFile.write(configDoc.toJson());

    configFile.flush();
    configFile.close();

    return true;
}

Configuracion *GestorConfiguracion::getConfig(){
    return config;
}

void GestorConfiguracion::configurarDefecto(Configuracion *obj_config){
    obj_config->setI_maxBytesFF(1024*1024*128);
    obj_config->setI_maxBytesFFLote(1024*1024*2048);
    obj_config->setI_maxResMem(900);
    obj_config->setQstr_ipRserve("127.0.0.1:6311");
    obj_config->setQstr_pathFFTMP( QString( "%1/ff" ).arg( QDir::tempPath() ) );
    obj_config->setQstr_pathTMP( QString( "%1/tmp" ).arg( qstr_pathConfig ) );
    obj_config->setI_maxHilos(2);

    guardarConfig();
}

bool GestorConfiguracion::cargarConfig(){
    QFile configFile(QStringLiteral("%1/config.json").arg(qstr_pathConfig));

    if( !configFile.open(QIODevice::ReadOnly) ){
        qWarning("(EE) GestorConfiguiracion(cargarConfig): No se pudo abrir archivo de configuración.");
        return false;
    }


    QByteArray datosConfig = configFile.readAll();
    QJsonDocument configDoc(QJsonDocument::fromJson(datosConfig));

    if( config != NULL ){
        delete config;
        config = NULL;
    }

    config = new Configuracion();
    config->leerConfig(configDoc.object());

    configFile.close();

    return true;
}



