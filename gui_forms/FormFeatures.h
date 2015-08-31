#ifndef FORMFEATURES_H
#define FORMFEATURES_H

#include <QWidget>
#include <GUI_SCOPPFoot.h>

namespace Ui {
class FormFeatures;
}

class FormFeatures : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del formulario
     * Construye y configura el formulario del método de clustering K-Medoids
     * @param parent
     */
    FormFeatures(QWidget *parent);

    /**
     * \brief Destruye el formulario
     * Destructor por defecto del formulario
     */
    ~FormFeatures();

    /**
     * @brief maxTolMap obtiene el máximo de tolerancia para mapeo en caso de haberse seleccionado la matriz de distancias
     * @return <double> Máximo valor tolerado para la matriz de características.
     */
    double maxTolMap();

    /**
     * @brief tamSubMat
     * @return <int> Tamaño de las submatrices
     */
    int tamSubMat();

    void setearProgreso(int progreso);

    /**
     * @brief numFeatDom
     * @return <int> Número de Clusters por dominio
     */
    int numFeatDom();

    /**
     * @brief numFeatClase
     * @return <int> Número de Clusters por clase
     */
    int numFeatClase();

    /**
     * @brief numHilos
     * @return
     */
    int numHilos();

    /**
     * @brief tipoMatriz
     * @return <QString> Tipo de matriz con la que mapeara las proteínas
     */
    QString tipoMatriz();

    /**
     * @brief getURLFeatures Obtiene la URL al archivo de features seleccionado
     * @return <QString> URL al archivo con features
     */
    QString getURLFeatures();

    /**
     * @brief Obtiene valor de la barra de progreso
     * @return
     */
    int getProgreso();

public slots:
    void activarCalculoFootprints( QString url_features );

private slots:
    /**
     * @brief on_qcbx_clustering_activated se ejecuta cuando el combo box se activa.
     * Se ejecuta cuando el Combo Box es activado y se ha realizado una selección del método de
     * @param <QString &> qstr_seleccion Elemento seleccionado del QComboBox
     */
    void on_qcbx_clustering_activated(const QString & qstr_seleccion);

    void on_qcbx_tipoMatriz_activated(const QString &arg1);


    void on_qpb_calcularFootprint_clicked();

private:
    Ui::FormFeatures *ui;
    GUI_SCOPPFoot * gui_ppal;
    QWidget * formClustering;
    //Métodos privados

    /**
     * \brief Carga a un form widget en un widget especificado
     * Carga a un widget especificado como destino un nuevo widget en su layout
     * \param <QWidget *> ptrqw_destino Widget al cual se le va a cargar el widget.
     * \param <QWidget *> ptrqw_widget Nuevo widget que será cargado en el componente especificado anteriormente
     */
    void anadirAwidget(QWidget * ptrqw_destino, QWidget * ptrqw_widget);

    /**
     * @brief limpiarLayout Toma un layout y eliminia todo lo que este contenga
     * @param ptrql_layout
     */
    void limpiarLayout(QLayout * ptrql_layout);

    QString URL_FEATURES;
};

#endif // FORMFEATURES_H
