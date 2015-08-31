#include "GestorDataset.h"
#include "gestorconfiguracion.h" // para sacar valores de configuración
#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>
#include <math.h> // Para valor absoluto
#include "utils/randomutils.h" // Para generador de semillas

#include <iostream>
using namespace std;

GestorDataset::GestorDataset(){

    pdbh_manejadorProts = new PDBHandler();
    qsrand( RandomUtils::randomSeed() );
    flag_muestreado = false;

}

QMap <QString, QString>* GestorDataset::getDataset(){
    return &qmap_proteinas;
}

void GestorDataset::removerMuestreo(){
    flag_muestreado = false;
    qsls_proteinasMuestreadas.clear();
    cout<<"GestorDataset(removerMue..): Limpio lista proteínas y flag."<<endl;
}

GestorDataset & GestorDataset::getInstance(){

    // La unica instancia del gestor
    // Garantiza inicialización perezosa
    // Garantiza que será destruido adecuadamente
    static GestorDataset instancia;

    return instancia;
}

void GestorDataset::addProteina(QString & qstr_proteina){

    if(!esDirectorio(qstr_proteina)){
        QFileInfo qfio_proteina(qstr_proteina);
        qmap_proteinas.insert(qfio_proteina.baseName(),qstr_proteina);
    }else{
        //Pego los folders hijos
        cargarDirectorio(qstr_proteina);
    }
}

GestorDataset::~GestorDataset(){
    qmap_proteinas.clear();
}

bool GestorDataset::esDirectorio(QString & qstr_url){
    QFileInfo infoUrl (qstr_url);

    return infoUrl.isDir();
}

void GestorDataset::cargarDirectorio(QString & qstr_urlDir){
    QString nodo;
    QStringList nodosDir;
    QStringList partialNodes;

    nodosDir.append(qstr_urlDir);
    int numArchivos = 0;


    do{
        nodo = nodosDir.first() ;
        nodosDir.removeFirst();


        //Pego directorios a lista de nodos (nodos en arbol de dir)
        QDir qdir_directorio(nodo);
        qdir_directorio.setFilter(
                    QDir::Dirs|QDir::NoDotAndDotDot|QDir::Hidden|QDir::NoSymLinks);

        partialNodes.clear();
        partialNodes = qdir_directorio.entryList();
        QString newNode;

        foreach (QString partialNode, partialNodes) {

            newNode = QString("%1/%2").arg(
                        qdir_directorio.absolutePath(),
                        partialNode);
            nodosDir.append( newNode );
        }


        //Proceso los archivos en el directorio (hojas en arbol de dir)
        qdir_directorio.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
        QStringList qslt_filtro;
        qslt_filtro<<"*.pdb"<<"*.PDB"<<"*.ent";
        qdir_directorio.setNameFilters(qslt_filtro);
        qdir_directorio.setSorting(QDir::Size);
        QStringList archivos = qdir_directorio.entryList();

        foreach (QString archivo, archivos) {
    //        cout<<"Archivo: ";
    //        cout<<QString(qdir_directorio.absolutePath()+"/"+archivo).toUtf8().constData()<<endl;
            QString qstr_uriProt = qdir_directorio.absolutePath()+"/"+archivo;
            QFileInfo qfio_proteina(qstr_uriProt);
            qmap_proteinas.insert(qfio_proteina.baseName(), qstr_uriProt);
            numArchivos++;
        }

    }while ( !nodosDir.isEmpty() );

//    Debug
//    cout<<"GestorDataset(cargarDir..): Se añadieron al dataset "<<numArchivos<<" archivos del directorio "<<qPrintable( qstr_urlDir )<<endl;
}


int GestorDataset::removeNomProt(const QString &qstr_nomProt){
    if(qmap_proteinas.contains(qstr_nomProt)){
        int resultMap = qmap_proteinas.remove(qstr_nomProt);
        if(resultMap == 1) return FOUND;
        else if(resultMap == 0) return NOTFOUND;
    }
}

QStringList GestorDataset::getProteinasMuestreadas(){
    return qsls_proteinasMuestreadas;
}

QMap<QString, QString> *GestorDataset::getDatasetMuestreado(){
    qmap_sample.clear();
    foreach (QString nomProt, qsls_proteinasMuestreadas) {
        qmap_sample.insert( nomProt, qmap_proteinas.value(nomProt) );
    }
    return &qmap_sample;
}



void GestorDataset::setMuestraProteinas(QStringList listaProteinas){
    this->qsls_proteinasMuestreadas = listaProteinas;
}

QStringList GestorDataset::muestrearDataset(int tam_muestra){

//    Debug
//    cout<<"GestorDataset(muestreoD...): Entró al método de muestreo"<<endl;

    QStringList listaProteinas = this->qmap_proteinas.keys(); //Liista de proteínas sobre las que se va a realizar el muestreo.
    int i_idElementoAleatorio = 0; //Número aleatorio obtenido, se le añadirá al iterador.
    int i_contadorMuestra = 0; // Número de muestras que se han sacado.
    int i_numIntentCercaLim = 100; // Número máximo de intentos de búsqueda, quemado en código
    int i_contIntentosCercaLim = 0; // Número de veces que ha buscado aleatoriamente una proteína cercana al límite
    bool flag_busquedaCercaLim = false; // Flag que determina si se está buscando una proteína cerca al límite.
    int i_minDiferenciaCota = INFINITY; // Valo mínimo encontrado en la diferencia al límite.
    int i_diferenciaCota = INFINITY; // Valor temporal calculado con la diferencia al límite
    qsls_proteinasMuestreadas.clear();// Garantizo que la lista de proteínas muestreadas se haya limpiado
     int i_maxRes = GestorConfiguracion::getInstancia().getConfig()->getI_maxResMem(); // Limite de tamapo de proteínas para procesamiento en memoria.
    QString nomProtMuest;
    QString nomProtOptLimite;
    int i_tamanoProteina = 0;

    do{
        //Obtengo proteína aleatoriamente
        i_idElementoAleatorio = qrand() % listaProteinas.size( );
        nomProtMuest = listaProteinas.at( i_idElementoAleatorio );
        //Verifica restricciones de tamaño.
        pdbh_manejadorProts->leerPDB( qPrintable( qmap_proteinas.value(nomProtMuest) ) );
        i_tamanoProteina = pdbh_manejadorProts->getNumResiduos();

        if( flag_busquedaCercaLim ){
            if( ++i_contIntentosCercaLim == i_numIntentCercaLim ){
                flag_busquedaCercaLim = false;
                qsls_proteinasMuestreadas<<nomProtOptLimite;
                listaProteinas.removeAll( nomProtOptLimite );
                ++i_contadorMuestra;
                continue;
            }

            i_diferenciaCota = std::abs( i_maxRes - i_tamanoProteina );

            if( i_minDiferenciaCota > i_diferenciaCota ){
                nomProtOptLimite = nomProtMuest;
                i_minDiferenciaCota = i_diferenciaCota;
            }

        } else if( i_tamanoProteina > i_maxRes ){
            flag_busquedaCercaLim = true;

            i_minDiferenciaCota = std::abs( i_maxRes - i_tamanoProteina );
            i_contIntentosCercaLim = 0;
            nomProtOptLimite = nomProtMuest;
            listaProteinas.removeAt( i_idElementoAleatorio );

        }else {
            qsls_proteinasMuestreadas<<nomProtMuest;
            listaProteinas.removeAll( nomProtOptLimite );
            ++i_contadorMuestra;
        }

    }  while( i_contadorMuestra < tam_muestra );

//     Debug
//     cout<<"GestorDataset(muestrearD...): Se realizó muestreo aleatorio obteniendo "<<tam_muestra<<" proteínas."<<endl;
    flag_muestreado = true;
    return qsls_proteinasMuestreadas;
}

bool GestorDataset::estaMuestreado(){
    return flag_muestreado;
}

