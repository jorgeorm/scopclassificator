#include "ClusterFormFactory.h"
#include "clustering_forms/CLARAForm.h"
#include <iostream>

using namespace std;

QWidget * ClusterFormFactory::construirClusteringForm(int tipoTecnica, QWidget * qwgt_padre){
    switch (tipoTecnica){

        case CLARA:
            return new CLARAForm(qwgt_padre);
            break;
        default:
            cout<<"Método no soportado"<<endl;
            return NULL;
            break;

    }
}
