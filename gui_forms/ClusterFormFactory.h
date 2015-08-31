#ifndef CLUSTERFORMFACTORY_H
#define CLUSTERFORMFACTORY_H

#include <QWidget>

/**
 * @brief La clase ClusterFormFactory inicializa forms de configuración de algortimos de clustering
 * Se encarga de inicializar formularios de configuración de técnicas de clustering en tiempo de ejecución.
 */
class ClusterFormFactory
{
public:

    /**
     * @brief The CLUSTERING_TECH enum mapea los formularios que la fábrica puede producir
     */
    enum CLUSTERING_TECH{
        CLARA
    };

    /**
     * @brief construirClusteringForm crea una instancia de form de clustering que se va a ejecutar
     * @param <int> tipoTecnica que debe ser obtenido del CLUSTERING_TECH
     * @return <QWidget *> Form del widget
     */
    static QWidget * construirClusteringForm(int tipoTecnica, QWidget * qwgt_padre);



private:
    // Métodos privados para que no existan constructores y destructores
    /**
     * @brief ClusterFormFactory Construye sin parametros la fábrica
     * Inicializa los atributos requeridos para el funcionamiento de la fábrica
     */
    ClusterFormFactory(){}

    //Edito constructores
    ClusterFormFactory(ClusterFormFactory const &){}
    void operator = (ClusterFormFactory const &){}

    /**
     * @brief ~ClusterFormFactory destruye sin parametros la fábrica
     */
    virtual ~ClusterFormFactory(){}

};

#endif // CLUSTERFORMFACTORY_H
