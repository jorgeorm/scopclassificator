
#include "SCOPPFoot.h"
#include <string>
/*Includes para el debugueo
*/
#include <iostream>
#include <cstdio>

///*Includes de libreria Bioinf
//*/
//#include "bioLib/PRO.h"


using namespace Control;
using namespace std;

// Constructors/Destructors
//  

SCOPPFoot::SCOPPFoot ( )
{
    initAttributes();
}

SCOPPFoot::~SCOPPFoot ( )
{
    /*
     *  Borro las submatrices de memoria
    */
    //Lo sustituí por un clear del vector, se puede por los destructores en los punteros
//    while(!conjuntoSubmatrices.empty()){
//        Matriz * subMat = conjuntoSubmatrices.back();
//        conjuntoSubmatrices.pop_back();

//        delete subMat;
//        subMat = NULL;
//    }
    conjuntoSubmatrices.clear();

//    //cout<<"Borré conjunto de submatrices";

//    //cout<<"Entro al destructor de SCOPPFoot"<<endl;
    delete ptr_pdbHandler;
    ptr_pdbHandler  = NULL;

//    //cout<<"Borré el puntero al handler";

}

void SCOPPFoot::initAttributes ( )
{
    ptr_pdbHandler = new PDBHandler();
}


void SCOPPFoot::cargarPDB(string str_pathArchivo)
{
    cout<<"Voy a leer: "<<str_pathArchivo.c_str()<<endl;
    ptr_pdbHandler->leerPDB(str_pathArchivo.c_str());
//    cout<<"Voy a Filtrar: "<<str_pathArchivo.c_str()<<endl;
//    ptr_pdbHandler->filtrarAtomos("CA");
    cout<<"Voy a Mapear: "<<str_pathArchivo.c_str()<<endl;
    ptr_pdbHandler->mapearaMatriz(ptr_pdbHandler->DIST);
    cout<<"Voy a EscribirMatriz: "<<str_pathArchivo.c_str()<<endl;
    ptr_pdbHandler->escribirMatriz(ptr_pdbHandler->DIST);

    int tamMat = ptr_pdbHandler->getTamMatriz(ptr_pdbHandler->DIST);

    double ** matriz = ptr_pdbHandler->getMatriz(ptr_pdbHandler->DIST);

    int tamSubMat = 100;

//    FeaturesDataset * conjuntoSubmatsProt = ptr_pdbHandler->obtenerSubmatrices(matriz, tamSubMat, tamMat);

//    for(int idSubmat = 0; idSubmat < conjuntoSubmatsProt->size(); ++idSubmat){
//        conjuntoSubmatrices.push_back(conjuntoSubmatsProt->at(idSubmat));
//    }

    /**
     * Valido la salida de los subconjuntos
     */
//    //cout<<"El conjunto obtenido tiene "<<conjuntoSubmatsProt->size()<<" matrices"<<endl;

//    int tamConj = conjuntoSubmatsProt->size();
//    for(int idSubMat = 0; idSubMat < 3; idSubMat++){
//        Matriz * subMat = conjuntoSubmatsProt->at(idSubMat);
//        cout<<"Tam submat: "<<subMat->getNumRows()<<endl;
//        cout<<"Imprimo la subMatriz "<<idSubMat<<endl;

//        /*
//         * Imprimo subMatriz
//        */
//        for(int idy = 0; idy < tamSubMat; idy++){
//            for(int idx = 0; idx < tamSubMat; idx++){
//                cout<<subMat->getElement(idx,idy);
//                if(idx<tamSubMat-1){
//                    cout<<",";
//                }
//            }
//            cout<<endl;
//        }
//    }

    //cout<<"El conjunto total tiene "<<conjuntoSubmatsProt->size()<<" matrices"<<endl;

//    int tamConj = conjuntoSubmatsProt->size();
//    for(int idSubMat = 0; idSubMat < 3; idSubMat++){
//        Matriz * subMat = conjuntoSubmatsProt->at(idSubMat);
//        cout<<"Tam submat: "<<subMat->getNumRows()<<endl;
//        cout<<"Imprimo la subMatriz "<<idSubMat<<endl;

//        /*
//         * Imprimo subMatriz
//        */
//        for(int idy = 0; idy < tamSubMat; idy++){
//            for(int idx = 0; idx < tamSubMat; idx++){
//                cout<<subMat->getElement(idx,idy);
//                if(idx<tamSubMat-1){
//                    cout<<",";
//                }
//            }
//            cout<<endl;
//        }
//    }


    /*
     * Limpio subconjunto de matrices de la proteina que esta siendo cargada
     */
//    for(int idSubMat = 0; idSubMat < tamConj; idSubMat++){
//        Matriz * subMat = conjuntoSubmatsProt->at(idSubMat);
//        delete subMat;
//        subMat = NULL;
//    }

//    delete conjuntoSubmatsProt;

}

void SCOPPFoot::cargarPDBs(const string * arry_pathsArchivo, int tamEntrada)
{
    for (int idArchivo = 0; idArchivo < tamEntrada; ++idArchivo) {
        this->cargarPDB(arry_pathsArchivo[idArchivo]);
    }
}
