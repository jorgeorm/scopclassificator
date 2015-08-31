
#ifndef SCOPPFOOT_H
#define SCOPPFOOT_H

#include <string>
#include "modelo/pdbhandler.h"
#include "modelo/matriz.h"

using namespace std;

namespace Control {

/*! \brief Clase que contiene la logica de la GUI de la
 *         herramienta SCOPPFoot
 * \version 0.01
 * \author Jorge Ordoñez Mendez jorge.ordonez@correounivalle.edu.co
*/

    class SCOPPFoot
    {
    public:

      // Constructors/Destructors
      //


      /**
       * \brief Constructor sin parametros
       */
      SCOPPFoot ( );

      /**
       * \brief Destructor sin parametros
       */
      virtual ~SCOPPFoot ( );

      /**
       * \brief Carga un archivo pdb y extrae las matrices de distancia y angulos de una
       *        proteina dada.
       * \param str_pathArchivo URL de archivo PDB a cargar
       */
      void cargarPDB (string str_pathArchivo = "" );

      /** \brief Carga multiples archivos en un array pasado de manera indirecata
       * \param arry_pathsArchivo Nombres de archivos a cargar, permite que se carguen multiples archivos para construir sus matrices de distancias y angulos.
       * \param tamEntrada Numero de archivos a cargar, por defecto toma el valor de 0
       */
      void cargarPDBs (const string * arry_pathsArchivo = NULL, int tamEntrada = 0);

    private:

    /*
     * Metodos privados
    */

      /**
       * \brief Se encarga de inicializar los componentes de esta clase
        */
      void initAttributes ( ) ;

    /*
     * Atributos privados
    */

    PDBHandler * ptr_pdbHandler; /// < Puntero al manejador de archivos PDB.
    vector <Matriz*> conjuntoSubmatrices; /// < Vector de submatrices de todas las proteinas cargadas


    }; // fin de la clase

}; // fin del paquete o espacio nombrado

#endif // SCOPPFOOT_H
