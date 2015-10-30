#ifndef BIO_H
#define BIO_H

#include <QDebug>


namespace Bio{

class Atom;
class Residue;
class Chain;

QDebug operator<<(QDebug dbg, const Bio::Atom &atom);
std::ostream & operator<<(std::ostream &os, const Bio::Atom &atom);

}

#endif // BIO_H

