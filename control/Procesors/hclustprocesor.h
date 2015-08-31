#ifndef HCLUSTPROCESOR_H
#define HCLUSTPROCESOR_H
#include "control/FeaturesProcesor.h"
#include <QObject>
#include <QThread>

/**
 * @brief The HClustProcesor class
 */
class HClustProcesor : public FeaturesProcesor
{
    Q_OBJECT
public:

    //Atributos publicos y estáticos
    static QString qstr_pathDirTrabajo;
    static QString qstr_maquetaComando;//! < Comando de hclust parametrizado clara(x, k, metric = "euclidean", stand = FALSE, samples = 5, sampsize = min(n, 40 + 2 * k), trace = 0, medoids.x = TRUE, keep.data = medoids.x, rngR = FALSE, pamLike = FALSE)
    static QString qstr_resultadoClustering;//! < Comando de clara que extrae los medoides
    static QString qstr_nomVarMedoids;//! < Nombre de la variable que contiene los medoides obtenidos.
    static QString qstr_nomvarDistancias;//! < Nombre de la variable que contendrá las distancias calculadas.
    //Construcción / Destrucción
    /**
     * @brief HClustProcesor Constructor por defcto
     * @param parent
     */
    HClustProcesor( QObject *parent);

    /**
     * @brief ~HClustProcesor Destructor sin parametros
     */
    ~HClustProcesor();

protected:
    //Para correr multiples hilos
    vector <QThread *> vec_hilosSimultaneos;//!< Hilos que están corriendo de manera simultanea
    int i_numMaxHilos; //!< Cantidad máxima de hilos que correran de manera simultanea
    int i_numProcsCorriendo;//! < Cantidad de hilos que en determinado momento hay corriendo
    bool b_flagGPU;//!< Flag que indica si el procesamiento que se hará será utilizando la GPU


private:
    /**
     * @brief initAtributos Inicializa los atributos de la clase con los valores por defecto adecuados
     */
    void initAtributos();

    /**
     * @brief asignarHilo Se encarga de asignar a un hilo un Cliente de R y con figurar el cliente para correr la tarea de clustering
     * @param idHilo posición del hilo en el array de hilos simultaneos
     */
    void asignarHilo(int idHilo);

public:
    //Setters y Getters
    void setFlagGPU(bool b_gpu);
    void setMaxHilos(int i_maxHilos);


    //Métodos core

    /**
     * @brief procesar Inicia el procesamiento de las proteínas
     * Inicia con el procesado de las proteínas en paralelo, configura y ejecuta en paraleo clustering de proteínas, estas tareas son controladas a traves de señales y slots que continuan con la ejecución del procesamiento.
     */
    void procesar();

    /**
     * @brief calcularFootprints Utiliza los features que se hayan asignado y calcula los vectores footprint para cada una de las proteínas en el dataset.
     */
    void calcularFootprints();

};

#endif // HCLUSTPROCESOR_H
