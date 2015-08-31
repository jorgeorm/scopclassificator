#ifndef FORMDATASET_H
#define FORMDATASET_H

#include <QWidget>
#include <GUI_SCOPPFoot.h>
#include <QStringList> // Para que poblar listado de proteinas sea parametrizable

namespace Ui {
class FormDataset;
}

class FormDataset : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormDataset(QWidget *parent = 0);
    virtual ~FormDataset();
    
private slots:
    /**
     * \brief Muestra un dialogo para la selección de archivos .PDB y directorios
     * Llama a un dialogo de selección de archivos y carpetas, una vez han sido seleccionados o no,
     * los agrega al dataset.
     */
    void on_qpbt_anadir_clicked();

    /**
     * @brief Saca del dataset y de la vista las proteínas seleccionadas
     * Toma los elementos seleccionados en la vista y los elimina del dataset y la vista.
     */
    void on_qpbt_remover_clicked();

    /**
     * @brief Resetea el dataset y quita todas las proteínas que lo componen
     *
     */
    void on_qpbt_reset_clicked();

    /**
     * @brief Finaliza el proceso de selección y devuelve
     *
     */
    void on_qpbt_finalizar_clicked();

private:
    Ui::FormDataset *ui;
    QString qstr_homeDir;
    QStringList qstr_dataSet;
    GUI_SCOPPFoot * gui_formPadre;

/*
 * Métodos privados
*/
    /**
     * @brief poblarListadoProteinas Se encarga de llenar la tabla coon las proteínas que están en el dataset
     */
    void poblarListadoProteinas(QStringList qsl_listaProteinas);

    /**
     * @brief cargarDirectorio Abre formulario para seleccionar folder y cargar archivos .ent y .pdb en un directorio.
     * @return <bool>
     */
    bool cargarDirectorio();

    /**
     * @brief cargarArchivoIds Abre formulario para seleccionar archivo ID con las proteínas.
     * @return <bool>
     */
    bool cargarArchivoIds();

private:

    /** \brief Slot para el qact_archivo en el form .ui, permite seleccionar el archivo a cargar
     * Desencadena la aparicion del widget para la seleccion del archivo a cargar
    */
    void abrirArchivo();

    /** \brief Slot para el qact_folder en el form .ui, permite seleccionar carpetas a cargar
     * Desencadena la aparicion del widget para la seleccion de la carpeta a cargar
    */
    void abrirCarpeta();

    /**
     * \brief Formatea numero de bytes en formato humano
     * Toma un número de bytes, los procesa y retorna el QString correspondiente a la representación humana de los bytes
     * \param <int> i_numBytes bytes que entran a la función para
     * \return <QString> Número de bytes formateados para lectura de humano.
     */
    QString bytesATamano(int i_numBytes);

    /**
     * @brief limpiarTablaProteinas elimina los elementos que haya en la tabla de proteínas.
     */
    void limpiarTablaProteinas();

};

#endif // FormDataset_H
