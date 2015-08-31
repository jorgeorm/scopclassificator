#include "FormDataset.h"
#include "ui_FormDataset.h"
#include "qfiledialog.h"
#include "iostream"
#include <QTreeView>
#include <QMessageBox> // Para poder utilizar las qmessage box
/*
 * Incluyo librerías para cargar el homedir
*/
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/*
 * Incluyo las clases implementadas
 */
#include "control/GestorDataset.h"
#include "libs/bioLib/BioC++.h"

using namespace std;

FormDataset::FormDataset(QWidget *parent) :
    QWidget( parent ),
    ui(new Ui::FormDataset){
    ui->setupUi(this);

    if( parent ){
        gui_formPadre = qobject_cast<GUI_SCOPPFoot *>(parent);
    }

    struct passwd * psps_pass = getpwuid( getuid() );
    qstr_homeDir = tr(psps_pass->pw_dir);

    GestorDataset & dataset = GestorDataset::getInstance();
    limpiarTablaProteinas();
    if(dataset.estaMuestreado()){
        poblarListadoProteinas(dataset.getProteinasMuestreadas());

    }else if(dataset.getDataset()->size() > 0){
        poblarListadoProteinas(dataset.getDataset()->keys());
    }
}

FormDataset::~FormDataset(){

    qstr_homeDir = "";
    delete ui;

    //cout<<"(II) FormDataset(~FormDataset): Salió del destructor"<<endl;
}

bool FormDataset::cargarDirectorio(){

    QFileDialog * pqfd_selectorPDBs = new QFileDialog(
                this,
                tr("Seleccionar PDBs"),qstr_homeDir,
                tr("Archivos PDB (*.pdb *.PDB *.ent)"));
    pqfd_selectorPDBs->setFileMode(QFileDialog::Directory);
    pqfd_selectorPDBs->setOption(QFileDialog::DontUseNativeDialog, true);


    int i_salidaQfd = pqfd_selectorPDBs->exec();


    //Verifico si se confirmó una selección
    if(i_salidaQfd == QDialog::Accepted){

        //Obtengo instancia del dataset
        GestorDataset & dataset = GestorDataset::getInstance();

        //Obtengo los elementos seleccionados
        QStringList listaElementos= pqfd_selectorPDBs->selectedFiles();

        //Itero sobre elementos seleccionados
        foreach (QString qstr_elmnt, listaElementos){
            dataset.addProteina(qstr_elmnt);
        }

        return true;
    }

    return false;

}

bool FormDataset::cargarArchivoIds(){
    QString qstr_nombreArchivo = QFileDialog::getOpenFileName(
                this,
                tr("Abrir archivo IDS"),
                qstr_homeDir,
                tr("Archivo ID (*.ids *.fa)"));

    return false;
}

void FormDataset::on_qpbt_anadir_clicked(){
    //TODO: El file Dialog tiene que tener la capacidad de seleccionar folders y archivos
    //Configuro el qfileDialog

    bool exitoCarga = false;

    if( this->ui->qrb_dirEntrada->isChecked() ){
        exitoCarga = this->cargarDirectorio();
    }else if ( this->ui->qrb_idsEntrada->isChecked() ){
        exitoCarga = this->cargarArchivoIds();
    } else {
        QMessageBox::critical(
                    this,
                    tr("Se debe seleccionar un tipo de entrada."),
                    tr("Debe escoger si va a cargar las proteínas desde un directorio o desde un listado de proteínas filtradas."));
    }

    if( exitoCarga ){
        GestorDataset & dataset = GestorDataset::getInstance();
        QStringList listaProts;

        if( this->ui->qgb_muestDset->isChecked() ){ // Si se ha seleccionado que se desea hacer muestreo aleatorio
            int i_tamMuestra = ui->qsbx_tamMuestra->value();
            //Saco proteínas
            if( dataset.estaMuestreado() ){
                listaProts = dataset.getProteinasMuestreadas();
                listaProts << dataset.muestrearDataset(i_tamMuestra);
                dataset.setMuestraProteinas( listaProts );
            }else{
                listaProts = dataset.muestrearDataset(i_tamMuestra);
            }
            ui->qpte_informe->appendPlainText(
                        QString( "Se muestrearon  %1 proteínas al datset.").arg( i_tamMuestra ) );
        }else { // Si no se desea hacer muestreo aleatorio
            listaProts = dataset.getDataset()->keys();
        }

        limpiarTablaProteinas();
        poblarListadoProteinas(listaProts);
        ui->qpte_informe->appendPlainText(
                    QString("Se añadieron %1 proteínas al datset.").arg(
                        dataset.getDataset()->size()));
    }
}

QString FormDataset::bytesATamano(int i_numBytes){
    int kiloByte = 1024;
    int megaByte = kiloByte*1024;
    int gigaByte = megaByte*1024;
    int teraByte = gigaByte*1024;

    if(i_numBytes>=0 && i_numBytes<kiloByte){
        return QString("%1 B").arg(i_numBytes);
    } else if ((i_numBytes >= kiloByte) && (i_numBytes < megaByte)){
        return QString("%1 KB").arg(i_numBytes/kiloByte);
    } else if((i_numBytes >= megaByte) && (i_numBytes < gigaByte)){
        return QString("%1 MB").arg(i_numBytes/megaByte);
    } else if((i_numBytes >= gigaByte) && (i_numBytes < teraByte)){
        return QString("%1 GB").arg(i_numBytes/gigaByte);
    }

    return QString("0B'");
}

void FormDataset::limpiarTablaProteinas(){
    GestorDataset & gdts_dataset = GestorDataset::getInstance();
    QMap<QString, QString> * qmpa_dataset = gdts_dataset.getDataset();
    int numProteinas = 0;

    bool flag_muestreo = gdts_dataset.estaMuestreado();

    if( flag_muestreo ) numProteinas = gdts_dataset.getProteinasMuestreadas().size();
    else numProteinas = qmpa_dataset->size();

    for(int idProt = 0; idProt < numProteinas; idProt++){
        QTreeWidgetItem * qtwi_proteina = ui->qtwg_dataset->takeTopLevelItem(0);
        delete qtwi_proteina;
    }

    ui->qtwg_dataset->dumpObjectTree();

}

void FormDataset::on_qpbt_remover_clicked(){

    GestorDataset & gdts_datset = GestorDataset::getInstance();
    QList<QTreeWidgetItem *>qlst_seleccion = ui->qtwg_dataset->selectedItems();
    int contProts = 0;

    foreach(QTreeWidgetItem * qtwi_proteina, qlst_seleccion ){

        QString qstr_nomProt = qtwi_proteina->text(0);
        int respData = gdts_datset.removeNomProt(qstr_nomProt);
        if( respData == GestorDataset::FOUND){
            contProts++;
            delete qtwi_proteina;
        }else if(respData == GestorDataset::NOTFOUND){
            ui->qpte_informe->appendPlainText(QString("La proteína no existe en el dataset"));
//            cout<<"La proteína no existe en el dataset!!!"<<endl;
        }
    }

    ui->qpte_informe->appendPlainText(
                QString("Se quitaron %1 proteínas del dataset.").arg(contProts));

}

void FormDataset::on_qpbt_reset_clicked(){

    limpiarTablaProteinas();

    GestorDataset & gdts_dataset = GestorDataset::getInstance();
    QMap<QString, QString> * qmpa_dataset = gdts_dataset.getDataset();
    int numProteinas = 0;
    int tamMuestra = 0;

    bool flag_muestreo = gdts_dataset.estaMuestreado();
    numProteinas = qmpa_dataset->size();

    if( flag_muestreo ) {
        tamMuestra = gdts_dataset.getProteinasMuestreadas().size();

        gdts_dataset.removerMuestreo();
        ui->qpte_informe->appendPlainText(
                QString( "Se eliminaron %1 proteínas en la muestra" ).arg( tamMuestra ) );
    }

    qmpa_dataset->clear();

    ui->qpte_informe->appendPlainText(
                QString("Se eliminaron %1 proteínas del dataset.").arg(numProteinas) );

}

void FormDataset::on_qpbt_finalizar_clicked()
{
    GestorDataset & gdts_dataset = GestorDataset::getInstance();
    QMap<QString, QString> * qmap_dataset = gdts_dataset.getDataset();
    if(!qmap_dataset->empty()){
        gui_formPadre->setEnabledCluster(true);
    }

    this->~FormDataset();
}

void FormDataset::poblarListadoProteinas(QStringList qsl_listaProteinas){
    GestorDataset & dataset = GestorDataset::getInstance();

    int contProts = 0;


    QMap<QString, QString> * qmap_proteinas = dataset.getDataset();
    QString qstr_proteina;
    int tamListaProt = qsl_listaProteinas.size();

    for( int idProtEnList = 0;
        idProtEnList < tamListaProt && contProts<100;
        ++idProtEnList ){
        qstr_proteina = qmap_proteinas->value( qsl_listaProteinas.at( idProtEnList ) );

        Protein proteina;

        QFileInfo qfio_archivoProt(qstr_proteina);
        proteina.readPDB( qstr_proteina.toUtf8().data() );
//            cout<<"Leyó el PDB"<<qPrintable(itProt.key())<<"Y la ultima es: "<<qPrintable( qmap_proteinas->lastKey() )<<endl;
        QStringList qslt_regProteina;
        qslt_regProteina<<qfio_archivoProt.baseName();
        qslt_regProteina<<QString::number(proteina.residue_p.size());
        qslt_regProteina<<bytesATamano(qfio_archivoProt.size());
//            cout<<qPrintable(qslt_regProteina.join("\n"))<<endl;
        QTreeWidgetItem * item = new QTreeWidgetItem(qslt_regProteina);
        ui->qtwg_dataset->addTopLevelItem(item);
        contProts++;

    }

}
