#ifndef PDBHANDLER_H
#define PDBHANDLER_H

#include "libs/bioLib/BioC++.h"
#include <vector>
#include "modelo/matriz.h"



/**
 * \brief Realiza trim de la parte izquierda de un string.
 * Limpia la part izquierda del string pasado por referencia.
 * \param s <string> string de c++ que entra por referencia para ser modificada.
 * \return Puntero al string modificado
*/
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}


/**
 * \brief Realiza trim de la parte derecha de un string.
 * Limpia la parte derecha del string pasado por referencia.
 * \param s <string> string de c++ que entra por referencia para ser modificada.
 * \return Puntero al string modificado
*/
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}


/**
 * \brief Realiza trim de un string.
 * Limpia la parte derecha e izquierda del string pasado por referencia removiendo espacios.
 * \param s <string> string de c++ que entra por referencia para ser modificada.
 * \return Puntero al string modificado
*/
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


class PDBHandler
{
/*
 * Constantes publicas
*/
public:

    enum TIPOMATRIZ {
        ANGS,
        DIST,
        AMBAS
    };


/*
 * Metodos publicos
 */
public:

    /**
     * \brief Constructor sin parametros
     * Crea una instancia de PDBHandler, inicializa los atributos de la clase
    */
    PDBHandler();

    /**
     * \brief Destructor sin parametros
     * Limpia los atributos de la clase que han sido reservados con memoria dinamica
     */
    virtual ~PDBHandler();

    /**
     * \brief Lee un archivo PDB y extrae la informacion necesaria para el funcionamiento de la aplicacion
     * \param ptrchr_pathArchivo <const char *> Path completo del archivo .PDB a leer
     */
    void leerPDB(const char * ptrchr_pathArchivo);

    /** \brief Extrae de los atomos aquellos que tengan un nombre dado
     * Extrae al atributo ptratm_atomosFiltrados
     * \param ptrchr_nombreAtomo <const char *> nombre del atomo a extraer
     */
    void filtrarAtomos(const char * ptrchr_nombreAtomo);

    /**
     * \brief Calcula las matrices de angulos y distancias
     * Realiza el calculo de la o las matrices indicadas a travez del parametro
     * \param tipoMatriz indica si la matriz a calcular es de angulos o distancia o ambas
    */
    void mapearaMatriz(int tipoMatriz = AMBAS, double maxTol = 20.0);

    /**
     * \brief Escribe matrices de angulos y distancias
     * Escribe a un archivo formateado por ";" con el formato (NomProtTipoMat.csv)
     * \param tipoMatriz indica si es angulos, distancia o ambas dependiendo de constantes definidas
     * \param cptr_archivo Nombre para el archivo de salida donde sera escrita la matriz
     * \todo Generalizar metodo permitiendo que la matriz pase como parametro de entrada.
    */
    void escribirMatriz(int tipoMatriz = AMBAS, const char * cptr_pathArchivo = NULL);

//    /**
//     * \brief Toma un conjunto de submatrices de un tamaño especificado de la matriz especificada
//     * Selecciona un conjunto de submatrices que pueden o no estar superlapadas de un tamaño dado,
//     * se debe especificar a que matriz se le desea extraer el subconjunto.
//     * \param matriz <[double][double] &>Matriz pasada como referencia a partir de la cual se generaran las submatrices.
//     * \param tamano <int> Tamaño que tendra cada una de las submatrices
//     * \param overlap <boolean> indica si se permitira sobrelapamiento de las matrices a la hora de generarlas
//     * \return vector <FeaturesDataset*> Puntero de conjunto de submatrices obtenidas
//     */
//    FeaturesDataset * obtenerSubmatrices(double ** matriz, int tamSubMat, int tamMat, bool overlap = true);


    /**
     * @brief getNumResiduos
     * @return int Número de residuos que la proteína contiene
     */
    int getNumResiduos();
//  Metodos protegidos
protected:
    /**
     * \brief Escribe la matriz de distancias interatomicas calculada
     * Toma la matriz de distancias y la escribe a un archivo CSV separado por ";"
     * con floats de tres decimales nombrandolo como proteinaDist.csv
     * \param cptr_pathArchivo Path como const char * a donde se guardara el archivo
    */
    void escribirDistancias(const char* cptr_pathArchivo);

    /**
     * \brief Escribe la matriz de angulos entre residuos
     * Toma la matriz de angulos y la escribe a un archivo CSV separado por comas
     * con tres decimales de precision nombrandolos como proteinaAng.csv
     * \param cptr_pathArchivo Path como const char * a donde se guardara el archivo
    */
    void escribirAngulos(const char* cptr_pathArchivo);

    /**
     * \brief Incializa las matrices de angulos y distancias
     * Inicializa a ceros cada uno de las celdas de la matriz de angulos y distancias segun sea requerido.
     * \param tipoMatriz indica si es angulos, distancia o ambas dependiendo de constantes definidas
    */
    void initMatriz(int tipoMatriz = AMBAS);

    /**
     * \brief Borra las matrices de angulos y distancias
     * Borra los punteros de las matrices y los aterriza a NULL.
     * \param tipoMatriz indica si es angulos, distancia o ambas dependiendo de constantes definidas
    */
    void limpiarMatriz(int tipoMatriz = AMBAS);

    /**
     * \brief Construye la matriz de distancias
     * Construye la matriz de distancias entre atomos cargados previamente de un PDB
     * \param maxTolera Maximo de distancia permitido para hacer enfasis en los m´as cercanos, por defecto tiene un valor de 20 Amstrongs;
     * \param <std::string> nomAtomoRef Atomo de referencia con el cual se calcularán las distancias entre residuos.
    */
    void calcularMatrizDistancias(double maxTolera = 20, string nomAtomoRef = "CA");

    /**
     * \brief Construye la matriz de angulos
     * Construye la matriz de angulos entre los atomos cargados previamente de un PDB
    */
    void calcularMatrizAngulos();

    /**
     * \brief Limpia la matriz de distancias y angulos
     * Limpia cada una de las matrices de distancias y angulos
     *
    */
    void limpiarMatrices();

    /**
     * \brief Busca en un residuo un atomo determinado
     * Recibe el nombre de un atomo (CA, C, N, ... nombre puesto en PDB) y lo busca en el residuo dado.
     * \param nomAtomo <const char *> Nombre del atomo a buscar
     * \param residuo <Residue *> Puntero a residuo en el cual se desea buscar el atomo
     */
    Atom * extraerAtomo(const char * nomAtomo, Residue * residuo);

    /* Setters y Getters
    */
public:

    Atom ** getAtomos();

    Residue ** getResiduos();

    Atom ** getAtomosFiltrados();

    double ** getMatriz(int tipoMatriz = AMBAS);

    int getTamMatriz(int tipoMatriz = DIST);

    /*
     * Metodos de uso interno
    */
private:
    /**
     * \brief Recorre el vector de objetos pasado como parametro.
     * Recorre el vector de objetos pasado como parametro de referencia y libera la memoria ocupada por los objetos que lo componen.
     * \param vectorAlimpiar <vector <*>> Referencia;
     */
    template <typename T> /// < Permite funcion generica de limpiar vectores
    inline void limpiarVector(vector <T> & vectorAlimpiar){
        int numAtms = vectorAlimpiar.size();
        for(int idAtm = 0; idAtm < numAtms; ++idAtm){
            delete vectorAlimpiar[idAtm];
        }
        vectorAlimpiar.clear();
    }

    /**
     * \brief Calcula el vector normal generado por un residuo segun el metodo especificado en el paper.
     * Recibe un puntero y obtiene el vector normal generado por los atomos N-CA-C del residuo.
     * \param ptr_residuo <Residue *> puntero a residuo a partir del cual se calculara el vector normal.
     * \return <double *> Puntero de doubles de tamaño 3 correspondiente a las coordenadas x, y, z del vector.
     */
    double * calcularVecNormal(Residue * ptr_residuo);

    /**
     * \brief Calcula el angulo Tetha entre dos residuos
     * \param residuo1 <Residue *> Puntero al primer residuo;
     * \param residuo2 <Residue *> Puntero al segundo residuo;
     * \return <double> Angulo Tetha etre los dos residuos (0-PI) radianes
     */
    double calcularAngulo(Residue * residuo1, Residue * residuo2);


    /**
     * \brief Genera un puntero a una copia del atomo pasado como puntero.
     * Toma un atomo como puntero y genera una copia exacta de los atributos del atomo en un puntero que ocupa
     * memoria dinamica.
     * \param atm <Atom *> Puntero al atomo que va a ser copiado
     * \return <Atom *> Puntero a atomo copia reservado con memoria dinamica
    */
    Atom * copiarAtomo (Atom * atm);

    /**
     * \brief Genera un puntero a una copia del residuo pasado como valor.
     * Toma un residuo como valor y genera una copia exacta de los atributos del residuo en un puntero que ocupa
     * memoria reservada dinamicamente.
     * \param residuoActual <Residue> Residuo pasado como valor que sera copiado.
     * \return <Residue *> Puntero a residuo copia reservado con memoria dinamica.
     */
    Residue * copiarResiduo (Residue residuoActual);

//    /**
//     * \brief Genera un vector de submatrices a apartir de la matriz original
//     * Toma la matriz original y genera un conjunto de submatrices de tamaño especificado permitiendo
//     * superposicion entre ellas en la matriz original.
//     * \param matrizOrig <double **> Arreglo de dos dimensiones en puntero.
//     * \param tamMat <int> Tamaño de la matriz original
//     * \param tamSubMat <int> Tamaño que tendra cada una de las submatrices.
//     * \return vector <Matriz*> Puntero a conjunto de submatrices obtenidas
//     */
//    FeaturesDataset * submatricesOverlap(double ** matrizOrig, int tamMat, int tamSubMat);

    /**
     * \brief Genera un vector de submatrices a apartir de la matriz original
     * Toma la matriz original y genera un conjunto de submatrices de tamaño especificado no permite
     * superposicion entre ellas en la matriz original.
     * \param matrizOrig <double **> Arreglo de dos dimensiones en puntero.
     * \param tamMat <int> Tamaño de la matriz original
     * \param tamSubMat <int> Tamaño que tendra cada una de las submatrices.
     * \return vector <double **> & Referencia a conjunto de submatrices obtenidas
     */
    vector <Matriz *> * submatricesSimple(double ** matrizOrig, int tamMat, int tamSubMat);
    string nombreProteina; /// < <cstring> Retiene temporalmente el nombre de la proteina con la que se esta trabajando


/*
 * Atributos protegidos
 */
protected:
    vector <Atom *> atomos_ptr; /// < Array de atomos leidos del .PDB
    vector <Residue *> residuos_ptr; /// < Residuos leidos del .PDB
    vector <Atom *> atomosFiltrados_ptr; /// < Array de atomos filtrados por el metodo filtrarAtomos
    double ** mat_distancias; /// < Matriz de doubles que almacena las distancias interatomicas Cai-Caj
    double ** mat_angulos; /// < Matriz de double sque almacena los angulos entre los residuos Ri-Rj
    int i_tamDistancias, i_tamAngulos; /// < Tamaños de las matrices de distancias y atomos, se almacenan para referencia

};

#endif // PDBHANDLER_H
