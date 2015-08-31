#ifndef GESTORDATASET_H
#define GESTORDATASET_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <modelo/pdbhandler.h> //Para cargar Proteínas.


/**
 * \author Jorge Ordoñez Mendez - jorge.ordonez@correunivalle.edu.co
 * \brief Clase singleton para gestionar el dataset de proteínas.
 * Esta clase se encarga de añadir o remover elementos al dataset utilizado por el software de clasificación, está implementada en
 * singleton para que pueda ser utilizada por los diferentes controladores.
 */
class GestorDataset{
public:

    enum SEARCHRESULT{
        FOUND,
        NOTFOUND,
        SEARCHERROR
    };

    /**
     * @brief Singleton que obtiene instancia
     * Obtiene la instancia existente del objeto en tiempo de ejecución
     * @return GestorDataset&
     */
    static GestorDataset& getInstance();

    /**
     * @brief Destructor por defecto
     */
    virtual ~GestorDataset();

    /**
     * @brief Añade proteina al dataset
     * Recibe la url de una proteina para ser cargada en el dataset utilizado.
     * @param <QString *> qstr_proteina URI donde se encuentra la proteína que se va a cargar
     */
    void addProteina(QString & qstr_proteina);



    /**
     * @brief Obtiene las proteínas en el dataset
     * Obtiene las proteínas que componen el dataset en forma de un puntero a un QStringList
     * @return <QStringList *> Puntero a la lista de QStrings con las proteínas en el dataset
     */
    QMap<QString, QString> * getDataset();

    /**
     * @brief removerMuestreo se encarga de remover el muestreo que se le haya hecho al dataset.
     */
    void removerMuestreo();

    /**
     * @brief Remueve una proteína a partir del nombre de la proteína
     * @param <QString> qstr_nomProt Referencia constante a el nombre de la proteína
     * @return <int> FOUND si la proteína fue exitosamente removida,
     * NOTFOUND si la proteína no pudo ser encontrada en el dataset, ERROR si no se pudo remover la proteína.
     */
    int removeNomProt(const QString & qstr_nomProt);


    //Setters y Getters para muestreo
    /**
     * @brief getProteinasMuestreadas
     * @return
     */
    QStringList getProteinasMuestreadas();


    /**
     * @brief getDatasetMuestreado obtiene el dataset como mapa de nombre de proteínas mas el path al .ent
     * @return QMap<QString, QString> * puntero al datasetMuestreado
     */
    QMap<QString, QString> * getDatasetMuestreado();

    /**
     * @brief setMuestraProteinas
     * @param listaProteinas
     */
    void setMuestraProteinas( QStringList listaProteinas );

    /**
     * @brief Realiza muestreo aleatorio simple sobre el dataset
     * @return <QStringList> qsls_proteinasMuestreadas Lista de proteínas seleccionadas del dataset.
     */
    QStringList muestrearDataset(int tamMuestra);

    /**
     * @brief estaMuestreado
     * @return <bool> flag indicando si el dataset ha sido muestreado
     */
    bool estaMuestreado();

private:
    /**
     *
     * @brief Constructor Singleton por defecto sin parámetros
     * Instancia inicialmente el objeto cuando es requerido, no requiere parámetros.
     */
    GestorDataset();

    //Métodos para asegurarme que aplica el patrón singletón para esta clase, no se implementan pero se declaran privados.
    //Detiene el compilador para generar métodos de copia del objeto
    GestorDataset(GestorDataset const &){};
    void operator = (GestorDataset const &){};

    //Atributos privados
    QMap<QString, QString> qmap_proteinas; ///< QMap el dataset con la llave como el nombre de la proteína y valor las URIs de las proteínas del dataset
    QMap<QString, QString> qmap_sample; ///< QMap la muestra obtenida para que los algoritmos construyan el modelo

    PDBHandler * pdbh_manejadorProts; /// < Puntero a <PDBHandler> Clase util para gestionar los datos formateados de la proteína.

    QStringList qsls_proteinasMuestreadas; ///< QStringList con el listado de proteínas que fueron muestreadas
private:
    /**
     * @brief Verifica si la URL es un directorio o un achivo
     * @param <QString> qstr_urlElemento URL al directorio que se desea verificar
     * @return <bool> true si es un directorio, false si es un archivo
     */
    bool esDirectorio(QString & qstr_url);

    /**
     * @brief Carga los pdbs existentes en un directorio
     * @param <QString> qstr_urlDir URL al directorio que será incluido en el dataset
     */
    void cargarDirectorio (QString & qstr_urlDir);


    /**
     * @brief <bool> flag_muestreado Identifica si el dataset ha pasado por muestreo aleatorio
     */
    bool flag_muestreado;
};

#endif // GESTORDATASET_H
