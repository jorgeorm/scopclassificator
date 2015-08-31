#include "BioC++.h"
#include "modelo/pdbhandler.h"
/*
 Para separarlo a ver si funciona
*/


// Section 1.4. Global functions

int lowercase(char *s) {
  while(*s) {
    *s=tolower(*s);
    s++;
  }
  return 0;
}

int chopSpace(char *s){
  for(;*s;s++){
    if(*s==' ') {
        char *s2=s;
        for(;*s2;s2++) {
            *(s2)=*(s2+1);
        }
    }
  }
  return 0;
}



// definition of 20 amino acid objects
//      3_char_name     1_char_name, full_name,       hydrophobicity
AAProperty cys( "CYS ",     "C",         "Cysteine",      1 );
AAProperty phe( "PHE ",     "F",         "Phenylalanine", 1 );
AAProperty ile( "ILE ",     "I",         "Isoleucine",    1 );
AAProperty leu( "LEU ",     "L",         "Leucine",       1 );
AAProperty met( "MET ",     "M",         "Methionine",    1 );
AAProperty val( "VAL ",     "V",         "Valine",        1 );
AAProperty trp( "TRP ",     "W",         "Tryptophan",    1 );
AAProperty gly( "GLY ",     "G",         "Glycine",       0 );
AAProperty ala( "ALA ",     "A",         "Alanine",       0 );
AAProperty pro( "PRO ",     "P",         "Proline",       0 );
AAProperty ser( "SER ",     "S",         "Serine",        0 );
AAProperty thr( "THR ",     "T",         "Threonine",     0 );
AAProperty asn( "ASN ",     "N",         "Asparagine",    0 );
AAProperty gln( "GLN ",     "Q",         "Glutamine",     0 );
AAProperty asp( "ASP ",     "D",         "Aspartic acid", 0 );
AAProperty glu( "GLU ",     "E",         "Glutamic acid", 0 );
AAProperty tyr( "TYR ",     "Y",         "Tyrosine",      0 );
AAProperty lys( "LYS ",     "K",         "Lysine",        0 );
AAProperty his( "HIS ",     "H",         "Histidine",     0 );
AAProperty arg( "ARG ",     "R",         "Arginine",      0 );



// map a given residue name (string) to its AAProperty
map<string, AAProperty> mapAAProperty;

// amino acid sequence, used to generate the HSSP profile

//char AAPropertySeqlNo[20][2] = {"A","C","D","E","F","G","H","I","K","L","M","N","P","Q","R","S","T","V","W","Y"};

//char AAPropertySeqlNo[20][4] = {"ALA","CYS","ASP","GLU","PHE","GLY","HIS","ILE","LYS","LEU","MET","ASN","PRO","GLN","ARG","SER","THR","VAL","TRP","TYR"};



// Section 2.2.	Member function of the classes

// Atom functions

Atom::Atom()
{
    bondNo = 0;
    for(int i=0;i<4;i++) {
        bond[i]=NULL;
        bondCovalence[i]=0;
    }
    weight = 1.0;
    active=true;
}

ofstream &operator<<(ofstream &ostrm, Atom &atom)  //write the coordinates
{
        ostrm << "ATOM  "  << atom.No << atom.name;
        ostrm << atom.residueName << atom.chainName << atom.residueNo;
        ostrm << "    ";
        ostrm.setf(ios::showpoint|ios::fixed);
        ostrm<<setprecision(3)<<setw(8)<<atom.xyz[0];
        ostrm<<setprecision(3)<<setw(8)<<atom.xyz[1];
        ostrm<<setprecision(3)<<setw(8)<<atom.xyz[2];
        ostrm<<endl;
    return ostrm;
}

Atom Atom::operator=(const Atom &pp) //copy an atom object
{
    int i;
    strcpy(No, pp.No);
    strcpy(name, pp.name);
    strcpy(isomer, pp.isomer);
    strcpy(residueName, pp.residueName);
    strcpy(chainName, pp.chainName);
    strcpy(residueNo, pp.residueNo);
    for(i=0;i<3;i++) xyz[i]=pp.xyz[i];
    TFactor=pp.TFactor;
    seqlNo=pp.seqlNo;
    residueSeqlNo=pp.residueSeqlNo;
    chainSeqlNo=pp.chainSeqlNo;
    residue_p=pp.residue_p;
    protein_p=pp.protein_p;
    for(i=0;i<4;i++) bond[i]=pp.bond[i];
    bondNo=pp.bondNo;
    for(i=0;i<4;i++) bondCovalence[i]=pp.bondCovalence[i];
    active=pp.active;
    weight=pp.weight;
}

void Atom::link(Atom &pp) // set a bond pointer to Atom pp
{
    bond[bondNo++]=&pp;
    if(bondNo>4) {
       cerr<<"Error: more than 4 bonds for atom "<< \
        this->No<<endl;
       cerr << "it's neighbors are: " << bond[0]->No << " " <<
         bond[1]->No << " " << bond[2]->No << " " << bond[3]->No;
       cerr <<endl;
       exit(1);
    }
}

float Atom::operator-(const Atom &pp)    //atomic distance
{
    return  sqrt(
        (xyz[0]-pp.xyz[0])*(xyz[0]-pp.xyz[0])+
        (xyz[1]-pp.xyz[1])*(xyz[1]-pp.xyz[1])+
                (xyz[2]-pp.xyz[2])*(xyz[2]-pp.xyz[2]));
}

 Atom Atom::operator<=(const Atom &bb) //vector from bb to this
{
  Atom tmp=*this;
  for(int i=0;i<3;i++) tmp.xyz[i]=xyz[i]-bb.xyz[i];
  return tmp;
}

 Atom Atom::operator+(const Atom &bb)  //vectors sum
{
  Atom tmp;
  for(int i=0;i<3;i++) tmp.xyz[i]=this->xyz[i]+bb.xyz[i];
  return tmp;
}

/*
float angle(const Atom &aa, const Atom &bb, const Atom &cc) //angle <abc
{
  Atom a,b;
  a=aa<=bb;
  b=cc<=bb;
  return (acosf((a.xyz[0]*b.xyz[0]+a.xyz[1]*b.xyz[1]+a.xyz[2]*b.xyz[2])/
                sqrtf(a.xyz[0]*a.xyz[0]+a.xyz[1]*a.xyz[1]+a.xyz[2]*a.xyz[2])/
                sqrtf(b.xyz[0]*b.xyz[0]+b.xyz[1]*b.xyz[1]+b.xyz[2]*b.xyz[2])));
}
*/

 Atom Atom::operator*(Atom atm)   //vector product
{
  Atom tmp;
  tmp.xyz[0]=xyz[1]*atm.xyz[2]-xyz[2]*atm.xyz[1];
  tmp.xyz[1]=-xyz[0]*atm.xyz[2]+xyz[2]*atm.xyz[0];
  tmp.xyz[2]=xyz[0]*atm.xyz[1]-xyz[1]*atm.xyz[0];
  return tmp;
}

 float Atom::distanceSquare(Atom pp)    //atomic distance square
{
    return
        (xyz[0]-pp.xyz[0])*(xyz[0]-pp.xyz[0])+
        (xyz[1]-pp.xyz[1])*(xyz[1]-pp.xyz[1])+
                (xyz[2]-pp.xyz[2])*(xyz[2]-pp.xyz[2]);
}

 float Atom::vdw()	//
{
    switch(name[2]) {
           case 'C':
                return 1.70;
        break;
           case 'N':
                return 1.55;
        break;
           case 'O':
                return 1.52;
        break;
           default:
                return 1.80;
        break;
     }
}

 float bondLengthLimit(Atom &aa, Atom &bb) //standard bond length upper limit
{
      return aa.name[2]=='S'||bb.name[2]=='S'? 1.9:1.7;
}



// ------- Residue functions


 Residue::Residue()
{
    seqlNo = chainSeqlNo = 0;
    startAtomSeqlNo = endAtomSeqlNo =0;
    active=true;
}

 void Residue::setParm()
// set parameters
{
  if(atom.size()==0) exit(0);
  int i;

  setParm_No( atom[0].residueNo );
  setParm_chainName( atom[0].chainName );
  setParm_seqlNo( atom[0].residueSeqlNo );
  setParm_chainSeqlNo( atom[0].chainSeqlNo );
  setParm_name( atom[0].residueName );

  setParm_name_1(mapAAProperty[name].name_1);
  setParm_name_all(mapAAProperty[name].name_all);
  setParm_hydrophobicity(mapAAProperty[name].hydrophobicity);
}

 bool Residue::connection(Residue &r2)
{
  bool tmp;

  if(this->mapAtom.count("  C  ") && r2.mapAtom.count("  N  ")) {
        Atom *aC = this->mapAtom["  C  "];
        Atom *aN = r2.mapAtom["  N  "];
        if( *aC - *aN <= bondLengthLimit(*aC, *aN) ) tmp = true;
    else tmp = false;
  }
  else
  {
    throw "The two residues for connection check don't have C or N atoms";
  }

  return tmp;
}

 void Residue::addAtom(Atom atm)
{
    atom.push_back(atm);
}

 void Residue::clear()
{
    atom.clear();
}


 ofstream &operator<<(ofstream &ostrm, Residue residue)
// output all of the atom record of this residue
{

    for(int i=0; i<residue.atom.size(); i++) ostrm << residue.atom[i];
    return ostrm;
}


/*
// ------- fragment functions

fragment::fragment(Protein &p, int start, int end)
{
    int i,j;

    // copy general information
    for(i=start; i<=end; i++) {
        residue.push_back(p.residue[i]);
        for(j=0; j<p.residue[i].atom_p.size(); j++)
            atom.push_back(*(p.residue[i].atom_p[j]));
    }

    // correct topology information
    int k=0;
    for(i=0; i<residue.size(); i++) {
        int num=residue[i].atom_p.size();
        residue[i].atom_p.clear();
        for(j=0; j<num; j++) {
            residue[i].add_atom_p(&atom[k]);
            residue[j].setParm_mapAtom(atom[k].name, &atom[k]);
            k++;
        }
    }
}

float fragment::distanceMatrixError( fragment f2 )
{
    int i,j;
    float rms;

    for(i=0; i<atom.size()-1; i++) {
    for(j=i+1; j<atom.size(); j++) {
        rms+= fabs(atom[i].distanceSquare(atom[j]) -
              f2.atom[i].distanceSquare(f2.atom[j]));
    }
    }

    int n = atom.size();

    return sqrt(rms/(n*(n-1)/2));
}
*/

 void Protein::setActive( char *str )
{

/* the naming gramar is from QUANTA

ALL         All atoms
    e.g ALL
NONH        All non-hydrogen atoms
    e.g NONH
PICK        Atoms with matching atom name
    e.g PICK CA N?
ATOM        Atoms with matching name,residue name and ID
    e.g ATOM C? VAL *:*
ATYP        Atoms with matching atom type
    e.g ATYP XCL  or ATYP 93
SPHERE      Atoms within distance of atom or coordinate
    e.g SPHE 5.5 CA 1S:2    or SPHE BYRES 5 0.25 1.0 0.1
ROUND       Residues within distance of atom or coordinate
    e.g ROUND 5.5 CA 2 prot.msf  (same as SPHE BYRES)
TYPE        Residues matching residue ID or in range
    e.g TYPE VAL ALA CYS
ZONE        Residues matching residue type
    e.g ZONE 2 4 10  or ZONE 1S:20 TO $ ($ is last residue)
NAYB        Atoms within distance of residue
    e.g NAYB 10 1S:10 or NAYB BYRES 10 12,A
SEGM        Segments matching segment name
    e.g SEGM 1S 2H
PROX        Atoms within distance of segment or molecule
    e.g PROX 10 1S or PROX BYRES 15 prot.msf
FOURTH      Atoms given fourth parameter value
    e.g FOURTH exp 10 (exp= EQ GT GE LT LE)
XCOR        Atoms with given x-coordinate
    e.g XCOR exp .3
YCOR        Atoms with given y-coordinate
    e.g YCOR exp .3
ZCOR        Atoms with given z-coordinate
    e.g ZCOR exp .3
CHARGE      Atoms with given atomic charge
    e.g CHARGE exp 0.0
SYMM        Symmetry-related atoms
    e.g SYMM 2 1
NATO        Atom of given number
    e.g NATO 10 12 14 or NATO 10 to $
NRES        Residue of given number
    e.g NRES 3 1 or NRES 1 to 10
NSEG        Segment of given number
    e.g NSEG 2 to $ or NSEG 1 3 5
FIRST       First atom in residue
    e.g FIRST or FIRST 5 (first atom of every 5th residue
CENT        Atom nearest centroid
    e.g CENT or CENT ALL or CENT MOL (default) or CENT RES 5
EXCL        Exclude atoms in following selection
    e.g EXCL
INCL        Include atoms in following selection
    e.g INCL
LOAD BVALUE Load extra information into 4th param
    e.g LOAD BVALUE
CONN        Connected atoms
    e.g CONN ALL CA 1S:2  or CONN 3 C1
RING        Atoms in same ring system
    e.g RING NE2 1S:3
CYLI        Atoms within a cylinder
    e.g CYLI BYRES 5.0 atom ca 1s:1 atom ca 1s:5
SLAB        Atoms within a slab
    e.g SLAB BYRES 10  7.5  8 atom c1 2
PROT         All atoms in Amino Acid Residues
    e.g PROT
SOLV        All solvent atoms
    e.g SOLV
BACK        Protein Backbone Atoms
    e.g BACK


  int i;
// set all objects in-active
  if(!strcmp(str, "NONE")){
    activeAtom_p.clear();
    activeResidue_p.clear();
    for(i=0; i<atom.size(); i++) atom[i].setParm_active(false);
    for(i=0; i<residue.size(); i++) residue[i].setParm_active(false);
    for(i=0; i<chain.size(); i++) chain[i].setParm_active(false);
  }

  // All atoms
  else if(!strcmp(str, "ALL")){
    for(i=0; i<chain.size(); i++) {
        chain[i].setParm_active(true);
    }
    for(i=0; i<residue.size(); i++) {
        activeResidue_p.push_back(&residue[i]);
        residue[i].setParm_active(true);
    }
    for(i=0; i<atom.size(); i++) {
        activeAtom_p.push_back(&atom[i]);
        atom[i].setParm_active(true);
    }
  }

  // Backbone atoms
  else if(!strcmp(str, "BACKBONE")){
    for(i=0; i<chain.size(); i++) {
        chain[i].setParm_active(true);
    }
    for(i=0; i<residue.size(); i++){
        activeResidue_p.push_back(&residue[i]);
        residue[i].setParm_active(true);

        residue[i].mapAtom["  N  "].setParm_active(false);
        residue[i].mapAtom["  CA "].setParm_active(false);
        residue[i].mapAtom["  C  "].setParm_active(false);
        residue[i].mapAtom["  O  "].setParm_active(false);
        activeAtom_p.push_back(&residue[i].mapAtom["  N  "]);
        activeAtom_p.push_back(&residue[i].mapAtom["  CA "]);
        activeAtom_p.push_back(&residue[i].mapAtom["  C  "]);
        activeAtom_p.push_back(&residue[i].mapAtom["  O  "]);
    }
  }

  // CA atoms
  else if(!strcmp(str, "CA ATOM")){
    for(i=0; i<chain.size(); i++) {
        chain[i].setParm_active(true);
    }
    for(i=0; i<residue.size(); i++){
        activeResidue_p.push_back(&residue[i]);
        residue[i].setParm_active(true);

        residue[i].mapAtom["  CA "].setParm_active(false);
        activeAtom_p.push_back(&residue[i].mapAtom["  CA "]);
    }
  }

  // Heavy atoms
  else if(!strcmp(str, "HEAVY ATOM")){
    for(i=0; i<chain.size(); i++) {
        chain[i].setParm_active(true);
    }
    for(i=0; i<residue.size(); i++) residue[i].setParm_active(true);
    for(i=0; i<atom.size(); i++) {
        if(atom[i].name[2]!='H') atom[i].setParm_active(true);
    }
  }
*/

}


/*
float Protein::rmsd( Protein p2, bool superimpose )
{
This function was translated from an old FORTRAN program, please read
the following description to find the original author:

c**** CALCULATES A BEST ROTATION & TRANSLATION BETWEEN TWO VECTOR SETS
c**** SUCH THAT U*X+T IS THE CLOSEST APPROXIMATION TO Y.
c**** THE CALCULATED BEST SUPERPOSITION MAY NOT BE UNIQUE AS INDICATED
c**** BY A RESULT VALUE IER=-1. HOWEVER IT IS GARANTIED THAT WITHIN
c**** NUMERICAL TOLERANCES NO OTHER SUPERPOSITION EXISTS GIVING A
c**** SMALLER VALUE FOR RMS.
c**** THIS VERSION OF THE ALGORITHM IS OPTIMIZED FOR THREE-DIMENSIONAL
c**** REAL VECTOR SPACE.
c**** USE OF THIS ROUTINE IS RESTRICTED TO NON-PROFIT ACADEMIC
c**** APPLICATIONS.
c**** PLEASE REPORT ERRORS TO
c**** PROGRAMMER:  W.KABSCH   MAX-PLANCK-INSTITUTE FOR MEDICAL RESEARCH
c        JAHNSTRASSE 29, 6900 HEIDELBERG, FRG.
c**** REFERENCES:  W.KABSCH   ACTA CRYST.(1978).A34,827-828
c           W.KABSCH ACTA CRYST.(1976).A32,922-923
c
c  W    - W(M) IS WEIGHT FOR ATOM PAIR  # M           (GIVEN)
c  X    - X(I,M) ARE COORDINATES OF ATOM # M IN SET X       (GIVEN)
c  Y    - Y(I,M) ARE COORDINATES OF ATOM # M IN SET Y       (GIVEN)
c  N    - N IS number OF ATOM PAIRS             (GIVEN)
c  MODE  - 0:CALCULATE RMS ONLY              (GIVEN)
c      1:CALCULATE RMS,U,T   (TAKES LONGER)
c  RMS   - SUM OF W*(UX+T-Y)**2 OVER ALL ATOM PAIRS        (RESULT)
c  U    - U(I,J) IS   ROTATION  MATRIX FOR BEST SUPERPOSITION  (RESULT)
c  T    - T(I)   IS TRANSLATION VECTOR FOR BEST SUPERPOSITION  (RESULT)
c  IER   - 0: A UNIQUE OPTIMAL SUPERPOSITION HAS BEEN DETERMINED(RESULT)
c     -1: SUPERPOSITION IS NOT UNIQUE BUT OPTIMAL
c     -2: NO RESULT OBTAINED BECAUSE OF NEGATIVE WEIGHTS W
c      OR ALL WEIGHTS EQUAL TO ZERO.
c
c-----------------------------------------------------------------------
*/
/*
      int i, j, k, l, m1, m, n, nr;
      float rms, sigma, e0, r[3][3], xc[3], yc[3], wc, a[3][3], b[3][3],
        e[3], d, spur, det, cof, h, g, cth, sth, sqrth, p, tol,
        rr[6], ss[6], ss1, ss2, ss3, ss4,
        ss5, ss6, sqrt3;

      int mode=0;
      int ier;
      float U[3][3], T[3];

      sqrt3 = 1.73205080756888 ;
      tol = 0.01;
      int ip[9] = { 1, 2, 4, 2, 3, 5, 4, 5, 6 };
      int ip2312[4] = { 2, 3, 1, 2 };
      nr = totalResidueNo;
      n = totalAtomNo;
      if(nr != f2.totalResidueNo) {
    cerr << "Error: unequal length for fragment 1 and 2" << endl;
    exit(1);
      }

      wc = 0;
      rms = 0.0;
      e0 = 0;
      for( i = 0; i<3; i++ ) {
            xc[i] = 0;
            yc[i] = 0;
            t[i] = 0.0;

            for(j=0; j<3; j++) {
                if (i == j) {
                u[i][j] = 1;
                a[i][j] = 1;
            } else {
                u[i][j] = 0;
                a[i][j] = 0;
            }

                r[i][j] = 0;
        }
      }

//c**** DETERMINE CENTROIDS OF BOTH VECTOR SETS X AND Y

      ier = -2;

      for(j=0; j<atom.size(); j++) {
      if(atom
        if( strcmp(atom[j].name, f2.atom[j].name) ) {
           cerr << "Atom name unequal between two fragments" << endl;
           exit(1);
        }
        else
        {
           for(i=0; i<3; i++) {
            xc[i] += atom[j].weight * atom[j].xyz[i];
            yc[i] += f2.atom[j].weight * f2.atom[j].xyz[i];
           }
           wc += atom[j].weight;
        }
        a2++;
      }
      }
      else if ( !strcmp(flag, "-h") ) { //heavy atoms RMSD
      for(j=0; j<atom.sjze(); j++) {
        if( strcmp(atom[j].name, f2.atom[j].name) ) {
                   cerr << "Atom name unequal between two fragments" << endl;
                   exit(1);
                }
                else if( atom[j].name[2] == 'H' ) continue;
        else
                {
           for(i=0; i<3; i++) {
            xc[i] += atom[j].weight * atom[j].xyz[i];
            yc[i] += f2.atom[j].weight * f2.atom[j].xyz[i];
           }
           wc += atom[j].weight;
                }
        a2++;
          }
      }
      else if( !strcmp(flag, "-m") ) { // main chain RMSD
    for(j=0; j<residue.size(); j++) {
          wc += residue[j].mapAtom["  N  "].weight +
        residue[j].mapAtom["  CA "].weight +
                residue[j].mapAtom["  C  "].weight +
        residue[j].mapAtom["  O  "].weight ;

      for(i=0; i<3; i++) {

            xc[i] += residue[j].mapAtom["  N  "].weight *
             residue[j].mapAtom["  N  "].xyz[i]
                  +  residue[j].mapAtom["  CA "].weight *
             residue[j].mapAtom["  CA "].xyz[i]
                  +  residue[j].mapAtom["  C  "].weight *
             residue[j].mapAtom["  C  "].xyz[i]
                  +  residue[j].mapAtom["  O  "].weight *
             residue[j].mapAtom["  O  "].xyz[i];
            yc[i] += f2.residue[j].mapAtom["  N  "].weight *
             f2.residue[j].mapAtom["  N  "].xyz[i]
                  +  f2.residue[j].mapAtom["  CA "].weight *
             f2.residue[j].mapAtom["  CA "].xyz[i]
                  +  f2.residue[j].mapAtom["  C  "].weight *
             f2.residue[j].mapAtom["  C  "].xyz[i]
                  +  f2.residue[j].mapAtom["  O  "].weight *
             f2.residue[j].mapAtom["  O  "].xyz[i];
      }

    }
      }
      else if( !strcmp(flag, "-c") ) { // Ca atoms RMSD
        for(j=0; j<residue.size(); j++) {
          wc += residue[j].mapAtom["  CA "].weight ;
          for(i=0; i<3; i++) {
            xc[i] += residue[j].mapAtom["  CA "].weight *
             residue[j].mapAtom["  CA "].xyz[i];
            yc[i] += f2.residue[j].mapAtom["  CA "].weight *
             f2.residue[j].mapAtom["  CA "].xyz[i];
          }
        }
      }
      else {
    cerr << "ERROR: no such flag in fragment rmsd calculation" << endl;
    exit(1);
      }

      if (wc <= 0) {
    cerr << "total weight <= 0 " << endl;
    exit(1);
      }

      for(i=0; i<3; i++) {
        xc[i] = xc[i] / wc;
        yc[i] = yc[i] / wc;
      }

//c**** DETERMINE CORRELATION MATRIX R BETWEEN VECTOR SETS Y AND X


      if( ! strcmp(flag, "-a") ) {
          for(a1 = firstAtom; a1 <= lastAtom; a1++) {
             for(i=0; i<3; i++) {
            e0 += a1.weight *
            ((a1.xyz[i] - xc[i]) * (a1.xyz[i] - xc[i]) +
            ((a2.xyz[i] - yc[i]) * (a2.xyz[i] - yc[i]) ));
            d = a1.weight * (a2.xyz[i] - yc[i]);
            for(j=0; j<3; j++) r[i][j] += d * (a1.xyz[j] - xc[j]);
         }
         a2++;
      }
      }
      else if( ! strcmp(flag, "-h") ) {
          for(a1 = firstAtom; a1 <= lastAtom; a1++) {
         if( a1.name[2] == 'H' ) continue;
             for(i=0; i<3; i++) {
                e0 += a1.weight *
                        ((a1.xyz[i] - xc[i]) * (a1.xyz[i] - xc[i]) +
                        ((a2.xyz[i] - yc[i]) * (a2.xyz[i] - yc[i]) ));
                d = a1.weight * (a2.xyz[i] - yc[i]);
                for(j=0; j<3; j++) r[i][j] += d * (a1.xyz[j] - xc[j]);
             }
             a2++;
          }
      }
      else if( ! strcmp(flag, "-m" ) ) {
      f2.residue[j] = f2.firstResidue;
          for (r1=firstResidue; r1<=lastResidue; r1++) {
             for(i=0; i<3; i++) {
            float tmp1_1 = r1.mapAtom["  N  "].xyz[i] - xc[i] ;
            float tmp1_2 = f2.residue[j].mapAtom["  N  "].xyz[i] - yc[i] ;
            float tmp2_1 = r1.mapAtom["  CA "].xyz[i] - xc[i] ;
            float tmp2_2 = f2.residue[j].mapAtom["  CA "].xyz[i] - yc[i] ;
            float tmp3_1 = r1.mapAtom["  C  "].xyz[i] - xc[i] ;
            float tmp3_2 = f2.residue[j].mapAtom["  C  "].xyz[i] - yc[i] ;
            float tmp4_1 = r1.mapAtom["  O  "].xyz[i] - xc[i] ;
            float tmp4_2 = f2.residue[j].mapAtom["  O  "].xyz[i] - yc[i] ;

                e0 += r1.mapAtom["  N  "].weight *  ( tmp1_1 * tmp1_1 +
              tmp1_2 * tmp1_2 ) +
                      r1.mapAtom["  CA "].weight *  ( tmp2_1 * tmp2_1 +
              tmp2_2 * tmp2_2 ) +
                      r1.mapAtom["  C  "].weight *  ( tmp3_1 * tmp3_1 +
              tmp3_2 * tmp3_2 ) +
                      r1.mapAtom["  O  "].weight *  ( tmp4_1 * tmp4_1 +
              tmp4_2 * tmp4_2 ) ;

                for(j=0; j<3; j++) r[i][j] +=
        r1.mapAtom["  N  "].weight * (r1.mapAtom["  N  "].xyz[j] -
        xc[j] ) * tmp1_2 +
        r1.mapAtom["  CA "].weight * (r1.mapAtom["  CA "].xyz[j] -
        xc[j] ) * tmp2_2 +
        r1.mapAtom["  C  "].weight * (r1.mapAtom["  C  "].xyz[j] -
        xc[j] ) * tmp3_2 +
        r1.mapAtom["  O  "].weight * (r1.mapAtom["  O  "].xyz[j] -
        xc[j] ) * tmp4_2 ;
         }
         f2.residue[j]++;
      }
      }
      else if( ! strcmp(flag, "-c" ) ) {
      f2.residue[j] = f2.firstResidue;
          for (r1=firstResidue; r1<=lastResidue; r1++) {
             for(i=0; i<3; i++) {
            float tmp1_1 = r1.mapAtom["  CA "].xyz[i] - xc[i] ;
            float tmp1_2 = f2.residue[j].mapAtom["  CA "].xyz[i] - yc[i] ;

                e0 += r1.mapAtom["  CA "].weight *
        ( tmp1_1 * tmp1_1 + tmp1_2 * tmp1_2 );

                for(j=0; j<3; j++) r[i][j] +=
        r1.mapAtom["  CA "].weight * (r1.mapAtom["  CA "].xyz[j] -
        xc[j] ) * tmp1_2 ;
         }
         f2.residue[j]++;
      }
      }





//c**** CALCULATE DETERMINANT OF R(I,J)

      det = ((r[0][0] * ((r[1][1] * r[2][2]) - (r[1][2] * r[2][1] ))) -
        ( r[0][1] * ((r[1][0] * r[2][2]) - (r[1][2] * r[2][0] )))) +
        ( r[0][2] * ((r[1][0] * r[2][1]) - (r[1][1] * r[2][0] )));

//c**** FORM UPPER TRIANGLE OF TRANSPOSED(R)*R
      sigma = det;

      m = 0;
      for(j=0; j<3; j++) {
      for(i=0; i<=j; i++) {

//c***************** EIGENVALUES *****************************************
//c**** FORM CHARACTERISTIC CUBIC  X**3-3*SPUR*X**2+3*COF*X-DET=0

        rr[m++] = ((r[0][i] * r[0][j]) + (r[1][i] * r[1][j])) +
        (r[2][i] * r[2][j]);
      }
      }

      spur = ((rr[0] + rr[2]) + rr[5]) / 3.0;
      cof = ((((((rr[2] * rr[5]) - (rr[4] * rr[4])) + (rr[0] * rr[5])) -
        (rr[3] * rr[3])) + (rr[0] * rr[2])) - (rr[1] * rr[1])) / 3.0;

      det = det * det;

      for(i=0; i<3; i++) e[i] = spur;

//c**** REDUCE CUBIC TO STANDARD FORM Y**3-3HY+2G=0 BY PUTTING X=Y+SPUR
      if (spur <= 0) goto loop_40;

      d = spur * spur;
      h = d - cof;

//c**** SOLVE CUBIC. ROOTS ARE e[0],e[1],e[2] IN DECREASING ORDER

      g = (((spur * cof) - det) / 2.0 ) - (spur * h);

      if (h <= 0) goto loop_8;
      sqrth = sqrt(h);
      d = ((h * h) * h) - (g * g);
      if (d < 0) d = 0;
      d = atan2(sqrt(d), - g) / 3.0;
      cth = sqrth * cos(d);
      sth = (sqrth * sqrt3) * sin(d);
      e[0] = (spur + cth) + cth;
      e[1] = (spur - cth) + sth;
      e[2] = (spur - cth) - sth;

//c.....HANDLE SPECIAL CASE OF 3 IDENTICAL ROOTS

      if (mode) goto loop_10;
      else goto loop_50;

//c**************** EIGENVECTORS *****************************************

   loop_8:
      if (mode) goto loop_30;
      else goto loop_50;

   loop_10:
      for(l=1; l<3; l=l+2) {
        d = e[l];
        ss1 = ((d - rr[2]) * (d - rr[5])) - (rr[4] * rr[4]);
        ss2 = ((d - rr[5]) * rr[1]) + (rr[3] * rr[4]);
        ss3 = ((d - rr[0]) * (d - rr[5])) - (rr[3] * rr[3]);
        ss4 = ((d - rr[2]) * rr[3]) + (rr[1] * rr[4]);
        ss5 = ((d - rr[0]) * rr[4]) + (rr[1] * rr[3]);
        ss6 = ((d - rr[0]) * (d - rr[2])) - (rr[1] * rr[1]);

    j = 1;
//      	if ( fabs(ss1) >= fabs(ss3) && fabs(ss1) >= fabs(ss6) ) j=1;
        if ( fabs(ss1) >= fabs(ss3) && fabs(ss1) < fabs(ss6) ) j=3;
        if ( fabs(ss1) < fabs(ss3) && fabs(ss3) < fabs(ss6) ) j=3;
        if ( fabs(ss1) < fabs(ss3) && fabs(ss3) >= fabs(ss6) ) j=2;

        d = 0;
        j = 3 * (j - 1);
    for(i=0; i<3; i++) {
            k = ip[i + j];
            a[i][l] = ss[k];
            d += ss[k] * ss[k];
    }
        if (d > 0) d = 1 / sqrt(d);
        for(i=0; i<3; i++) a[i][l] = a[i][l] * d;
      }

      d = ((a[0][0] * a[0][2]) + (a[1][0] * a[1][2])) + (a[2][0] * a[2][2]);

      m1 = 3;
      m = 1;
      if ((e[0] - e[1]) <= (e[1] - e[2])) {
        m1 = 1;
        m = 3;
      }

      p = 0;
      for(i=0; i<3; i++) {
        a[i][m1] -= d * a[i][m];
        p += a[i][m1] * a[i][m1];
      }

      if (p > tol) {
        p = 1.0 / sqrt(p);
        for(i=0; i<3; i++)  a[i][m1] = a[i][m1] * p;
      }
      else {

        p = 1;
        for(i=0; i<3; i++) {
            if (p < fabs(a[i][m])) continue;
            p = fabs(a[i][m]);
            j = i;
        }

        k = ip2312[j];
        l = ip2312[j + 1];
        p = sqrt(a[k][m] * a[k][m] + a[l][m] * a[l][m]);
        if (p <= tol) goto loop_40;
        a[j][m1] = 0;
        a[k][m1] = - a[l][m] / p;
        a[l][m1] = a[k][m] / p;
      }

      a[0][1] = a[1][2] * a[2][0] - a[1][0] * a[2][2];
      a[1][1] = a[2][2] * a[0][0] - a[2][0] * a[0][2];
      a[2][1] = a[0][2] * a[1][0] - a[0][0] * a[1][2];

//c****************** ROTATION MATRIX ************************************
   loop_30:
      for(l=0; l<2; l++) {
        d = 0;
    for(i=0; i<3; i++) {
            b[i][l] = r[i][0] * a[0][0] + r[i][1] * a[1][0] + r[i][2] * a[2][0];
            d += b[i][l] * b[i][l];
    }
        if (d > 0) d = 1/ sqrt(d);
        for(i=0; i<3; i++) b[i][l] = b[i][l] * d;
      }

      d = b[0][0] * b[0][1] + b[1][0] * b[1][1] + b[2][0] * b[2][1];
      p = 0;

      for(i=0; i<3; i++) {
        b[i][1] -= d * b[i][0];
        p += b[i][1] * b[i][1];
      }

      if (p > tol) {
        p = 1.0 / sqrt(p);
        for(i=0; i<3; i++) b[i][1] = b[i][1] * p;
      }
      else {
        p = 1;
        for(i=0; i<3; i++) {
            if (p < fabs(b[i][1])) continue;
            p = fabs(b[i][1]);
            j = i;
        }

        k = ip2312[j];
        l = ip2312[j + 1];
        p = sqrt(b[k][0] *b[k][0] + b[l][0] * b[l][0]);
        if (p <= tol) goto loop_40;
        b[j][1] = 0;
        b[k][1] = - b[l][0] / p;
        b[l][1] = b[k][0] / p;
      }

      b[0][2] = b[1][0] * b[2][1] - b[1][1] * b[2][0] ;
      b[1][2] = b[2][0] * b[0][1] - b[2][1] * b[0][0] ;
      b[2][2] = b[0][0] * b[1][1] - b[0][1] * b[1][0] ;

      for(i=0; i<3; i++) {
      for(j=0; j<3; j++) {
//c****************** TRANSLATION VECTOR *********************************
     u[i][j] = b[i][0] * a[j][0] + b[i][1] * a[j][1] + b[i][2] * a[j][2];
      }
      }

//c********************** RMS ERROR **************************************
   loop_40:
      for(i=0; i<3; i++) {
    t[i] = yc[i] - u[i][0] * xc[0] - u[i][1] * xc[1] - u[i][2] * xc[2];
      }

   loop_50:
      for(i=0; i<3; i++) {
        if (e[i] < 0) e[i] = 0;
        e[i] = sqrt(e[i]);
      }

      d = e[2];
      if (sigma < 0.0) {
        d = -d;
        if ((e[1] - e[2]) <= (e[0] * 1.0E-05)) ier = -1;
      }
      d = d + e[1] + e[0];
      rms = e0 - 2*d;
      if (rms < 0.0) rms = 0.0;
      return sqrt(rms/wc);
}

*/

// ------- Chain functions

 Chain::Chain()
{
    seqlNo = 0;
    startResidueSeqlNo=endResidueSeqlNo=0;
    startAtomSeqlNo=endAtomSeqlNo=0;
    active=true;
}

 void Chain::setParm() // set parameters by copying them from its member residues
{
  if(residue.size()==0) exit(0);
  int i;

  setParm_name(residue[0].chainName );
}

 void Chain::addResidue(Residue res)
{
    residue.push_back(res);
}

 void Chain::clear()
{
    residue.clear();
}

 ofstream &operator<<(ofstream &ostrm, Chain chain)
// output all of the atom record of this residue
{
    for(int i=0; i<chain.residue.size(); i++) ostrm << chain.residue[i];
    ostrm << "TER" <<endl << endl;
    return ostrm;
}


// ------- Protein functions

 Protein::Protein ()  // construct class
{

  bondAvailable = false; // at the beginning, we don't know any bond information

  // initialize hash
  mapAAProperty["CYS "] = cys;
  mapAAProperty["PHE "] = phe;
  mapAAProperty["ILE "] = ile;
  mapAAProperty["LEU "] = leu;
  mapAAProperty["MET "] = met;
  mapAAProperty["VAL "] = val;
  mapAAProperty["TRP "] = trp;
  mapAAProperty["GLY "] = gly;
  mapAAProperty["ALA "] = ala;
  mapAAProperty["PRO "] = pro;
  mapAAProperty["SER "] = ser;
  mapAAProperty["THR "] = thr;
  mapAAProperty["ASN "] = asn;
  mapAAProperty["GLN "] = gln;
  mapAAProperty["ASP "] = asp;
  mapAAProperty["GLU "] = glu;
  mapAAProperty["TYR "] = tyr;
  mapAAProperty["LYS "] = lys;
  mapAAProperty["HIS "] = his;
  mapAAProperty["ARG "] = arg;
  mapAAProperty["C"] = cys;
  mapAAProperty["F"] = phe;
  mapAAProperty["I"] = ile;
  mapAAProperty["L"] = leu;
  mapAAProperty["M"] = met;
  mapAAProperty["V"] = val;
  mapAAProperty["W"] = trp;
  mapAAProperty["G"] = gly;
  mapAAProperty["A"] = ala;
  mapAAProperty["P"] = pro;
  mapAAProperty["S"] = ser;
  mapAAProperty["T"] = thr;
  mapAAProperty["N"] = asn;
  mapAAProperty["Q"] = gln;
  mapAAProperty["D"] = asp;
  mapAAProperty["E"] = glu;
  mapAAProperty["Y"] = tyr;
  mapAAProperty["K"] = lys;
  mapAAProperty["H"] = his;
  mapAAProperty["R"] = arg;

}

 void Protein::clear()
{
  chain.clear();
  residue_p.clear();
  atom_p.clear();
  mapChain.clear();
  cofactor.clear();
}

 void Protein::readPDB(char *pdbFile)
{

try
 {
  ifstream Fin(pdbFile);
  if(!Fin) throw strcat("Cannot open PDB file " , pdbFile);
  strcpy(name, pdbFile); // give name to this protein object

  string buf;
  int i,j,k;
  Atom newAtom, lastAtom;
  Residue newResidue;
  Chain newChain;
  Hetatm newHetatm;
  int atomSeqlNo, residueSeqlNo, chainSeqlNo;
  atomSeqlNo = residueSeqlNo = chainSeqlNo = 0;
  float x,y,z;

  int newLine, lastLine;
  newLine=lastLine=0;

  i=j=k=0;


  for(;;) {

    /* In each cycle, a new line (or record) is read from PDB file.
    This new line is marked with an integer, which corresponds
    to one of the following cases:

            1. Protein coordinate line, which starts with "ATOM  ".
            2. Cofactor coordinate line, which starts with "HETATM".
            3. Chain terminination line, which starts with "TER".
            4. End of file, detected by function eof().
            5. Other lines.
    */

    getline(Fin, buf);

        if(buf.substr(0,6)=="ATOM  ") newLine=1;
    else if(buf.substr(0,6)=="HETATM") newLine=2;
    else if(buf.substr(0,3)=="TER") newLine=3;
    else if(Fin.eof()) newLine=4;
    else newLine=5;


    /* After reading the the new line, doing different processes
    based on the charactoristics of the new line
    */

    string coordX;
    int idComa;

    switch (newLine) {

        case 1:
        // Read information if it's a protein coordinate line

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
        newAtom.setParm_No(buf.substr(6,5).c_str());
        newAtom.setParm_name(buf.substr(11,5).c_str());
        newAtom.setParm_isomer(buf.substr(16,1).c_str());
        newAtom.setParm_residueName(buf.substr(17,4).c_str());
        newAtom.setParm_chainName(buf.substr(21,1).c_str());
        newAtom.setParm_residueNo(buf.substr(22,5).c_str());

        /*
         * TODO: Verificar como adaptar al locale
        */

        coordX = buf.substr(30,8);
        idComa = coordX.find(".");
        coordX = coordX.replace(idComa, 1, ",");

        x = atof(coordX.c_str());

        coordX = buf.substr(38,8);
        idComa = coordX.find(".");
        coordX = coordX.replace(idComa, 1, ",");
        y = atof(coordX.c_str());

        coordX = buf.substr(46,8);
        idComa = coordX.find(".");
        coordX = coordX.replace(idComa, 1, ",");
        z = atof(coordX.c_str());
        newAtom.setParm_xyz(x,y,z);

        if( buf.length()>66 )
            newAtom.setParm_TFactor(atof(buf.substr(60,6).c_str()));
        else
            newAtom.setParm_TFactor(0);

        if( buf.length()==80 )
        newAtom.setParm_extra(buf.substr(66,13).c_str());

        newAtom.setParm_seqlNo(atomSeqlNo);
        newAtom.setParm_residueSeqlNo(residueSeqlNo);

        atomSeqlNo++;

        break;

        case 2:
            // Read cofactor information if it's a cofactor coordinate line

//              newHetatm.setParm_record(buf.c_str());
//		cofactor.addAtom(newHetatm);

        continue; // start a new cycle to read the next line
        break;

        case 3:

        break;

        case 5:

        continue; // start a new cycle to read the next line
            break;

    }

    /*

        Push the newResidue object into chain and reset newResidue, when:

           last line is a protein coordinate line (lastLine==1) AND

    1) the new line is a coordinate line AND residue (or chain) changed.

       newLine==1 && (residue changed || chain changed);

    2) the new line is a cofactor line OR chain termination line OR
       end of file).

       newLine>1 && newLine<5

    */

    if(lastLine==1 && (

            // Situation 1
        (newLine==1 && (
        strcmp(newAtom.residueNo, lastAtom.residueNo) ||
            strcmp(newAtom.chainName, lastAtom.chainName)))
        ||
            // Situation 2
            (newLine>1 && newLine<5)) )
    {

            newResidue.setParm();

            //push the new residue
            newChain.addResidue(newResidue);

            //reset newResidue
            newResidue.clear();
            newResidue.setParm_startAtomSeqlNo(atomSeqlNo-1);

            residueSeqlNo++;
            newAtom.setParm_residueSeqlNo( residueSeqlNo);

    }

    /*

    Push newChain into protein and reset newChain, it happens when:

           last line is a protein coordinate line AND

    1) the new line is a coordinate line AND chain changed:

       newLine==1 && chain changed

    2) the new line is a chain termination line OR end of file:

       newLine>2 && newLine<5

    */

    if(lastLine==1 && (
        // Situation 1
            ( newLine==1 &&
        strcmp(newAtom.chainName, lastAtom.chainName) )
        ||
        // Situation 2
        (newLine>2 && newLine<5) ) ){

                newChain.setParm();
            // add newChain to protein
            newChain.setParm_endResidueSeqlNo(residueSeqlNo-2);
            newChain.setParm_endAtomSeqlNo(atomSeqlNo-1);
            addChain(newChain);

            // reset newChain
            newChain.clear();
            newChain.setParm_startResidueSeqlNo(residueSeqlNo-1);
            newChain.setParm_startAtomSeqlNo(atomSeqlNo);
            chainSeqlNo++;

    }

    /*

    end of file

    */

    // After we are sure that the newResidue have been reset when it has
    // to be, we can now add newAtom to it
    if(newLine==1) {

        // para debugueo

//        if(residueSeqlNo == 0 ||
//                residueSeqlNo == 1){
//            string nombreAtomo (newAtom.name);
//            nombreAtomo = trim(nombreAtomo);

//            if(nombreAtomo.compare("CA") == 0||
//                    nombreAtomo.compare("C") == 0||
//                    nombreAtomo.compare("N") == 0){
//                cout<<"Atomo "<<nombreAtomo.c_str()<<": "<<endl;
//                printf("(%.3f, %.3f, %.3f)\n", x, y, z);
//                printf("(%s, %s, %s)\n",
//                       buf.substr(30,8).c_str(),
//                       buf.substr(38,8).c_str(),
//                       buf.substr(46,8).c_str());

//            }

//        }
        newResidue.addAtom(newAtom);
    }

    // If it is the enf of file, end the loop
    if(newLine==4) break;

    // otherwise, prepare for the next cycle
    lastLine=newLine;
    lastAtom=newAtom;
  }

  Chain *lastChain_p;
  for(i=0; i<chain.size(); i++) {
    for(j=0; j<chain[i].residue.size(); j++) {

        for(k=0; k<chain[i].residue[j].atom.size(); k++) {
            Atom *pa = &chain[i].residue[j].atom[k];
            atom_p.push_back(pa);
            chain[i].atom_p.push_back(pa);
            chain[i].residue[j].setParm_mapAtom(pa->name,pa);
        }
        Residue *pr = &chain[i].residue[j];
        residue_p.push_back(pr);
        chain[i].setParm_mapResidue(pr->No,pr);
    }

    Chain *pc = &chain[i];
     // in case that multiple chains having the same name
    if(i!=0) {    // lastChain_p existed
        // if two chains happened to have the same name, usually
        // the first chain is the one we are really interested in,
        // so we don't need to set the map to the second one
        if( !strcmp(pc->name, lastChain_p->name) ) continue;
    }

    setParm_mapChain(pc->name, pc);
    lastChain_p = pc;
  }

  Fin.close();

 }

 catch(char* pMsg) { cerr << endl << "Exception:" << pMsg << endl; }

}


 void Protein::writePDB(char *pdbFile)
{

try
 {
  ofstream Fout(pdbFile);

  if(!Fout)
  {
    cerr<<"Cannot open pdb output file " << pdbFile << endl;
    exit(1);
  }

  // output coordinate
  int i,j,k;
  i=0;

  for(i=0; i<chain.size(); i++) {
    for(j=0; j<chain[i].atom_p.size(); j++) {
        Atom* atm= chain[i].atom_p[j];
        Fout << "ATOM  "
        << atm->No
        << atm->name
        << atm->isomer
        << atm->residueName
        << atm->chainName
        << atm->residueNo
        << "   ";
        Fout.setf(ios::showpoint|ios::fixed);
        Fout
        <<setprecision(3)<<setw(8)<<atm->xyz[0]
        <<setprecision(3)<<setw(8)<<atm->xyz[1]
        <<setprecision(3)<<setw(8)<<atm->xyz[2]
        <<"  1.00" <<setprecision(2)<<setw(6)
        << atm->TFactor <<endl;
    }
    Fout <<"TER"<<endl;
  }

  //output connection

  if(bondAvailable) {
     for(i=0; i<atom_p.size(); i++) {
        Fout<<"CONNECT"<<atom_p[i]->No;
        for(int j=0;j<atom_p[i]->bondNo;j++)  Fout<< atom_p[i]->bond[j]->No;
        Fout<<endl;
     }
  }

  Fout << "END"<<endl;

  Fout.close();
 }

catch(char* pMsg) { cerr << endl << "Exception:" << pMsg << endl; }

}

 void Protein::writePDB(char *file, char *chainName, int order=0)

/* output to a PDB file
                order=0: original order
                order=1: reorder atom number, starting from 1
                order=2: reorder residue number, starting from 1
                order=3: reorder both
*/


{
  ofstream Fout(file);

  if(!Fout) throw strcat("Error: cannot open pdb output file ",file);

  // output coordinate
  int i,j;
  i=0;

  if(chainName[0]==' ' || chainName[0]=='_') {
     for(i=0; i<chain.size(); i++) {
     for(j=0; j<chain[i].atom_p.size(); j++) {
    Atom *atm= chain[i].atom_p[j];

    Fout << "ATOM  ";
    if( order!=1 && order!=-3 )
        Fout << atm->No;
    else
        Fout << setw(5)<< atm->seqlNo + 1;
    Fout << atm->name;
    Fout << atm->isomer;
    Fout << atm->residueName << atm->chainName;
    if( order!=-2 && order!=-3 )
        Fout << atm->residueNo;
    else
        Fout << setw(4)<< atm->residueSeqlNo+1 <<" ";
    Fout << "   ";
    Fout.setf(ios::showpoint|ios::fixed);
    Fout<<setprecision(3)<<setw(8)<<atm->xyz[0];
    Fout<<setprecision(3)<<setw(8)<<atm->xyz[1];
    Fout<<setprecision(3)<<setw(8)<<atm->xyz[2];
    Fout<<"  1.00" <<setprecision(2)<<setw(6)<< atm->TFactor <<endl;
     }
     Fout <<"TER"<<endl;
     }
  }
  else
  {

     if(!mapChain.count(chainName)) throw strcat("No such a chain name as ",chainName);

     for(j=0; j<mapChain[chainName]->atom_p.size(); j++) {
    Atom *atm= mapChain[chainName]->atom_p[j];

    Fout << "ATOM  ";
    if( order!=1 && order!=-3 )
        Fout << atm->No;
    else
        Fout << setw(5)<< atm->seqlNo + 1;
    Fout << atm->name;
    Fout << atm->isomer;
    Fout << atm->residueName << atm->chainName;
    if( order!=-2 && order!=-3 )
        Fout << atm->residueNo;
    else
        Fout << setw(4)<< atm->residueSeqlNo+1 <<" ";
    Fout << "   ";
    Fout.setf(ios::showpoint|ios::fixed);
    Fout<<setprecision(3)<<setw(8)<<atm->xyz[0];
    Fout<<setprecision(3)<<setw(8)<<atm->xyz[1];
    Fout<<setprecision(3)<<setw(8)<<atm->xyz[2];
    Fout<<"  1.00" <<setprecision(2)<<setw(6)<< atm->TFactor <<endl;
     }
  }


  //output connection
  for( i=0; i<atom_p.size(); i++ ) {
     Atom *atm = atom_p[i];

     if( order!=1 && order!=3 )
     {
    Fout<<"CONNECT"<<atm->No;
    for(int j=0;j<atm->bondNo;j++)  Fout<< atm->bond[j]->No;
     }
     else
     {
    Fout<<"CONNECT"<< setw(5) << atm->seqlNo + 1;
    for(int j=0;j<atm->bondNo;j++)
        Fout<< setw(5) << atm->bond[j]->seqlNo + 1;
     }

     Fout<<endl;
  }

  Fout << "END"<<endl;

  Fout.close();
}



 void Protein::bondGeneration(int bFlag) // -------  generate bond

{

  int i,j;
//  if(bFlag==0){ // bond generation based on distance

    for(i=0;i<atom_p.size()-1;i++){
        for(j=i+1;j<atom_p.size();j++){
            if((*atom_p[i]-*atom_p[j])<bondLengthLimit(*atom_p[i],*atom_p[j]) &&
        ! strcmp (atom_p[i]->chainName, atom_p[j]->chainName))
        {
                atom_p[i]->link(*atom_p[j]);
                atom_p[j]->link(*atom_p[i]);
            }
        }
        }
//  }

    bondAvailable = true;  // bond information was set up from now on

}


 void Protein::writeFasta(const char* fastaFile)
// generate protein sequence in Fasta format
{

  int i, j, k;
  ofstream Fout(fastaFile);

  Fout << ">" << fastaFile << endl;
  for(i=0; i<chain.size(); i++) {
     k=0;
     for(j=0; j<chain[i].residue.size(); j++) {
        k = j + 1;
        Fout << chain[i].residue[j].name_1;
        if(k%10 == 0 && k%50 !=0) Fout <<" ";
        if(k%50 == 0 ) Fout <<endl;
     }
     Fout<<endl;
  }
}


 void Protein::writeHydrophobicity(char *file)  // generate hydrophobicity
                           // profile
{
  int i,j;
 try{
  ofstream Fout(file);
  if(!Fout) throw strcat("ERROR: cannot open hydrophobicity file ",file);

  cout << " Hydrophobicity profile was output to file: " << file
  << ", file format is: " << endl
  << "	residue number, residue name, hydrophobicity"<<endl;

  for(i=0;i<chain.size();i++){
    for(j=0;j<chain[i].residue.size();j++)
    Fout << chain[i].residue[j].No << " " << chain[i].residue[j].name
    << " " << chain[i].residue[j].hydrophobicity << endl;

    Fout << "TER" << endl;
  }
 }
 catch(char* pMsg) { cerr << endl << "Exception:" << pMsg << endl; }
}

/*

float Protein::rmsd(Protein p2, char *flag, int beginResidue, int endResidue)
// calculate rmsd of this structure to structure p2
{
  int i, N = 0;
  float rmsd2 = 0;
  Atom *atom_p1, *atom_p2;

  for(i=beginResidue-1; i<endResidue; i++){
     if(!strcmp(flag, "-c")) {
           rmsd2 += pow(residue[i].mapAtom["  CA "]
           - p2.residue[i].mapAtom["  CA "], 2);
       N++;
     }
     else if(!strcmp(flag, "-m")) {
           rmsd2 +=
       pow(residue[i].mapAtom["  N  "] - p2.residue[i].mapAtom["  N  "], 2)+
       pow(residue[i].mapAtom["  CA "] - p2.residue[i].mapAtom["  CA "], 2)+
       pow(residue[i].mapAtom["  C  "] - p2.residue[i].mapAtom["  C  "], 2)+
       pow(residue[i].mapAtom["  O  "] - p2.residue[i].mapAtom["  O  "], 2);
       N += 4;
     }
     else if(!strcmp(flag, "-a")) {
       atom_p1 = residue[i].atom_p.begin();
       atom_p2 = p2.residue[i].atom_p.begin();
       for(;atom_p1!=residue[i].atom_p.end();){
        rmsd2 += pow(*atom_p1 - *atom_p2, 2);
        atom_p1++;
        atom_p2++;
        N++;
       }
     }
     else {
       atom_p1 = residue[i].firstAtom;
       atom_p2 = p2.residue[i].firstAtom;
       for(;atom_p1<=residue[i].lastAtom;){
            string name = atom_p1.name;
            if(name[2] != 'H') {
           rmsd2 += pow(*atom_p1 - *atom_p2, 2);
            }
        atom_p1++;
        atom_p2++;
        N++;
       }
     }
  }

  return sqrt(rmsd2/N);
}
*/

 void Protein::readDSSP(char *dsspFile)
//read information of torsion angle etc. from DSSP file
{

 try
 {
  ifstream Fin(dsspFile);
  if(!Fin) throw strcat("Cannot open dssp file ",dsspFile);

  char buf[200], resNo[6], chainNo[2];

  for(; !Fin.eof(); )
  {
    Fin.get(buf, 4);
    Fin.ignore(INT_MAX, '\n');
    if(!strcmp(buf, "  #")) break;
  }

  for(; !Fin.eof(); )
  {
    Fin.ignore(6);
    Fin.get(resNo, 6);
    Fin.get(chainNo, 2);

    if(!mapChain.count(chainNo)) {
        Fin.ignore(INT_MAX, '\n');
        continue;
    }

    if(!mapChain[chainNo]->mapResidue.count(resNo)) {
        Fin.ignore(INT_MAX, '\n');
        continue;
    }

    Fin.ignore(4);
    Fin.get(buf, 2);
    mapChain[chainNo]->mapResidue[resNo]->setParm_SS(buf);
    Fin.ignore(86);
    Fin.get(buf, 7);
    mapChain[chainNo]->mapResidue[resNo]->setParm_phi(atof(buf));
    Fin.get(buf, 7);
    mapChain[chainNo]->mapResidue[resNo]->setParm_psi(atof(buf));

    Fin.ignore(200, '\n');
  }

  Fin.close();

 }

 catch(char* pMsg) { cerr << endl << "Exception:" << pMsg << endl; }


}

 void Protein::readHSSP(char *hsspFile)
// read alignment from HSSP file
{
try
 {
  int i, append;

  ifstream Fin(hsspFile);
  if(!Fin) throw strcat("Cannot open hssp file ", hsspFile);

  char buf[200], resNo[6], chainNo[2], lastResNo[6];
//  int tmpi;

  for(; !Fin.eof(); )
  {
    Fin.get(buf, 14);
    if(!strcmp(buf, "## ALIGNMENTS")) break;
    Fin.ignore(INT_MAX, '\n');
  }

  Fin.ignore(8, '\n');
  Fin.get(buf, 6);
  setParm_homologyNo(atoi(buf));
  Fin.ignore(INT_MAX, '\n');
  Fin.ignore(INT_MAX, '\n');

  append = 65;  //charactor 'A'
  bool ambiguousRes = false;

  for(i=0; !Fin.eof(); i++)
  {
    Fin.get(buf,8);
    if(buf[0]=='#') break;

    Fin.get(resNo, 6);
    Fin.get(chainNo, 2);

    if( !strncmp(lastResNo, resNo, 4) ) {

//	    cerr << "Residue numbering ambiguous in hssp file: "
//		 << hsspFile <<endl;
//	    cerr << "It happened in residue: " << resNo << " chain: "
//		 << chainNo << endl;
//	    exit(1);

        ambiguousRes = true;
        resNo[4] = (char) append++;
    } else {
        ambiguousRes = false;
        append = 65;
    }

    strcpy(lastResNo, resNo);

    Chain *pc;

    if(chainNo[0]==' ') pc=&chain[0];
    else
    {
        if(!mapChain.count(chainNo)) {
            Fin.ignore(INT_MAX, '\n');
            continue;
        }

        pc=mapChain[chainNo];

    }

    if(!pc->mapResidue.count(resNo)) {
        Fin.ignore(INT_MAX,'\n');
        continue;
    }

    Fin.ignore(38);
    Fin.get(buf, homologyNo+1);
    pc->mapResidue[resNo]->setParm_homology(buf);

    Fin.ignore(INT_MAX, '\n');
  }
 }

 catch(char* pMsg) { cerr << endl << "Exception:" << pMsg << endl; }

}


 DNAchain::DNAchain() {
  seq = "";
  id = "";
  description = "";
}

 void DNAchain::set_id(string input_id) {
  id = input_id;
}

 void DNAchain::set_description(string input_description) {
  description = input_description;
}

 void DNAchain::set_seq(string input_seq) {
  seq = input_seq;
}

 void DNAchain::clear() {
  seq = "";
  id = "";
  description = "";
}

bool not_nucleotide(char ch) {
  string str = "ACGT";
  string ts(1,ch);
  if( str.find(ts) == string::npos ) return true;
  else return false;
}

 void DNAchain::cleanSeq() {
  string::iterator endp = remove_if(seq.begin(), seq.end(), not_nucleotide);
  seq.erase(endp, seq.end());
}

 int DNAchain::length() {
  return seq.size();
}

char compliment_nucleotide(char ch) {
  string str = "ACGT";
  string ts(1,ch);
  if( ch == 'A' ) return 'T';
  else if( ch == 'T' ) return 'A';
  else if( ch == 'C' ) return 'G';
  else return 'C';
}

 DNAchain DNAchain::reverse_compliment() {
  DNAchain tmpChain;
  tmpChain.id = id;
  tmpChain.description = description;
  tmpChain.seq = seq;

  reverse_copy(seq.begin(), seq.end(), tmpChain.seq.begin());
  transform(tmpChain.seq.begin(), tmpChain.seq.end(), tmpChain.seq.begin(),
            compliment_nucleotide);
  return tmpChain;
}

class hsp {
  int query_start, query_end;
  int sbjct_start, sbjct_end;
};

/*DNAchain DNAchain::assemble(DNAchain c2, vector<hsp> hsps) {

DNAchain DNAchain::assemble(DNAchain c1, DNAchain c2, int minmatch) {
  int i,j;
  // DNAchain query_chain = this->length() > c2.length() ? c2 : *this ;
  // DNAchain sbjct_chain = this->length() > c2.length() ? *this : c2 ;
  // string query = query_chain.seq;
  // string sbjct = sbjct_chain.seq;
  string query = c1.seq;
  string sbjct = c2.seq;

  // first, build a list of the query's substrings
  if( sbjct->find(sbjct, query, minmatch) )

  vector<int> subStrings;
  string::iterator p;
  for(i=0; i<query.size(); i++) {
      // if the substring doesn't start and end with 'N', put it in the list
      if( query[i] != 'N' && query[p+minmatch] != 'N' ) {
      subStrings.push_back(i);
      }
  }

  // second, search every substring in the sbjct string
  for(i=0; i<subStrings.size(); i++) {
      string sbjct = sbjct_chain.seq;
      if( sbjct->find(query, subStrings[i], minmatch) )

  }
}
*/


DNA::DNA() { }

 void DNA::readFasta( char *file ) // read Fasta format sequences
{
  try {
      ifstream Fin(file);
      if(!Fin) throw strcat("Cannot open input Fasta file ", file);

      bool fastaFormat = false;
      string buf;
      int newLine;
      DNAchain tmpChain;


      for(;;) {

      // read a new line in every cycle
      getline(Fin, buf);

      /* The new line is marked with an integer, which corresponds
      to one of the following cases:

            1. New sequence line, which starts with "> ".
            2. Sequence line, which starts with any charactor except for ">"
            3. End of file, detected by function eof().
      */

          if(buf[0] == '>' ) newLine=1;
      else if(Fin.eof()) newLine=3;
      else newLine=2;

          if( newLine == 1 ) { // this is a start of a new chain.
          if( !fastaFormat ) {
              // if it is the first chain, just build a new chain object
          tmpChain.clear();
              fastaFormat = true;
          } else {
              // otherwise, need to store the old chain first, then build a new chain
          tmpChain.cleanSeq();
              addChain(tmpChain);
          tmpChain.clear();
          }
          tmpChain.set_description( buf.substr(1,buf.size()-1) );
          int pos = buf.find(" ");
          tmpChain.set_id( buf.substr(1,pos) );
      }

      if( newLine == 2 && fastaFormat ) { // sequence line
          tmpChain.seq.append(buf);
      }

      if( newLine == 3 ) { // end of file
          if( !fastaFormat ) throw strcat(file, " is not in Fasta format.");
          else {
              // otherwise, need to store the old chain first, then to
              // build a new chain

              tmpChain.cleanSeq();
              addChain(tmpChain);
              break;
          }
      }
      }
  }

  catch(char* pMsg) { cerr << endl << "Exception:" << pMsg << endl; }
}

 void DNA::writeFasta ( char* file )
{

  try {
      ofstream Fout(file);
      if(!Fout) throw strcat("Cannot open output Fasta file ", file);

      int i,j;
      for(i=0; i<chain.size(); i++) {
      Fout << ">" << chain[i].description << endl;
      for(j=0;j<chain[i].seq.size();j++) {
          Fout << chain[i].seq[j];
          if((j+1)%60==0) Fout << endl;
      }
      Fout << endl;
      }
  }

  catch(char* pMsg) { cerr << endl << "Exception:" << pMsg << endl; }
}

 void DNA::addChain(DNAchain dc)
{
  chain.push_back(dc);
}
