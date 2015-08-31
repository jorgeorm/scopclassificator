#include "pdbhandler.h"

/*
 * Imports para el trim
 */
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

PDBHandler::PDBHandler(){
    mat_angulos = NULL;
    mat_distancias = NULL;
    nombreProteina = "";

}


void PDBHandler::limpiarMatrices(){
    /*
     * Elimino las matrices de distancias y angulos de la memoria
    */

    int numAtomos = sizeof mat_distancias / sizeof (double);

    if(mat_distancias != NULL){
        for (int idCol = 0; idCol < numAtomos; ++idCol) {
            if(mat_distancias[idCol] != NULL ) {
                delete [] mat_distancias[idCol];
            }
        }
        delete [] mat_distancias;
    }


    int numResiduos = sizeof mat_angulos / sizeof (double);

    if(mat_angulos != NULL){
        for (int idCol = 0; idCol < numResiduos; ++idCol) {
            if(mat_angulos[idCol] != NULL) {
                delete [] mat_angulos[idCol];
            }
        }
        delete [] mat_angulos;
    }


}


PDBHandler::~PDBHandler(){

    limpiarMatriz(this->AMBAS);

    limpiarVector(atomosFiltrados_ptr);

    limpiarVector(atomos_ptr);

    limpiarVector(residuos_ptr);

}

void PDBHandler::mapearaMatriz(int tipoMatriz, double maxTol)
{
    switch(tipoMatriz){
    case ANGS:
        calcularMatrizAngulos();
        break;
    case DIST:
        calcularMatrizDistancias(maxTol);
        break;
    case AMBAS:
        calcularMatrizAngulos();
        calcularMatrizDistancias();
        break;
    }
}

void PDBHandler::escribirDistancias(const char *cptr_pathArchivo){
    FILE * flujoSalida; //<< flujo donde se escribira la matriz

    string pathTmp;
    unsigned numAtomosFilt = atomosFiltrados_ptr.size();

    if(cptr_pathArchivo != NULL){
        pathTmp = string ( cptr_pathArchivo );
    } else {
        pathTmp = string(this->nombreProteina);
        pathTmp = pathTmp.substr(0 , pathTmp.size()-4);
    }

    pathTmp.append("_DIST.csv");

    try{

        flujoSalida = fopen( pathTmp.c_str(), "w" ); //< Creo el flujo

        if(flujoSalida == NULL){ //< Valido que se haya podido abrir
            cerr<<"No se pudo abrir el archivo de salida: "
                <<pathTmp.c_str()<<endl;
            exit(1);
        }else {
            /*
             * Escribo el encabezado del archivo
            */
            for(int idAtom = 0; idAtom < numAtomosFilt; idAtom++){
                fprintf(flujoSalida, "Ca%u",idAtom);
                if(idAtom < numAtomosFilt-1){
                    fprintf(flujoSalida, ";");
                }
            }
            fprintf(flujoSalida, "\n");

            /*
             * Imprimo la matriz
            */
            for(int idRow = 0; idRow < numAtomosFilt; idRow++){

                for(int idCol = 0; idCol < numAtomosFilt; idCol++){
                    fprintf(flujoSalida,
                            "%.4f",
                            mat_distancias[idRow][idCol]);
                    if(idCol < numAtomosFilt-1){
                        fprintf(flujoSalida, ";");
                    }
                }

//                if(idRow < numAtomosFilt-1){
                    fprintf(flujoSalida, "\n");
//                }
            }
            fclose(flujoSalida);
        }

    } catch ( char * pMensaje ){

        cerr<<endl<<"Excepcion: "<<pMensaje<<endl;

    }
}

void PDBHandler::escribirMatriz(int tipoMatriz, const char * cptr_pathArchivo)
{
    switch(tipoMatriz){
    case AMBAS:
        escribirDistancias(cptr_pathArchivo);
        escribirAngulos(cptr_pathArchivo);
        break;
    case ANGS:
        escribirAngulos(cptr_pathArchivo);
        break;
    case DIST:
        escribirDistancias(cptr_pathArchivo);
        break;
    }
}

int PDBHandler::getNumResiduos(){
    return residuos_ptr.size();
}

void PDBHandler::escribirAngulos(const char *cptr_pathArchivo){

    FILE * flujoSalida; //<< flujo donde se escribira la matriz

    string pathTmp;

    if(cptr_pathArchivo != NULL){
        pathTmp = string ( cptr_pathArchivo );
    } else {
        pathTmp = string(this->nombreProteina);
        pathTmp = pathTmp.substr(0 , pathTmp.size()-4);
    }

    pathTmp.append("_ANGS.csv");

    try{

        flujoSalida = fopen( pathTmp.c_str(), "w" ); //< Creo el flujo

        if(flujoSalida == NULL){ //< Valido que se haya podido abrir
            cerr<<"No se pudo abrir el archivo de salida: "
                <<pathTmp.c_str()<<endl;
            exit(1);
        }else {

            int numRes = residuos_ptr.size();
            /*
             * Escribo el encabezado del archivo
            */
            for(int idAtom = 0; idAtom < numRes; idAtom++){
                fprintf(flujoSalida, "R%u",idAtom);
                if(idAtom < numRes-1){
                    fprintf(flujoSalida, ";");
                }
            }
            fprintf(flujoSalida, "\n");

            /*
             * Imprimo la matriz
            */
            for(int idRow = 0; idRow < numRes; idRow++){

                for(int idCol = 0; idCol < numRes; idCol++){
                    fprintf(flujoSalida,
                            "%.4f",
                            mat_angulos[idRow][idCol]);
                    if(idCol < numRes-1){
                        fprintf(flujoSalida, ";");
                    }
                }

//                if(idRow < numAtomosFilt-1){
                    fprintf(flujoSalida, "\n");
//                }
            }
            fclose(flujoSalida);
        }

    } catch ( char * pMensaje ){

        cerr<<endl<<"Excepcion: "<<pMensaje<<endl;

    }

}

/**
 * @brief PDBHandler::getTamMatriz Obtiene el tamaño de la matriz solicitada
 * @param tipoMatriz enum DIST: Distancias, o ANGS: angulos
 * @return <int> Cantidad de elementos en la matirz
 */
int PDBHandler::getTamMatriz(int tipoMatriz){
    switch (tipoMatriz){
        case DIST:
            return i_tamDistancias;
            break;
        case ANGS:
            return i_tamAngulos;
            break;
    }
}

Atom * PDBHandler::extraerAtomo(const char *nomAtomo, Residue *residuo){
    int numAtomos = residuo->atom.size();

    for(int idAtom = 0; idAtom < numAtomos; idAtom++){

        string nomAtomoRes (residuo->atom[idAtom].name);

        nomAtomoRes =trim(nomAtomoRes);

        if(nomAtomoRes.compare(nomAtomo) == 0 ){
//            cout<<"El atomo "<<nomAtomoRes<<" esta en: ";
//            cout<<residuo->atom[idAtom].xyz[0]<<",";
//            cout<<residuo->atom[idAtom].xyz[1]<<",";
//            cout<<residuo->atom[idAtom].xyz[2]<<endl;
            Atom * atomoSalida = copiarAtomo( & residuo->atom[idAtom] );
            return  atomoSalida;
        }
    }
}

double * PDBHandler::calcularVecNormal(Residue * ptr_residuo){

    if(ptr_residuo != NULL){
//        cout<<"El residuo tiene algo"<<endl;

        double * normal = new double[3];

        Atom * atomoN = extraerAtomo("N",ptr_residuo);

        Atom * atomoCa = extraerAtomo("CA",ptr_residuo);

        Atom * atomoC = extraerAtomo("C",ptr_residuo);

        Atom NCa = Atom();

        Atom CaC = Atom();

        for(int i=0; i<3; ++i){
            NCa.xyz[i] = atomoCa->xyz[i] - atomoN->xyz[i];
            CaC.xyz[i] = atomoC->xyz[i] - atomoCa->xyz[i];
        }

        /*
         * Producto cruz de los vectores NCa y CaC
        */

        Atom NCaxCaC = NCa * CaC;

        /*
         * Calculo la norma del vector tridimensional NCaxCaC
        */

        double normaNCaxCaC = 0.0;

        for(int i=0; i<3; i++){
            normaNCaxCaC += pow(NCaxCaC.xyz[i], 2.0);
        }

        normaNCaxCaC = sqrt(normaNCaxCaC);

        /*
         * Normalizo el vector
        */

        for(int i=0; i<3; i++){
            normal[i] = NCaxCaC.xyz[i] / normaNCaxCaC;
        }

        delete atomoC;
        delete atomoCa;
        delete atomoN;

        return normal;
    } else {
        return NULL;
    }
}

double PDBHandler::calcularAngulo(Residue *residuo1, Residue *residuo2){
    /*
     * Calculo vectores normales
     */
    double * normalRes1 = calcularVecNormal(residuo1);
    double * normalRes2 = calcularVecNormal(residuo2);

    // Calculo el valor (norm1)^2 y (norm2)^2
    double norm1_pow2 = 0.0;
    double norm2_pow2 = 0.0;
    double normN2N1_pow2 = 0.0;

    for(int i = 0; i < 3; i++ ){
        // Norma^2 de los vectores normales

        norm1_pow2 += pow(normalRes1[i], 2.0);
        norm2_pow2 += pow(normalRes2[i], 2.0);

        // Norma^2 del vector n2-n1
        double coordDif = normalRes2[i] - normalRes1[i];
        normN2N1_pow2 += pow(coordDif, 2.0);
    }

    double cosTetha = 0.0;

    cosTetha = norm1_pow2+norm2_pow2-normN2N1_pow2;
    cosTetha /= (2.0*sqrt(norm1_pow2)*sqrt(norm2_pow2));


    // Borro los vectores normales;
    delete [] normalRes1;
    delete [] normalRes2;

//    cout<<cosTetha<<";";


    return acos(cosTetha);
}

void PDBHandler::calcularMatrizAngulos(){
    if(mat_angulos == NULL){
        this->initMatriz(this->ANGS);
    }else {
        this->limpiarMatriz(this->ANGS);
        this->initMatriz(this->ANGS);
    }

    int numRes = residuos_ptr.size();

    cout<<"Residuos para: "<<this->nombreProteina<<" son: "<<numRes<<endl;

    for(int idRow = 0; idRow < numRes; ++idRow){
        for(int idCol = 0; idCol < numRes; ++idCol){

            if(idCol == idRow){
                break;
            }

            Residue * residuo1 = residuos_ptr[idRow];
            Residue * residuo2 = residuos_ptr[idCol];

            double angulo = calcularAngulo(residuo1, residuo2);

            mat_angulos[idRow][idCol] = angulo;
            mat_angulos[idCol][idRow] = angulo;
        }

    }
}


void PDBHandler::calcularMatrizDistancias(double maxTolera, string nomAtomoRef){
//    double valTmp = 0; // < Almacena temporalmente el calculo parcial de distancia
    this->filtrarAtomos(nomAtomoRef.c_str());

    if(mat_distancias == NULL){
        this->initMatriz(this->DIST);
    }else {
        this->limpiarMatriz(this->DIST);
        this->initMatriz(this->DIST);
    }


    double sumTmp = 0.0; // < Temporalmente almacena la suma para calcular distancias
    int numAtomos = atomosFiltrados_ptr.size(); // < Cantidad de atomos Ca en la proteina


    for (int idRow = 0; idRow < numAtomos; ++idRow) {
        for (int idCol = 0; idCol < numAtomos; ++idCol) {
            if(idRow != idCol){

                Atom  atomoTmp1 = * atomosFiltrados_ptr[idRow];
                Atom  atomoTmp2 = * atomosFiltrados_ptr[idCol];

                sumTmp = atomoTmp1 - atomoTmp2;//Saca el contenido del puntero para calcular las distancias valiendose del operador

                if(sumTmp < maxTolera){
                    mat_distancias[idRow][idCol] = sumTmp;
                    mat_distancias[idCol][idRow] = sumTmp;
                }else {
                    mat_distancias[idRow][idCol] = maxTolera;
                    mat_distancias[idCol][idRow] = maxTolera;
                }

            } else {
                break;
            }
        }
    }
}


void PDBHandler::limpiarMatriz(int tipoMatriz)
{
//    cout<<"Entre a limpiar matriz "<<tipoMatriz<<endl;

    switch(tipoMatriz)
    {
    case ANGS:

        for (int idRow = 0; idRow < i_tamAngulos; ++idRow) {
            delete [] mat_angulos[idRow];
        }
        delete [] mat_angulos;
        mat_angulos = NULL;
        i_tamAngulos = 0;

        break;
    case DIST:

        for (int idRow = 0; idRow < i_tamDistancias; ++idRow) {
//            cout<<"Voy a borrar la fila "<<idRow<<endl;
            delete [] mat_distancias[idRow];
        }
        delete [] mat_distancias;
        mat_distancias = NULL;
        i_tamDistancias = 0;

        break;
    case AMBAS:

        if(mat_angulos != NULL){

            for (int idRow = 0; idRow < i_tamAngulos; ++idRow) {
                delete [] mat_angulos[idRow];
            }
            delete [] mat_angulos;
            mat_angulos = NULL;
            i_tamAngulos =0;

//            cout<<"Limpié la matriz de angulos"<<endl;

        }

        if(mat_distancias != NULL){
            for (int idRow = 0; idRow < i_tamDistancias; ++idRow) {
                delete [] mat_distancias[idRow];
            }
            delete [] mat_distancias;
            mat_distancias = NULL;
            i_tamDistancias = 0;

//            cout<<"Limpié la matriz de distancias"<<endl;

        }


        break;
    }
}

void PDBHandler::initMatriz(int tipoMatriz){

    int numResiduos = residuos_ptr.size();
    switch(tipoMatriz)
    {
    case ANGS:
        i_tamAngulos = numResiduos;
        mat_angulos = new double * [i_tamAngulos];
        for (int idRow = 0; idRow < i_tamAngulos; ++idRow) {
            mat_angulos [idRow] = new double[i_tamAngulos];

            for (int idCol = 0; idCol < i_tamAngulos; ++idCol) {
                    mat_angulos [idRow][idCol] = 0.0;
            }
        }
        break;
    case DIST:
        numResiduos = atomosFiltrados_ptr.size();
        i_tamDistancias = numResiduos;
        mat_distancias = new double * [numResiduos];
        for (int idRow = 0; idRow < i_tamDistancias; ++idRow) {
            mat_distancias [idRow] = new double[i_tamDistancias];
            for (int idCol = 0; idCol < i_tamDistancias; ++idCol) {
                mat_distancias [idRow][idCol] = 0.0;
            }
        }
        break;
    case 2:
        i_tamAngulos = numResiduos;
        mat_angulos = new double * [numResiduos];
        i_tamDistancias = atomosFiltrados_ptr.size();
        mat_distancias = new double * [i_tamDistancias];

        for (int idRow = 0; idRow < i_tamAngulos; ++idRow) {
            mat_angulos [idRow] = new double [i_tamAngulos];
            for (int idCol = 0; idCol < i_tamAngulos; ++idCol) {
                mat_angulos [idRow][idCol] = 0.0;
            }
        }

        for (int idRow = 0; idRow < i_tamDistancias; ++idRow) {
            mat_distancias [idRow] = new double [i_tamDistancias];
            for (int idCol = 0; idCol < i_tamDistancias; ++idCol) {
                mat_distancias [idRow][idCol] = 0.0;
            }
        }
        break;
    }
}

Residue * PDBHandler::copiarResiduo(Residue residuoActual){
    Residue * ptrResiduo = new Residue();

    ptrResiduo->active = residuoActual.active;
    ptrResiduo->atom.swap(residuoActual.atom);
    strcpy(ptrResiduo->chainName, residuoActual.chainName);
    ptrResiduo->chainSeqlNo = residuoActual.chainSeqlNo;
    ptrResiduo->endAtomSeqlNo = residuoActual.endAtomSeqlNo;
    strcpy(ptrResiduo->homology, residuoActual.homology);
    ptrResiduo->hydrophobicity = residuoActual.hydrophobicity;
    ptrResiduo->mapAtom.insert(residuoActual.mapAtom.begin(), residuoActual.mapAtom.begin());
    strcpy(ptrResiduo->name, residuoActual.name);
    strcpy(ptrResiduo->name_1, residuoActual.name_1);
    strcpy(ptrResiduo->name_all, residuoActual.name_all);
    strcpy(ptrResiduo->No, residuoActual.No);
    ptrResiduo->phi = residuoActual.phi;
    ptrResiduo->psi = residuoActual.psi;
    strcpy(ptrResiduo->SS, residuoActual.SS);
    ptrResiduo->startAtomSeqlNo = residuoActual.startAtomSeqlNo;
    ptrResiduo->totalAtomNo = residuoActual.totalAtomNo;
    ptrResiduo->seqlNo = residuoActual.seqlNo;

    return ptrResiduo;
}


Atom * PDBHandler::copiarAtomo (Atom * atm){
    Atom * nuevoAtomo = new Atom();
    /*
     * Atributos del .PDB
    */
    strcpy(nuevoAtomo->No,  atm->No);
    strcpy(nuevoAtomo->name, atm->name);
    strcpy(nuevoAtomo->isomer , atm->isomer);
    strcpy(nuevoAtomo->residueName , atm->residueName);
    strcpy(nuevoAtomo->chainName , atm->chainName);
    strcpy(nuevoAtomo->residueNo , atm->residueNo);
    nuevoAtomo->xyz[0] = atm->xyz[0];
    nuevoAtomo->xyz[1] = atm->xyz[1];
    nuevoAtomo->xyz[2] = atm->xyz[2];
    nuevoAtomo->TFactor = atm->TFactor;

    /*
     * Atributos adicionales
     */
    nuevoAtomo->seqlNo=atm->seqlNo;
    nuevoAtomo->residueSeqlNo=atm->residueSeqlNo;
    nuevoAtomo->chainSeqlNo=atm->chainSeqlNo;
    nuevoAtomo->residue_p=atm->residue_p;
    nuevoAtomo->protein_p=atm->protein_p;
    for(int i=0;i<4;i++) nuevoAtomo->bond[i]=atm->bond[i];
    nuevoAtomo->bondNo=atm->bondNo;
    for(int i=0;i<4;i++) nuevoAtomo->bondCovalence[i]=atm->bondCovalence[i];
    nuevoAtomo->active=atm->active;
    nuevoAtomo->weight=atm->weight;

    return nuevoAtomo;
}

void PDBHandler::filtrarAtomos(const char *ptrchr_nombreAtomo){
    int numAtms = atomos_ptr.size();
    Atom * ptrAtomoTmp = NULL;

    if(atomosFiltrados_ptr.size() > 0){
        limpiarMatriz(DIST);
        limpiarVector(atomosFiltrados_ptr);
    }

    for(int idAtm = 0; idAtm < numAtms; idAtm++){

        ptrAtomoTmp = atomos_ptr[idAtm];
        string nombreAtomo = ptrAtomoTmp->name;
        nombreAtomo = trim(nombreAtomo);

//        if(nombreAtomo.find(ptrchr_nombreAtomo) != string::npos){
        if(nombreAtomo.compare(ptrchr_nombreAtomo) == 0){
            atomosFiltrados_ptr.push_back(copiarAtomo(ptrAtomoTmp));
        }
    }
}

void PDBHandler::leerPDB(const char *ptrchr_pathArchivo){
    Protein proteinaPDB = Protein();

    char * pathArchivo = const_cast<char*>(ptrchr_pathArchivo);

    proteinaPDB.readPDB(pathArchivo);

    this->nombreProteina = string(proteinaPDB.name);

    int idAtm = 0;

    /*
     * Limpio los vectores de atomos y residuos necesarios para los calculos
    */

    if(residuos_ptr.size() != 0){
        limpiarVector(residuos_ptr);
    }
    residuos_ptr = vector<Residue*>();

    if(atomos_ptr.size() != 0){
        limpiarVector(atomos_ptr);
    }
    atomos_ptr = vector<Atom*>();

    for(int i=0; i<proteinaPDB.chain.size(); i++) {

        /*
         * Extraigo los residuos para el calculo de angulos
         */

        int nResiduos = proteinaPDB.chain[i].residue.size();

        for(int idResiduo = 0; idResiduo < nResiduos; idResiduo++){
            Residue residuoActual = proteinaPDB.chain[i].residue[idResiduo];
            residuos_ptr.push_back(copiarResiduo(residuoActual));
        }

        /*
         * Saco los atomos de la proteina y los extraigo a mi vector
        */
        for(int j=0; j<proteinaPDB.chain[i].atom_p.size(); j++) {
            Atom* atm= proteinaPDB.chain[i].atom_p[j];

             atomos_ptr.push_back(copiarAtomo(atm));;

            idAtm++;
        }
    }
}

/*
 * vectorters y Getters
*/
Atom ** PDBHandler::getAtomos(){
    return & atomos_ptr[0];
}

Atom ** PDBHandler::getAtomosFiltrados(){
    return & atomosFiltrados_ptr[0];
}

Residue ** PDBHandler::getResiduos(){
    return & residuos_ptr[0];
}

double ** PDBHandler::getMatriz(int tipoMatriz){
    switch (tipoMatriz){
        case DIST:
            return this->mat_distancias;
            break;
        case ANGS:
            return this->mat_angulos;
            break;
        case AMBAS:
            cout<<"No es posible obtener ambas matrices"<<endl;
            return NULL;
            break;
    }
}
