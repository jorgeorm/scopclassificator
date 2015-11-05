#include "residue.h"

using namespace Bio;

Residue::Residue():
_chainId(),
_atoms(),
_mapAtoms(){
    _chain = NULL;

    _seqNo = 0;
//    _chainSeqNo = 0;
    _numOfAtoms = 0;
    _startAtomSeqNo = 0;
    _endAtomSeqNo = 0;

    _shortLenghtName = "";
    _mediumLengthName = "";
    _fullLengthName = "";
//    _chainName = "";
}

Residue::Residue(const Residue& other):
_chainId(other.chainId()),
_atoms(other.atoms()),
_mapAtoms(other.mapAtoms()){

    _chain = other.chain();
    _seqNo = other.seqNo();
//    _chainSeqNo = other.chainSeqNo();
    _numOfAtoms = other.numOfAtoms();
    _startAtomSeqNo = other.startAtomSeqNo();
    _endAtomSeqNo = other.endAtomSeqNo();
    _shortLenghtName = other.shortLenghtName();
    _mediumLengthName = other.mediumLengthName();
    _fullLengthName = other.fullLengthName();
//    _chainName = other.chainName();

}

Residue::~Residue(){
}

Residue& Residue::operator=(const Residue& other){
    _atoms = other.atoms();
    _mapAtoms = other.mapAtoms();

    _chain = other.chain();
    _seqNo = other.seqNo();
//    _chainSeqNo = other.chainSeqNo();
    _numOfAtoms = other.numOfAtoms();
    _startAtomSeqNo = other.startAtomSeqNo();
    _endAtomSeqNo = other.endAtomSeqNo();
    _shortLenghtName = other.shortLenghtName();
    _mediumLengthName = other.mediumLengthName();
    _fullLengthName = other.fullLengthName();
//    _chainName = other.chainName();

    return *this;
}

QString Residue::shortLenghtName() const
{
    return _shortLenghtName;
}

void Residue::setShortLenghtName(const QString &shortLenghtName)
{
    _shortLenghtName = shortLenghtName;
}

QString Residue::mediumLengthName() const
{
    return _mediumLengthName;
}

void Residue::setMediumLengthName(const QString &mediumLengthName)
{
    _mediumLengthName = mediumLengthName;
}

QString Residue::fullLengthName() const
{
    return _fullLengthName;
}

void Residue::setFullLengthName(const QString &fullLengthName)
{
    _fullLengthName = fullLengthName;
}

unsigned int Residue::seqNo() const
{
    return _seqNo;
}

void Residue::setSeqNo(const unsigned int &seqNo)
{
    _seqNo = seqNo;
}

//int Residue::chainSeqNo() const
//{
//    return _chainSeqNo;
//}

//void Residue::setChainSeqNo(int chainSeqNo)
//{
//    _chainSeqNo = chainSeqNo;
//}

//QString Residue::chainName() const
//{
//    return _chainName;
//}

//void Residue::setChainName(const QString &chainName)
//{
//    _chainName = chainName;
//}

int Residue::numOfAtoms() const
{
    return _numOfAtoms;
}

void Residue::setNumOfAtoms(int numOfAtoms)
{
    _numOfAtoms = numOfAtoms;
}

int Residue::startAtomSeqNo() const
{
    return _startAtomSeqNo;
}

void Residue::setStartAtomSeqNo(int startAtomSeqNo)
{
    _startAtomSeqNo = startAtomSeqNo;
}

int Residue::endAtomSeqNo() const
{
    return _endAtomSeqNo;
}

void Residue::setEndAtomSeqNo(int endAtomSeqNo)
{
    _endAtomSeqNo = endAtomSeqNo;
}

QList<Atom *> Residue::atoms() const{
    return _atoms;
}

void Residue::addAtom(Atom *atom){
    _atoms.append(atom);
}

void Residue::setAtoms(const QList<Atom *> &atoms){
    _atoms = atoms;
}

Atom *Residue::getAtomByName(QString atomName){
    return _mapAtoms.value(atomName);
}

void Residue::setAtomByName(QString atomName, Atom *atom){
    _mapAtoms[atomName] = atom;
}

QMap<QString, Atom *> Residue::mapAtoms() const{
    return _mapAtoms;
}

Chain *Residue::chain() const
{
    return _chain;
}

void Residue::setChain(Chain *chain)
{
    _chain = chain;
}

QChar Residue::chainId() const
{
    return _chainId;
}

void Residue::setChainId(const QChar &chainId)
{
    _chainId = chainId;
}

Atom Residue::normalVector()
const {
    Atom *n = _mapAtoms["N"];
    Atom *ca = _mapAtoms["CA"];
    Atom *c = _mapAtoms["C"];

    Atom CaN = *n<=*ca;
    Atom CaC = *c<=*ca;

    Atom normal = CaN * CaC;

    return normal;
}

double Residue::operator-(const Bio::Residue &other)
{
    Atom *atomI, *atomJ;
    atomI = _mapAtoms["CA"];
    atomJ = other.mapAtoms()["CA"];
    if (atomI != NULL && atomJ != NULL) {

        return *atomI - *atomJ;
    } else {

//        qDebug() << " ----There was an error with the following residues"
//                 << this->shortLenghtName() << ": " << this->seqNo()
//                 << ", " << other.shortLenghtName() << ": " << other.seqNo();

//        if(atomI == NULL) qDebug() << " ---- First atom NULL ";
//        else qDebug() << " ---- Second atom NULL ";

        return std::numeric_limits<double>::max();
    }
}

double Residue::operator%(const Bio::Residue &other)
{
    if (_mapAtoms.contains("CA") && other.mapAtoms().contains("CA")){
        Atom thisNormal = normalVector();
        Atom otherNormal = other.normalVector();

        thisNormal.normalize();
        otherNormal.normalize();

        double cosTetha = 0.0;

        cosTetha = thisNormal.dotProduct(otherNormal);

        return acos(cosTetha);
    } else {
        return M_1_PI;
    }

}
