#ifndef CLARAFORM_H
#define CLARAFORM_H

#include <QWidget>
#include <gui_forms/FormFeatures.h>
#include <modelo/pdbhandler.h>
#include <control/Procesors/claraprocesor.h>
#include <QAbstractButton>

namespace Ui {
class CLARAForm;
}

/**
 * @brief La clase CLARAForm mapea la vista de configuración del algorítmo CLARA del paquete Cluster de R.
 */
class CLARAForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief El constructor CLARAForm inicializa sin parámetros o el widget padre.
     * Crea sin parmetros el formulario de configuración para el algorítmo CLARA.
     * @param parent Objeto QWidget padre, por defecto es nulo
     */
    explicit CLARAForm(QWidget * const parent = NULL);

    /**
     * @brief Destructor sin parámetros
     */
    ~CLARAForm();

    /**
     * @brief setPathCaracComunes
     */
    void setPathCaracComunes(QString path_featuresComunes);

signals:
    /**
      * @brief tengoFeaturesCalculados
      */
     void tengoFeaturesCalculados(QString urlFeatures);


public slots:
     /**
       NOTE: Todos los forms de clustering tienen que tener este slot para conectar la señal del botón del form de features, en versiones futuras meter jerarquía
      * @brief calcularFootprintsModelo Slot para la señal de clicked sobre el botón de calcular footprints
      */
     void calcularFootprintsModelo();

private slots:

     /**
      * @brief modeloCalculado Recibe la señal que indica cuando el modelo de clasificación ha sido calculado
      * @param path_modelo path al archivo
      */
     void modeloCalculado( QString path_modelo );


     /**
      * @brief numHilosCambiado
      * @param numHilos
      */
     void numHilosCambiado(int numHilos);


    /**
     * @brief on_qpbt_ejecutar_clicked empieza con el proceso de identificación de características
     * Utiliza las proteínas en el dataset y ejecuta el proceso de selección de características comunes
     */
    void on_qpbt_ejecutar_clicked();

    /**
     * @brief progresoClara Slot para recibir notificación de los hilos de la proteína que ya ha sido procesada
     * @param nombreProt
     */
    void progresoClara(QString nombreProt);

    /**
     * @brief obtuvieronFeatures Slot para recibir notificación del hilo principal cuando ya se hayan obtenido los features
     * @param urlFeatures
     */
    void obtuvieronFeatures(QString urlFeatures);

private:
    // Métodos privados
    /**
     * @brief initAtributos inicializa atributos no inherentes a la a interfaz gráfica
     */
    void initAtributos();

    // Atributos privados
    /**
     * @brief ui es el objeto de configuración que contiene los elementos gráficos del formulario
     */
    Ui::CLARAForm *ui;
    /**
     * @brief form_clustering que contiene los datos generales de la configuración de clusters
     */
    FormFeatures *form_clustering;
    /**
      * @brief <PDBHandler *> pdbh_manejador es un puntero a la herramienta que gestiona los .PDBs y extrae los datos necesarios
      */
     PDBHandler * pdbh_manejadorProts = NULL;

     ClaraProcesor * claraProc;//! < Puntero a objeto con procesador de clara
     QThread * qtrd_hiloClara;

     QString pathFeaturesComunes;

     /**
      * @brief numProtsProcesar Número de proteínas que van a ser procesadas para generar los featrues
      */
     int numProtsProcesar;
     /**
      * @brief numProtsProcesadas Contador de proteínas que han sido procesadas
      */
     int numProtsProcesadas;


};

#endif // CLARAFORM_H
