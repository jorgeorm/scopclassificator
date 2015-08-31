#ifndef GESTORCONFIGURACION_H
#define GESTORCONFIGURACION_H

#include <QString>
#include <QJsonObject>
#include <modelo/configuracion.h>///Modelo que almacenará los datos de la configuración.


/**
 * @author Jorge Ordoñez Mendez - jorge.ordonez@correounivalle.edu.co
 * @brief La clase GestorConfiguracion es una clase singleton para gestionar la configuracion de la aplicacion
 * Esta clase se encarga de cambiar la configuración de la aplicación con forme sea requerida para el entorno donde se utilice.
 */
class GestorConfiguracion
{
public:

    /**
     * @brief string cstr_pathConfig Path a la configuración
     */
    static QString qstr_pathConfig;

    /**
     * @brief ~GestorConfiguracion
     */
    virtual ~GestorConfiguracion();

    /**
     * @brief getInstancia Obtiene la instancia de la configuracion
     * Obtiene una referencia a la instancia existente de la clase en tiempo de ejecución.
     * @return GestorConfiguracion&
     */
    static GestorConfiguracion &getInstancia();

    /**
     * @brief guardarConfig Almacena permatentemente la configuración de la aplicación
     * @return <bool> Flag indicando si la operación pudo ser completada
     */
    bool guardarConfig();

    /**
     * @brief getConfig obtiene la configuración, de haber un archivo de confguración lo carga, si no devuelve objeto con configuración por defecto.
     * @return <Configuracion *> Puntero a objeto de configuración.
     */
    Configuracion * getConfig ();

    /**
     * @brief configurarDefecto Establece los valores por defecto
     */
    void configurarDefecto(Configuracion * obj_config);

private:
    /**
     * @brief GestorConfiguracion es método constructor sin parámetros, se declara privado para que se pueda utilizar en singleton.
     */
    GestorConfiguracion();
    //Métodos para asegurarme que aplica el patrón singletón para esta clase, no se implementan pero se declaran privados.
    //Detiene el compilador para generar métodos de copia del objeto
    GestorConfiguracion(GestorConfiguracion const &){};
    void operator = (GestorConfiguracion const &){};


    /**
     * @brief cargarConfig Lee archivo de configuración y lo carga.
     * @return <bool> Flag de estado indicando si la tarea pudo ser completada
     */
    bool cargarConfig();


    Configuracion *config;
};

#endif // GESTORCONFIGURACION_H
