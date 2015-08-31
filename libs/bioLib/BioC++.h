#ifndef BIOC++_H
#define BIOC++_H
/* ============================================================================



		######                   #####
		#     #     #     ####  #     #    #       #
		#     #     #    #    # #          #       #
		######      #    #    # #        #####   #####
		#     #     #    #    # #          #       #
		#     #     #    #    # #     #    #       #
		######      #     ####   #####




  	   A C++ LIBRARY FOR STRUCTURE BIOINFORMATICS APPLICATIONS


                         Copyright Dr. Hongyu Zhang
                  Email: http://hongyu.org/html/email.html


                                July 2000

                               VERSION 0.2




The contents of this library are arranged in the following order:

. DESCRIPTION OF THE lIBRARY

. EXAMPLE

. DATA STRUCTURE

. THE SOURCE CODE


				DESCRIPTION
				-----------

 We heard of BioPerl, BioJava, why no BioC++ ? Now here it is ! 

 During my research work on structure bioinformatics and protein folding,
 I felt the need for a C++ library that can handle a lot of daily repeated
 jobs, so I don't need to rewrite my code each time. Having not seen
 any good one on internet, I think this library might be useful for the
 structure bioinformatics community. Currently, it mainly focuses on
 structure processing, like PDB format protein coordinates input, output,
 bond generation, homologous family building, secondary and tertiary
 structure information generation, RMSD calculation etc.. It also have
 a limited number of modules for DNA and protein sequence processing. 
 In future, I expect it will include more sequence processing modules. 

 Including of the latest development in C++ language like Standard Template
 Library (STL) made this package small in size. The program is still in
 alpha status, as I am adding more functions to it from time to time.
 If you have any interest, you are welcome to make it a more useful protocol
 for communities.  Although commercial vendors are under high presure to fully
 support STL, some of them, like SGI compiler, still has annoying problems with
 string processing. Therefore, I compiled this library using the GNU g++. The
 package has been tested on SGI IRIX and Linux PC.

 Basically, the whole data structure of this library consists of two major
 classes, Protein and DNA. Main focus is on Protein, which used to be my
 major working field. The modules related to Protein, from top to bottom,
 are: Protein, Chain, Residue and Atom. You can tell from the name which
 biological layers they represent. Similarly, DNA related modules include 
 DNA, DNAchain.

 The main class is called class Protein. Currently, this module has the
 following member functions:

 1. Read protein structure in PDB format(class constructor)

	Protein.readPDB(char *file);

 2. Output protein structure in PDB format

	Protein.writePDB(char *file);
        or
  	Protein.writePDB(char *file, char *chainName, int order);
    where
        order = -0: original order
              = -1: atom number starting from 1
              = -2: residue number starting from 1
              = -3: both

 3. Bond generation

	Protein.bondGeneration(int bFlag=0);

    where  i=0: distance-based; i=1: topology-based (not available yet)

 4. Root Mean Square Deviation between two proteins (coding not finished yet)

 	Protein.rmsd(Protein p2, char* flag, int beginResidue, int endResidue); 

    where
	
	flag = BACKBONE, Main chain RMSD (including N, CA, C, O)
	     = CA,	CA RMSD
	     = All,	All atom RMSD
	     = HEAVY,	Heavy atom RMSD (no hydrogens)

	p2: the protein to which this protein is being compared.

	beginResidue, endResidue: the begin and end residue positions of the
			segment we calculated RMSD for.

 5. Output Fasta format amino acid sequence

	Protein.writeFasta(const char* file);

 6. Hydrophobicity profile output

	Protein.writeHydrophobicity(char *file);

 7. DSSP format input, generating secondary and tertiary structure information

	Protein.readDSSP(char *file);

 8. HSSP format input, generating homologous familiy information

	Protein.readHSSP(char *file);


 Class Fragment is another important class, which can be used to operate on
 protein segments. I will add more informatin about it in future in this 
 document. For now, please study the source code section below for more details
 of its member functions.


				EXAMPLE
				-------

 It's very simple to use this module. You just need to put the following line
 before your main() function, 

 #include "PATH/BioC++.h"

 where PATH is the path name you store this libaray file. If the path is your
 present directory, or already included in your default include path, like
 /usr/local/include, then you can omit the path name PATH/.

 In your main() function, you can use all the member functions defined in
 this library to operate on the structures. Before using them, you are
 supposed to read the description of the format of these functions (see 
 Description above, and refer to source code if you can). 
 
 A simple example program is shown below.


 #include "BioC++.h"
 using namespace std;

 main()
 {

	Protein p;			// construct a new Protein object

	p.readPDB("4pti.pdb");		// read PDB file

	p.writePDB("output.pdb");		// output coordinates

	p.writeFasta("output.msf");		// output sequence

	return 0;

 }


			DATA STRUCTURE
			--------------


 The C++ codes in this library were written in the following order:

 Section 1.	Declarition of Global library, varable, constant and functions 

         1.1. 	Standard libraries
         1.2.	Constants 
         1.3.	Global variables
	 1.4.	Global functions

 Section 2.  	Definition of biological classes: Atom, Residue, Segment, 
		Chain and Protein

         2.1.	Class framework 
         2.2.	Member functions

============================================================================ */



//			THE SOURCE CODE
//			---------------



// Section 1.	Declarition of Global library, varable, constant and functions 



// Section 1.1. 	Standard libraries

#include <stdlib.h>	// standard C library function
#include <cmath>	// math
#include <fstream>	// file i/o
#include <cstring> // manejo de strings y copias
#include <iostream>	// i/o stream
#include <strstream>	// char *stream
#include <iomanip>	// i/o format
#include <string>  	// Standard string library
#include <map>   	// STL associative container
#include <limits.h>   	// Noeric limit
#include <algorithm> 	// STL algorithm
#include <vector>    	// STL vector
#include <functional>	// STL functional
#include <cmath>

// Section 1.2.	Constants 
 
#define MAX(a,b) a>b? a:b
#define MIN(a,b) a>b? b:a
#define CONV (180./acos(-1.))
#define Rvdw 1.8	// maximum van der Waals radius
#define Dvdw 3.6	// maximum van der Waals overlaping distance
#define MAX_HOMOLOGY 100 // maximum homologous sequence number

using namespace std;

// Section 1.3.	Global variables

class AAProperty	// class variable to define the properties of 
			// amino acids
{
public:

        char name[5];
        char name_1[2];
        char name_all[15];
        bool hydrophobicity;

        AAProperty()
	{
		strcpy(name, "UNK ");
		strcpy(name_1, "X");
		strcpy(name_all,"unknown residue");
                hydrophobicity = 0;
	}

        AAProperty(char* t, char* t_1, char* t_all, bool h)
        {
                strcpy(name, t);
                strcpy(name_1, t_1);
                strcpy(name_all, t_all);
                hydrophobicity = h;
        }

};


/*

Section 2.  Definition of biological classes

Section 2.1.	Framework of the classes 

*/

// Here are all the classes


// handling all data and operations on protein molecules
class Protein;

// handling a single protein chain
class Chain;

// handling a segment in the chain
class Segment;

// handling an amino acid
class Residue;

// handling an atom
class Atom;

// protein fragment
class Fragment;

// protein cofactor
class Cofactor;

// cofactor atom
class Hetatm;

// handling DNA molecules data
class DNA; 

// similar to class Chain, but handling a single DNA chain
class DNAchain;


// The detailed definitions of the above classes start from here


class Atom
{
public:

/*
PDB related variables

An example of a PDB atom coordinate line

ATOM      1  N   ARG     1      26.465  27.452  -2.490  1.00 25.18      4PTI  89
      < | >< | >|< |>|< | ><             |           >      <  | ><       |    >
        No   |	|  | |  |               xyz                TFactor       extra
           name |  | |  |
             isomer| |  |
                   | | residueNo
         residueName |
                   chainName
*/
  char No[6], name[6], isomer[2], residueName[5], chainName[2], residueNo[6];
  char extra[15];
  float xyz[3], TFactor;
  int	seqlNo, residueSeqlNo, chainSeqlNo; // sequential numbers


  // molecular topology related variables
  Residue *residue_p;
  Chain *chain_p;
  Protein *protein_p;
  Atom	*bond[4]; // connectivity
  short int bondNo, bondCovalence[4]; 

  // other variables
  bool active;  // status of this atom
  float weight; // weight status, used in e.g. RMSD calculation


  // member functions

  Atom(); // constructor

  // In order to keep the OO flavor, we build
  // setParm_ functions to handle all the attributes' setup
  void setParm_No(const char *str) { strcpy(No, str); }
  void setParm_Weight(const float w) { weight=w; }
  void setParm_name(const char *str) { strcpy(name, str); }
  void setParm_isomer(const char *str) { strcpy(isomer, str); }
  void setParm_residueName(const char *str) { strcpy(residueName, str); }
  void setParm_chainName(const char *str) { strcpy(chainName, str); }
  void setParm_residueNo(const char *str) { strcpy(residueNo, str); }
  void setParm_extra(const char *str) { strcpy(extra, str); }
  void setParm_xyz(const float x, const float y, const float z) 
  { xyz[0]=x; xyz[1]=y; xyz[2]=z; }
  void setParm_TFactor(const float t) { TFactor=t; }
  void setParm_seqlNo(const int t) { seqlNo=t; }
  void setParm_residueSeqlNo(const int t) { residueSeqlNo=t; }
  void setParm_chainSeqlNo(const int t) { chainSeqlNo=t; }
  void setParm_residue_p( Residue *pt) { residue_p=pt; }
  void setParm_chain_p( Chain *pt) { chain_p=pt; }
  void setParm_active(const bool b) { active=b; }


  // other functions
  friend ofstream &operator<<(ofstream &ostrm, Atom &atom); //output coordinates
  Atom operator=(const Atom &pp);	//copy an atom object
  void link(Atom &pp);		//set a bond to Atom pp
  float operator-(const Atom &pp);	//distance from Atom pp
  float distanceSquare(const Atom &pp);	//square distance from Atom pp
  float vdw();	// van der Waals radius
  // Standard bondlength upper limit
  friend float bondLengthLimit(const Atom &aa, const Atom &bb); 
  Atom operator<=(const Atom &bb); //vector from bb to this
  Atom operator+(const Atom &bb);  //vectors sum
  Atom operator*(const Atom atm);   //vector product
  float distanceSquare(Atom pp);    //atomic distance square
//  float angle(const Atom &aa, const Atom &bb, const Atom &cc); //angle <abc
};

class Hetatm
{
public:	
  char record[81];

  //constructor
  inline Hetatm(){};

  // set up member
  void setParm_record(const char *str) { strcpy(record, str); }

};

class Cofactor
{
public:
  vector<Hetatm> hetatm;

  //member functions
  void addAtom(Hetatm ht) { hetatm.push_back(ht); }
  void clear() { hetatm.clear(); }
};

class Residue
{

public:

  // general information
  char name[5], name_1[2], name_all[15];  // 3 naming styles
  bool hydrophobicity;	// 1 is hydrophobic, 0 is non-hydrophobic
  char SS[2];  //secondary structure
  float phi, psi;  // main chain torsion angle
  char homology[MAX_HOMOLOGY]; 	// used for HSSP homologous family

  // PDB related information
  char  No[6];
  char	chainName[2];

  // molecular topology information
  int seqlNo, chainSeqlNo;
  vector<Atom> atom; // a vector containing the Atoms
  map<string, Atom *> mapAtom; // map an atom name to its Atom object
  int startAtomSeqlNo, endAtomSeqlNo, totalAtomNo;

  // other variables
  bool active; // status of this residue


  // member functions  

  Residue(); //constructor

  // parameters setup
  void setParm_No(char *str) { strcpy(No, str); }
  void setParm_chainName(char *str) { strcpy(chainName, str); }
  void setParm_seqlNo(int i) { seqlNo=i; }
  void setParm_chainSeqlNo(int i) { chainSeqlNo=i; }
  void setParm_name(char *str) { strcpy(name, str); }
  void setParm_name_1(char *str) { strcpy(name_1, str); }
  void setParm_name_all(char *str) { strcpy(name_all, str); }
  void setParm_homology(char *str) { strcpy(homology, str); }
  void setParm_SS(char *str) { strcpy(SS, str); }
  void setParm_phi(float f) { phi=f; }
  void setParm_psi(float f) { psi=f; }
  void setParm_hydrophobicity(bool b) { hydrophobicity=b; }
  void setParm_startAtomSeqlNo(int i) { startAtomSeqlNo=i; }
  void setParm_endAtomSeqlNo(int i) { endAtomSeqlNo=i; }
  void setParm_totalAtomNo(int i) { totalAtomNo=i; }
  void setParm_mapAtom(char *str, Atom *at) { mapAtom[str]=at; }
  void setParm_active(const bool b) { active=b; }

  void setParm(); // set parameters by copying from its member atoms

  // output all of the atom record of this residue
  friend ofstream &operator<<(ofstream &ostrm, Residue residue);  

  bool connection(Residue &r2);
  void addAtom(Atom atm);
  void clear();
  
};

class Chain
{

public:
  // general information	
  char  name[2];

  // molecular topology information
  int	seqlNo;
  int	startAtomSeqlNo, endAtomSeqlNo, totalAtomNo;
  int	startResidueSeqlNo, endResidueSeqlNo, totalResidueNo;
  vector<Residue> residue;  // a vector containing all residues
  vector<Atom *> atom_p;
  map<string, Residue *> mapResidue; // map a residue's PDB No to its object

  // other variables
  bool active; // status of this chain

// functions

  Chain(); // constructor

  // parameters setup functions
  void setParm();
  void setParm_name(char *str) { strcpy(name, str); }
  void setParm_seqlNo(int i) { seqlNo=i; }
  void setParm_startAtomSeqlNo(int i) { startAtomSeqlNo=i; }
  void setParm_endAtomSeqlNo(int i) { endAtomSeqlNo=i; }
  void setParm_totalAtomNo(int i) { totalAtomNo=i; }
  void setParm_startResidueSeqlNo(int i) { startResidueSeqlNo=i; }
  void setParm_endResidueSeqlNo(int i) { endResidueSeqlNo=i; }
  void setParm_totalResidueNo(int i) { totalResidueNo=i; }
  void setParm_mapResidue(char *str, Residue *r) { mapResidue[str]=r; }
  void setParm_active(const bool b) { active=b; }

  // other functions
  friend ofstream &operator<<(ofstream &ostrm, Chain chain);
  void addResidue(Residue res);
  void clear();

};

class Protein
{

public:

  // basic varibles
  char name[80];
  vector<Chain> chain;
  Cofactor cofactor;

  // pointers
  vector<Residue *> residue_p;
  vector<Atom *> atom_p;
  map<string, Chain *> mapChain; // map a chain name to its object

  // other varables
  bool bondAvailable;	// bond information available or not
  int homologyNo;	// homologous sequence number


  Protein();			//constructor

  void clear();	// clear the vectors stored for this Protein object

  void readPDB(char *file); //read coordinates

  void writePDB(char *file);	

  void writePDB(char *file, char *chainName, int order);
        // output to a PDB file
	//	order=0: original order
	// 	order=1: reorder atom number, starting from 1 
	// 	order=2: reorder residue number, starting from 1 
	// 	order=3: reorder both

  void setActive(char *str); // set the active atoms or residues

  void bondGeneration(int bFlag=0);	
	// generate bond. i=0: topology-based; i=1: distance-based

  void writeFasta(const char* fastaFile);
	// generate protein sequence in MSF format

  void writeHydrophobicity(char *file);	//output the hydrophobic profile 

//  float rmsd(Protein p2, char* flag, int beginResidue, int endResidue); 
	// root mean square deviation between two proteins

  void readDSSP(char *dsspFile);

  void readHSSP(char *hsspFile);

  void setParm_mapChain(char *str, Chain *c) { mapChain[str]=c; }

  void setParm_homologyNo(int i) { homologyNo=i; }

  void addChain(Chain ch) { chain.push_back(ch); }

};

class DNAchain
{
public:
  string id;
  string description;
  string seq;

  DNAchain(); // constructor
  void clear(); // clear the content of object
  void cleanSeq(); // clean the sequence, remove non "AGCT" charactors
  void set_id(string input_id);
  void set_description(string input_description);
  void set_seq(string input_seq);
  DNAchain reverse_compliment(); // return the reverse compliment
  DNAchain assemble(DNAchain c1, DNAchain c2, int minmatch);
  int length();
};

class DNA
{
public:
  vector<DNAchain> chain;

  DNA(); // constructor
  void readFasta(char *file); // read Fasta format sequence
  void writeFasta(char *file); // write Fasta format sequence
  void addChain(DNAchain dc); // add a new chain member

};
#endif

// End of the BioC++ library
