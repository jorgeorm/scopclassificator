#include "GUI_SCOPPFoot.h"
#include "ui_GUI_SCOPPFoot.h"
#include <iostream>
#include <QFileDialog>
#include <string>
#include <QMessageBox>
#include <gui_forms/FormDataset.h> /// Carga del dataset
#include <gui_forms/FormFeatures.h> /// Caracterización de proteínas
#include <control/GestorDataset.h>
#include <utils/guiutils.h>///Para añadir funciones habituales de la GUI
#include <gui_forms/dialogformconfig.h>//Para configuración
//#include <QUiLoader>
/*Incluyo librer´ias para printf
*/
#include <cstdio>

/*Includes para que funcionara lo de obtener el home
*/
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/* Añado la capa de control
*/
#include "control/SCOPPFoot.h"

GUI_SCOPPFoot::GUI_SCOPPFoot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUI_SCOPPFoot)
{
    /*Inicializo los componentes de la clase
    */
//    RInside rins_default = RInside();
//    inicializarComponentes(rins_default);
//    inicializarComponentes();


    cargaronFeatures = false;
    qstr_urlFeaturesFounded = "";

    /*
     *Inicializo la GUI
    */
    ui->setupUi(this);
    ui->qpbr_avanceTarea->setValue(0);

    this->setEnabledCluster(false);

}

GUI_SCOPPFoot::~GUI_SCOPPFoot()
{
    delete ui;
}

void GUI_SCOPPFoot::setProgreso(int progreso){
    this->ui->qpbr_avanceTarea->setValue(progreso);
}

int GUI_SCOPPFoot::getProgreso()
{
    return this->ui->qpbr_avanceTarea->value();
}

void GUI_SCOPPFoot::inicializarComponentes()
{
/*Inicializo la clase de control de la herramienta
*/
}

void GUI_SCOPPFoot::cargarModelo(){
    QMessageBox qmsb_implementar;
    qmsb_implementar.setText("Aún no está implementado");
    qmsb_implementar.exec();
}

void GUI_SCOPPFoot::limpiarLayout(QLayout *ptrql_layout){
    QLayoutItem * item;
    QWidget * widgetDelete;
    while((item = ptrql_layout->takeAt(0))){
        widgetDelete = item->widget();
        delete widgetDelete;
    }
    delete ptrql_layout;
}

void GUI_SCOPPFoot::anadirAframeForms(QWidget *pqw_form){
    QLayout * layFrameForms = ui->qfrm_forms->layout();

    if( layFrameForms == NULL ){
        layFrameForms = new QVBoxLayout;
        ui->qfrm_forms->setLayout(layFrameForms);
    }else{
        GUIUtils::limpiarLayout(layFrameForms);
    }

    layFrameForms->addWidget(pqw_form);
}

void GUI_SCOPPFoot::on_qcbl_selecDataSet_clicked(){

    FormDataset * ptrqw_formDataset = new FormDataset(this);
//    this->anadirAwidget(ui->qwgt_general,ptrqw_formDataset);
    anadirAframeForms(ptrqw_formDataset);

}

void GUI_SCOPPFoot::on_qcbl_caracterizarDataset_clicked(){
    FormFeatures * ptrqw_formFeatures = new FormFeatures(this);
    if(cargaronFeatures){
        ptrqw_formFeatures->activarCalculoFootprints(qstr_urlFeaturesFounded);
    }
    anadirAframeForms(ptrqw_formFeatures);

//    this->anadirAwidget(ui->qwgt_general, ptrqw_formFeatures);

}


void GUI_SCOPPFoot::setEnabledCluster(bool enabled){
    this->ui->qcbl_caracterizarDataset->setEnabled(enabled);
}

void GUI_SCOPPFoot::on_qact_cargarFeatures_triggered()
{
    QString qstr_SCOPP_HOME = tr("%1/%2").arg(QDir::homePath(), "SCOPP_Files");
    qstr_urlFeaturesFounded = QFileDialog::getOpenFileName(this, tr("Seleccionar archivo de features"), qstr_SCOPP_HOME, tr("Archivos (*.*)"));

    //Verifico que existan proteínas a procesar, de no existir pongo deshabilitado el botón de procesar dataset
    int numProts =  GestorDataset::getInstance().getDataset()->size();

    if(QFile::exists(qstr_urlFeaturesFounded)){

        cargaronFeatures = true;

        if( numProts <= 0 ){ // Garantizo que existan

            on_qcbl_selecDataSet_clicked();

        } else{

            setEnabledCluster(true);
            on_qcbl_caracterizarDataset_clicked();
        }


    }else {
        QMessageBox mensajeError(this);
        mensajeError.setText("El archivo seleccionado no existe");
        mensajeError.setDetailedText(tr("El archivo seleccionado: %1, no existe.").arg(qstr_urlFeaturesFounded));

        mensajeError.exec();
    }
}

void GUI_SCOPPFoot::on_actionPreferencias_triggered(){
    DialogFormConfig * config = new DialogFormConfig(this);

    config->exec();
    delete config;
}
