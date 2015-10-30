#include "chain.h"

using namespace Bio;

Chain::Chain():
_chainId(),
_residues(),
_atoms(),
_mapResidue() {

//    _name = "";
//    _seqlNo = 0;
    _startAtomSeqlNo = 0;
    _endAtomSeqlNo = 0;
    _totalAtomNo = 0;
    _startResidueSeqlNo = 0;
    _endResidueSeqlNo = 0;
    _totalResidueNo = 0;

}

Chain::Chain(const Chain &other):
_chainId(other.chainId()),
_residues(other.residues()),
_atoms(other.atoms()),
_mapResidue(other.mapResidue()){
//    _name = other.name();
//    _seqlNo = other.seqlNo();
    _startAtomSeqlNo = other.startAtomSeqlNo();
    _endAtomSeqlNo = other.endAtomSeqlNo();
    _totalAtomNo = other.totalAtomNo();
    _startResidueSeqlNo = other.startResidueSeqlNo();
    _endResidueSeqlNo = other.endResidueSeqlNo();
    _totalResidueNo = other.totalResidueNo();
}

Chain::~Chain(){

}

Chain &Chain::operator=(const Chain &other){

    _chainId = other.chainId();
    _mapResidue = other.mapResidue();
    _atoms = other.atoms();
    _residues = other.residues();
//    _name = other.name();
//    _seqlNo = other.seqlNo();
    _startAtomSeqlNo = other.startAtomSeqlNo();
    _endAtomSeqlNo = other.endAtomSeqlNo();
    _totalAtomNo = other.totalAtomNo();
    _startResidueSeqlNo = other.startResidueSeqlNo();
    _endResidueSeqlNo = other.endResidueSeqlNo();
    _totalResidueNo = other.totalResidueNo();

    return *this;
}


//GETTERS
//QString Chain::name() const
//{
//    return _name;
//}

//int Chain::seqlNo() const
//{
//    return _seqlNo;
//}

int Chain::startAtomSeqlNo() const
{
    return _startAtomSeqlNo;
}

int Chain::endAtomSeqlNo() const
{
    return _endAtomSeqlNo;
}

int Chain::totalAtomNo() const
{
    return _totalAtomNo;
}

int Chain::startResidueSeqlNo() const
{
    return _startResidueSeqlNo;
}

int Chain::endResidueSeqlNo() const
{
    return _endResidueSeqlNo;
}

int Chain::totalResidueNo() const
{
    return _totalResidueNo;
}

QList<Residue *> Chain::residues() const
{
    return _residues;
}

QList<Atom *> Chain::atoms() const
{
    return _atoms;
}

QMap<QString, Residue *> Chain::mapResidue() const
{
    return _mapResidue;
}

Residue *Chain::getResidueByName(QString resName) const{
    return _mapResidue.value(resName, NULL);
}


//SETERS
//void Chain::setName(const QString &name)
//{
//    _name = name;
//}

//void Chain::setSeqlNo(int seqlNo)
//{
//    _seqlNo = seqlNo;
//}

void Chain::setStartAtomSeqlNo(int startAtomSeqlNo)
{
    _startAtomSeqlNo = startAtomSeqlNo;
}

void Chain::setEndAtomSeqlNo(int endAtomSeqlNo)
{
    _endAtomSeqlNo = endAtomSeqlNo;
}

void Chain::setTotalAtomNo(int totalAtomNo)
{
    _totalAtomNo = totalAtomNo;
}

void Chain::setStartResidueSeqlNo(int startResidueSeqlNo)
{
    _startResidueSeqlNo = startResidueSeqlNo;
}

void Chain::setEndResidueSeqlNo(int endResidueSeqlNo)
{
    _endResidueSeqlNo = endResidueSeqlNo;
}

void Chain::setTotalResidueNo(int totalResidueNo)
{
    _totalResidueNo = totalResidueNo;
}

void Chain::addResidue(Residue *presiude)
{
    _residues.append(presiude);
}

void Chain::addAtom(Atom *patom)
{
    _atoms.append(patom);
}

void Chain::setAtoms(const QList<Atom *> &atoms){
    _atoms = atoms;
}

void Chain::setResidue(QString name, Residue *presidue){
    _mapResidue[name] = presidue;
}

bool Chain::isActive() const{
    return _active;
}

void Chain::setActive(bool active){
    _active = active;
}

QChar Chain::chainId() const
{
    return _chainId;
}

void Chain::setChainId(const QChar &chainId)
{
    _chainId = chainId;
}
