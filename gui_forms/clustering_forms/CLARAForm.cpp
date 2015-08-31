#include "CLARAForm.h"
#include "ui_CLARAForm.h"
#include <RInside.h>
#include <iostream> //cout
#include <control/SCOPPFoot.h>
#include <control/GestorDataset.h>
#include <control/RParser.h> // Extracción submatrices
#include <algorithm> //
#include <math.h>       /* floor */
#include <QThread>
#include <QMessageBox>


using namespace std;

CLARAForm::CLARAForm(QWidget * const parent) :
    QWidget(parent),
    ui(new Ui::CLARAForm){
    ui->setupUi(this);

    //Saco puntero al widget de configuración general
    if(parent){
        form_clustering= qobject_cast<FormFeatures *>(parent);
        int numClust = max(form_clustering->numFeatClase(), form_clustering->numFeatDom());
        int sampSize = numClust*2;
        sampSize += 40;
        this->ui->qsbx_tamMuestra->setValue(sampSize);
        numProtsProcesadas = 0;
        numProtsProcesar = 0;
    }


    initAtributos();
}

void CLARAForm::initAtributos(){
//    pdbh_manejadorProts = new PDBHandler();
    claraProc = NULL;
//    rproc = new RController( 0, RInside::instance() );

}

CLARAForm::~CLARAForm(){
//    if(pdbh_manejadorProts != NULL){
//        delete pdbh_manejadorProts;
//        pdbh_manejadorProts = NULL;
//    }
    if( claraProc != NULL ){
        delete claraProc;
    }
    delete ui;
}

void CLARAForm::setPathCaracComunes(QString path_featuresComunes){
    pathFeaturesComunes = path_featuresComunes;
}

void CLARAForm::progresoClara( QString nombreProt ){

    ++numProtsProcesadas;
    int progreso = 0;
    progreso = numProtsProcesadas * 100 / ( numProtsProcesar );
    if(progreso != form_clustering->getProgreso()){
        form_clustering->setearProgreso( progreso );
    }
    if(progreso >= 100){
        ui->qpbt_ejecutar->setEnabled(true);
    }

    if( numProtsProcesadas % 10 == 0 ){
        cout<<"(II) ClaraForm(Progreso): Proteínas procesadas: "<<numProtsProcesadas<<endl;
    }

}


void CLARAForm::calcularFootprintsModelo(){

    if(claraProc != NULL){
        delete claraProc;
        if(qtrd_hiloClara->isRunning()){
            qtrd_hiloClara->quit();
            do{

            }while(qtrd_hiloClara->isRunning());
        }
        delete qtrd_hiloClara;
    }
    claraProc = new ClaraProcesor(0);
    qtrd_hiloClara = new QThread(0);



    if(form_clustering->tipoMatriz().compare( "Distancias" ) == 0 ){
        claraProc->setMaxMed( form_clustering->maxTolMap() );
    }

    claraProc->setSubMatSize( form_clustering->tamSubMat() );
    claraProc->setPoolProteinas( GestorDataset::getInstance().getDataset() );
    claraProc->setTipoMat( form_clustering->tipoMatriz() );
    claraProc->setPathArchivoMedoides( pathFeaturesComunes );
    claraProc->setNumFeatsClase(form_clustering->numFeatClase());
    claraProc->setNumHilos(form_clustering->numHilos());


    cout<<"(II) CLARAForm(calcularFootrpits): Seteó los parámetros necesarios"<<endl;

    connect(
                claraProc, SIGNAL( proceseProteina( QString ) ),
                this,           SLOT( progresoClara( QString ) ) );
    connect(
                claraProc, SIGNAL( calculeFootprintsModelo(QString) ),
                this,           SLOT( modeloCalculado(QString) )
                );

    numProtsProcesar = GestorDataset::getInstance().getDataset()->size();
    numProtsProcesadas = 0;


    form_clustering->activarCalculoFootprints(false);
    claraProc->calcularFootprintsModelo();



}

void CLARAForm::modeloCalculado(QString path_modelo){
    QMessageBox qmb_mensajeFeatures;
    qmb_mensajeFeatures.setText( tr("El modelo de clasificación fue guardado en:\n%1").arg( path_modelo ) );
    qmb_mensajeFeatures.exec();
}

void CLARAForm::numHilosCambiado(int numHilos){
    if( claraProc != NULL ){
        claraProc->setNumHilos( numHilos );
    }
}

void CLARAForm::obtuvieronFeatures(QString urlFeatures){
    QMessageBox qmb_mensajeFeatures;
    qmb_mensajeFeatures.setText( tr("El archivo fue guardado en:\n%1").arg( urlFeatures ) );
    qmb_mensajeFeatures.exec();

    form_clustering->activarCalculoFootprints( urlFeatures );
    cout<<"Recibí la señal de los features on "<<qPrintable(urlFeatures)<<endl;
}

void CLARAForm::on_qpbt_ejecutar_clicked(){

    ui->qpbt_ejecutar->setEnabled(false);
    claraProc = new ClaraProcesor(0);
    qtrd_hiloClara = new QThread(0);
    GestorDataset  & gdst_instancia = GestorDataset::getInstance();

    //Obtengo del formulario padre los clusters
    claraProc->setNumFeatsClase( form_clustering->numFeatClase() );
    claraProc->setNumFeatsDom( form_clustering->numFeatDom() );
    claraProc->setTipoMat( form_clustering->tipoMatriz() );
    claraProc->setSubMatSize( form_clustering->tamSubMat() );
    claraProc->setNumHilos(form_clustering->numHilos());

    if(form_clustering->tipoMatriz().compare( "Distancias" ) == 0 ){
        claraProc->setMaxMed( form_clustering->maxTolMap() );
    }

    //Config Parametros del procesador
    claraProc->setTamMuestra( ui->qsbx_tamMuestra->value() );
    claraProc->setNumMuestras( ui->qsbx_muestras->value() );

    int i_currentMetric = ui->qcbx_metrica->currentIndex();
    QString qstr_metrica = "";
    switch ( i_currentMetric ){
    case 0:
            qstr_metrica = "euclidean";
        break;
    case 1:
            qstr_metrica = "manhattan";
        break;
    }
    claraProc->setMetrica( qstr_metrica );

    claraProc->setFlagInterPam( ui->qcbx_pam->isChecked() );
    claraProc->setFlagRGenerad( ui->qcbx_aleatorios->isChecked() );

    // Seteo el pool de proteínas
    if ( gdst_instancia.estaMuestreado() ){
        cout<<"(II) ClaraForm(ejecutar): El dataset está muestreado. Tiene "<<gdst_instancia.getProteinasMuestreadas().size()<<" residuos."<<endl;
        claraProc->setPoolProteinas( gdst_instancia.getDatasetMuestreado() );
    }else {
        cout<<"(II) ClaraForm(ejecutar): Dataset con "<<gdst_instancia.getDataset()->size()<<" residuos."<<endl;
        claraProc->setPoolProteinas( gdst_instancia.getDataset() );
    }
    numProtsProcesar = gdst_instancia.getDataset()->size() +1 ;
    numProtsProcesadas = 0;
    claraProc->moveToThread( qtrd_hiloClara );

    connect(qtrd_hiloClara, SIGNAL( started() ), claraProc, SLOT( procesar() ) );
    connect(qtrd_hiloClara, SIGNAL(finished()), claraProc, SLOT(deleteLater()));
    connect(qtrd_hiloClara, SIGNAL(finished()), qtrd_hiloClara, SLOT(deleteLater()));

    connect(claraProc, SIGNAL( proceseProteina(QString) ), this, SLOT( progresoClara(QString) ) );
    connect(claraProc, SIGNAL( obtuveFeatures(QString) ), this, SLOT( obtuvieronFeatures(QString) ) );
    connect(claraProc, SIGNAL(finalizado()), qtrd_hiloClara, SLOT(quit()));

    qtrd_hiloClara->start();
}



