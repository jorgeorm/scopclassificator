#ifndef GUI_SCOPPFoot_H
#define GUI_SCOPPFoot_H

#ifndef MAIN
#define MAIN
#endif // MAIN

#include <QMainWindow>
#include "control/SCOPPFoot.h"

namespace Ui {
class GUI_SCOPPFoot;
}


/*! \brief Clase generada por QTCreator, representa la clase de la GUI principal.
 * Crea la GUI que se muestra incialmente, mapea los metodos de conexion
 * \version 0.01
 * \author Jorge Ordoñez Mendez jorge.ordonez@correounivalle.edu.co
 */
class GUI_SCOPPFoot : public QMainWindow
{
    Q_OBJECT
/*Constructores y destructores priivados
*/
public:

    /*! \brief Constructor de clase
     * Inicializar la clase SCOPPFoot con lo configurado en el .ui
     * \param parent Puntero al elemento padre de esta interfaz grafica
    */
    explicit GUI_SCOPPFoot(QWidget *parent = NULL);

    /*! \brief Destructor de clase
     * Se encarga de eliminar los objetos y punteros que la clase utilice
    */
    ~GUI_SCOPPFoot();

/* métodos públicos
*/
    /**
     * @brief setEnabledCluster habilita o deshabilita el botón para selección de clusters
     * @param <bool> enabled Boleano que indica el estado del botón
     */
    void setEnabledCluster(bool enabled);

    /**
       @brief setProgreso actualiza la barra de progreso de la GUI
       @param <int> progreso valor a reflejar en la GUI;
     */
    void setProgreso(int progreso);

    /**
     * @brief getUrlFeatures obtiene la URL a los features cargados
     * @return QString URL a los features
     */
    QString getUrlFeatures();


    /**
     * @brief getCargaronFeatures obtiene la bandera indicando si fueron cargados features previamente
     * @return bool flag que indica si se cargaron o no features
     */
    bool getCargaronFeatures();

    /**
     * @brief getProgreso obtiene el valor del progress bar
     * @return
     */
    int getProgreso();

/*
 * Signals
*/
signals:
    void featuresProvistos(QString uri_features);

/*Slots privados para eventos desde la GUI
*/
private slots:
    /*! \brief Inicia la carga de modelos previamente construidos
     * Slot para el qact_cargarModelo en el form .ui, permite cargar un modelo previamente construido
     * para la clásificación de proteínas.
     */
    void cargarModelo();


    /** ! \brief Permite seleccionar la ubicación de la URI de los archivos para el modelo
     * Slot para el CommandLinkButton en el .ui denominado qclb_selecDataSet en el evento de click, permite seleccionar
     * el data set con el cual se entrenará el modelo clasificador de proteínas.
     */
    void on_qcbl_selecDataSet_clicked();

    /**
     * @brief Abre formulario para la configuración de los algoritmos de clustering
     * Abre el formulario para la configuración de los clusters  que identificaran las características
     */
    void on_qcbl_caracterizarDataset_clicked();

    /**
     * @brief on_qact_cargarFeatures_triggered Abre formulario para seleccionar el archivo con features generado
     */
    void on_qact_cargarFeatures_triggered();


    void on_actionPreferencias_triggered();

private:
    /* Atributos privados
    */
    Ui::GUI_SCOPPFoot * ui; //!< Puntero de la interfaz grafica cargada del .ui

    bool cargaronFeatures;
    QString qstr_urlFeaturesFounded;
/* Metodos privados
*/
private:
    /** \brief Inicializa los atributos que componen la GUI
    */
    void inicializarComponentes();

    /**
     * @brief limpiarLayout Toma un layout y eliminia todo lo que este contenga
     * @param ptrql_layout
     */
    void limpiarLayout(QLayout * ptrql_layout);


// Aproximación descartada, consistía en intentar cargar archivos .ui en tiempo de ejecución
//     /**
//     * \brief Carga elementos .ui forms en elemento de la interfáz gráfica.
//     * Recibe una petición por un achivo .ui, lo carga y muestra en un widget especificado
//     * \param <QString> qstr_uiForm Dirección al form el cual será cargado
//     * \param <QWidget> ptrqw_destino Elemento de la GUI al cual se le va a cargar el form.
//     */
//    void cargarElementoUI(QString qstr_uiForm, QWidget * ptrqw_destino);

//    /**
//     * \brief Carga a un forms widgets en un widget especificado
//     * Carga a un widget especificado como destino un nuevo widget en su layout
//     * \param <QWidget *> ptrqw_destino Widget al cual se le va a cargar el widget.
//     * \param <QWidget *> ptrqw_widget Nuevo widget que será cargado en el componente especificado anteriormente
//     */
//    void anadirAwidget(QWidget * ptrqw_destino, QWidget * ptrqw_widget);

    /**
     * @brief anadirAframeForms Carga un form en widget al contenedor de formularios en la GUI
     * @param <QWidget *> pqw_form Formulario como widget
     */
    void anadirAframeForms( QWidget * pqw_form );

};


#endif // GUI_SCOPPFoot_H
