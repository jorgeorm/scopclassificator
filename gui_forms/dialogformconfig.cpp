#include "dialogformconfig.h"
#include "ui_dialogformconfig.h"
#include <QPushButton>
#include "control/gestorconfiguracion.h"
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

using namespace std;

DialogFormConfig::DialogFormConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFormConfig){
    ui->setupUi(this);

    setWindowTitle(tr("Configuración"));
    GestorConfiguracion & gestorConfig = GestorConfiguracion::getInstancia();
    config = gestorConfig.getConfig();

    cargarConfiguracion();
//    ui->qdbb_almacenar->button(QDialogButtonBox::Save)->setText(tr("Guardar"));
//    ui->qdbb_almacenar->button(QDialogButtonBox::RestoreDefaults)->setText(tr("Valores por defecto"));
//    ui->qdbb_almacenar->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));

}

DialogFormConfig::~DialogFormConfig(){
    delete ui;
}

void DialogFormConfig::cargarConfiguracion(){

    ui->ledt_rserveHost->setText(config->getQstr_ipRserve());
    ui->ledt_dirTmp->setText(config->getQstr_pathTMP());
    ui->ledt_dirFFtmp->setText(config->getQstr_pathFFTMP());
    ui->qsbx_numHilos->setValue(config->getI_maxHilos());
    ui->qsbx_maxMemoriaFF->setValue(config->getI_maxBytesFF()/(1024*1024));
    ui->qsbx_tamLote->setValue(config->getI_maxBytesFFLote()/(1024*1024));
    ui->qsbx_maxrResiduosMem->setValue(config->getI_maxResMem());

}

void DialogFormConfig::guardarClicked(){
    //Meto los datos de la vista al modelo
    long int maxFFBytes = ui->qsbx_maxMemoriaFF->value();
    maxFFBytes *= 1024*1024;
    config->setI_maxBytesFF( maxFFBytes );

    long int maxFFLote = ui->qsbx_tamLote->value();
    maxFFLote *= 1024*1024;
    config->setI_maxBytesFFLote( maxFFLote );

    config->setI_maxResMem( ui->qsbx_maxrResiduosMem->value() );
    config->setQstr_ipRserve( ui->ledt_rserveHost->text() );
    config->setQstr_pathFFTMP( ui->ledt_dirFFtmp->text() );
    config->setQstr_pathTMP( ui->ledt_dirTmp->text() );
    config->setI_maxHilos( ui->qsbx_numHilos->value() );

    //Almaceno en archivo de configuración
    GestorConfiguracion &gestorConf = GestorConfiguracion::getInstancia();
    if( !gestorConf.guardarConfig() ){
        QMessageBox::critical( this, tr("Error en configuración"), tr("Error al intentar guardar archivo de configuración, no fue posible."));
    }
}

void DialogFormConfig::cancelarClicked(){
    this->close();
}

void DialogFormConfig::resetearClicked(){
    GestorConfiguracion & gestorConfig = GestorConfiguracion::getInstancia();
    gestorConfig.configurarDefecto( config );
    cargarConfiguracion();
}

void DialogFormConfig::on_qdbb_almacenar_clicked(QAbstractButton *button){
    QDialogButtonBox::ButtonRole rol = ui->qdbb_almacenar->buttonRole( button );

    switch (rol) {
    case QDialogButtonBox::AcceptRole:
        guardarClicked();
        break;
    case QDialogButtonBox::ResetRole:
        resetearClicked();
        break;
    case QDialogButtonBox::RejectRole:
        cancelarClicked();
        break;
    }
}

void DialogFormConfig::on_tbtn_dirTmp_clicked(){
    QString qstr_pathFolder = QFileDialog::getExistingDirectory(
                this,
                tr("Directorio que contendrá los datos temporales generados por los procesos de clustering."),
                GestorConfiguracion::qstr_pathConfig
                );
    if( qstr_pathFolder != NULL ){
        this->ui->ledt_dirTmp->setText( qstr_pathFolder );
    }

}

void DialogFormConfig::on_tbtn_dirFFtmp_clicked(){
    QString qstr_pathFolder = QFileDialog::getExistingDirectory(
                this,
                tr("Directorio que contendrá los datos temporales generados por los procesos de clustering."),
                GestorConfiguracion::qstr_pathConfig
                );
    if( qstr_pathFolder != NULL ){
        this->ui->ledt_dirFFtmp->setText( qstr_pathFolder );
    }
}
