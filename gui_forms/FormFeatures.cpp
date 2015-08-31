#include "FormFeatures.h"
#include "ui_FormFeatures.h"
#include "iostream"
#include <gui_forms/ClusterFormFactory.h>
#include <gui_forms/clustering_forms/CLARAForm.h>
#include <utils/guiutils.h>//Para limpiar layouts
#include "control/gestorconfiguracion.h"
#include "modelo/configuracion.h"

using namespace std;

FormFeatures::FormFeatures(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFeatures)
{
    ui->setupUi(this);

    if(parent){
        gui_ppal= qobject_cast<GUI_SCOPPFoot *>(parent);
    }

    Configuracion *config  = GestorConfiguracion::getInstancia().getConfig();
    ui->qsbx_numHilos->setValue( config->getI_maxHilos() );

    on_qcbx_clustering_activated(tr("CLARA"));
}

FormFeatures::~FormFeatures(){
    delete ui;
}

void FormFeatures::on_qcbx_clustering_activated(const QString &qstr_seleccion){

//    TODO: Meter jerarquía en los forms por algoritmo que permita conectar al slot para el botón de calcular footprints
//    qstr_seleccion.compare(QString("K-Medoides"))
//    QLayoutItem * item;
//    while((item = this->ui->qvbl_clusteringTech->takeAt(0))){
//        delete item;
//    }

    if(qstr_seleccion.compare(tr("CLARA")) == 0){
        CLARAForm * form_clara  =(CLARAForm *) ClusterFormFactory::construirClusteringForm(ClusterFormFactory::CLARA, this);
        connect( form_clara, SIGNAL( tengoFeaturesCalculados( QString ) ), this, SLOT( activarCalculoFootprints( QString ) ) );
//        connect( this->ui->qpb_calcularFootprint, SIGNAL( clicked() ), form_clara, SLOT( calcularFootprints() ) );
        connect( this->ui->qsbx_numHilos, SIGNAL( valueChanged(int) ), form_clara, SLOT( numHilosCambiado(int) ) );

        formClustering = form_clara;
    }


    QLayout * destino = ui->qfra_formTecClust->layout();

    if( destino != NULL ){
        GUIUtils::limpiarLayout( destino );
    }else {
        destino = new QVBoxLayout;
        ui->qfra_formTecClust->setLayout(destino);
    }

    destino->addWidget(formClustering);



//    this->ui->qvbl_clusteringTech->addWidget( formClustering );

}

void FormFeatures::activarCalculoFootprints( QString ulr_features ){
    URL_FEATURES = ulr_features;

    ui->qpb_calcularFootprint->setEnabled( true );
}

int FormFeatures::getProgreso(){
    return this->gui_ppal->getProgreso();
}

QString FormFeatures::getURLFeatures(){
    return this->URL_FEATURES;
}

void FormFeatures::anadirAwidget(QWidget * ptrqw_destino, QWidget * ptrqw_widget){

    if(ptrqw_destino->layout() != NULL){
        limpiarLayout(ptrqw_destino->layout());
    }

    if(ptrqw_widget != NULL){ // Verifico que el widget cargado no sea nulo.
        QLayout * layout = new QGridLayout(ptrqw_destino);
        layout->addWidget(ptrqw_widget);
        ptrqw_destino->setLayout(layout);
    }

}

void FormFeatures::setearProgreso(int progreso){
    this->gui_ppal->setProgreso(progreso);
}

void FormFeatures::limpiarLayout(QLayout *ptrql_layout){
    QLayoutItem * item;
    QWidget * widgetDelete;
    while((item = ptrql_layout->takeAt(0))){
        widgetDelete = item->widget();
        delete widgetDelete;
    }
    delete ptrql_layout;
}

int FormFeatures::tamSubMat(){
    return ui->qsbx_tamSubm->value();
}

int FormFeatures::numFeatDom(){
    return ui->qsbx_numFeatDomin->value();
}

int FormFeatures::numFeatClase(){
    return ui->qsbx_numFeatClase->value();
}

int FormFeatures::numHilos(){
    return ui->qsbx_numHilos->value();
}

QString FormFeatures::tipoMatriz(){
    return ui->qcbx_tipoMatriz->currentText();
}

double FormFeatures::maxTolMap(){
    return ui->qdsb_maxVal->value();
}

void FormFeatures::on_qcbx_tipoMatriz_activated(const QString &arg1)
{
    if(arg1.compare(tr("Distancias")) == 0){
        if(!ui->qdsb_maxVal->isVisible()){
            ui->qdsb_maxVal->setVisible(true);
            ui->qlbl_maxVal->setVisible(true);
        }
    }else{
        if(ui->qdsb_maxVal->isVisible()){
            ui->qdsb_maxVal->setVisible(false);
            ui->qlbl_maxVal->setVisible(false);
        }
    }
}


void FormFeatures::on_qpb_calcularFootprint_clicked()
{
    QString qstr_seleccion = this->ui->qcbx_clustering->currentText();

    if(qstr_seleccion.compare(tr("CLARA")) == 0){
        CLARAForm * form_clara  = (CLARAForm *) formClustering;
        form_clara->setPathCaracComunes( URL_FEATURES );
        form_clara->calcularFootprintsModelo();

    }
}
