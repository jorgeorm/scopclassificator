#include "atom.h"

#include <QDebug>

using namespace Bio;

Atom::Atom():
_iCode()//,
//_segmentIdentifier()
{
    _seqNo = 0;
    _shortLenghtName = "";
    _seqResNo = 0;
    _shortLenghtResidueName = "";
    _tempFactor = 0.0;
    _position[0] = 0.0;
    _position[1] = 0.0;
    _position[2] = 0.0;

    _residue = NULL;
    _chain = NULL;
}

Atom::Atom(const Atom &other) :
_seqNo(other.seqNo()),
_shortLenghtName(other.shortLenghtName()),
_altLoc(other.altLoc()),
_seqResNo(other.seqResNo()),
_shortLenghtResidueName(other.shortLenghtResidueName()),
_iCode(other.iCode()),
_chainID(other.chainID()),
//_segmentIdentifier(other.segmentIdentifier()),
_occupancy(other.occupancy()),
_residue(other.residue()),
_chain(other.chain()){
    _position[0] = other.position()[0];
    _position[1] = other.position()[1];
    _position[2] = other.position()[2];
}

Atom::~Atom(){

}

Atom &Atom::operator=(const Atom &other){
    _seqNo = other.seqNo();
    _shortLenghtName = other.shortLenghtName();
    _altLoc = other.altLoc();
    _seqResNo = other.seqResNo();
    _shortLenghtResidueName = other.shortLenghtResidueName();
//    _seqChainNo = other.seqChainNo();
//    _shortLenghtChainName = other.shortLenghtChainName();
//    _segmentIdentifier = other.segmentIdentifier();
    _tempFactor= other.tempFactor();

    _position[0] = other.position()[0];
    _position[1] = other.position()[1];
    _position[2] = other.position()[2];
    _residue = other.residue();
    _chain = other.chain();

    return *this;
}

Residue *Atom::residue() const
{
    return _residue;
}

void Atom::setResidue(Residue *residue)
{
    _residue = residue;
}

Chain *Atom::chain() const
{
    return _chain;
}

void Atom::setChain(Chain *chain)
{
    _chain = chain;
}

QChar Atom::altLoc() const
{
    return _altLoc;
}

void Atom::setAltLoc(const QChar &altLoc)
{
    _altLoc = altLoc;
}

QChar Atom::chainID() const
{
    return _chainID;
}

void Atom::setChainID(const QChar &chainID)
{
    _chainID = chainID;
}

unsigned int Atom::occupancy() const
{
    return _occupancy;
}

void Atom::setOccupancy(unsigned int occupancy)
{
    _occupancy = occupancy;
}

float Atom::tempFactor() const
{
    return _tempFactor;
}

void Atom::setTempFactor(float tempFactor)
{
    _tempFactor = tempFactor;
}

void Atom::setICode(const QChar &iCode)
{
    _iCode = iCode;
}

//QString Atom::element() const
//{
//    return _element;
//}

//void Atom::setElement(const QString &element)
//{
//    _element = element;
//}

//QString Atom::charge() const
//{
//    return _charge;
//}

//void Atom::setCharge(const QString &charge)
//{
//    _charge = charge;
//}

//QString Atom::segmentIdentifier() const
//{
//    return _segmentIdentifier;
//}

//void Atom::setSegmentIdentifier(const QString &segmentIdentifier)
//{
//    _segmentIdentifier = segmentIdentifier;
//}

QString Atom::optionalData() const
{
    return _optionalData;
}

void Atom::setOptionalData(const QString &optionalData)
{
    _optionalData = optionalData;
}

double Bio::Atom::distanceSquare(Bio::Atom &other)
{
    double result = 0;
    double difference = 0;

    for (unsigned i = 0; i < 3; ++i){
        difference = this->_position[i] - other.position()[i];
        result += pow(difference, 2.0);
    }

    return result;
}

double Atom::dotProduct(Atom &other){
    double result = 0;

    for (unsigned i = 0; i < 3; ++i)
        result += _position[i]*other.position()[i];

    return result;
}

void Atom::normalize(){
    float norm = 0;

    for (unsigned int i = 0; i < 3; ++i)
        norm += pow(_position[i], 2.0);

    norm = sqrt(norm);

    _position[0] /= norm;
    _position[1] /= norm;
    _position[2] /= norm;
}

double Atom::operator-(Bio::Atom &other)
{
    double result = distanceSquare(other);
    return sqrt(result);
}

Atom Atom::operator*(const Bio::Atom &other) {

    float x,y,z;
    const float * otherXYZ = other.position();
    x = _position[1]*otherXYZ[2]-_position[2]*otherXYZ[1];
    y = -_position[0]*otherXYZ[2]+_position[2]*otherXYZ[0];
    z = _position[0]*otherXYZ[1]-_position[1]*otherXYZ[0];

    Atom tmp;
    tmp.setPosition(x, y, z);
    return tmp;
}

Atom Atom::operator<=(const Bio::Atom &origin){
    Atom tmp=*this;

    float x,y,z;

    x = _position[0] - origin.position()[0];
    y = _position[1] - origin.position()[1];
    z = _position[2] - origin.position()[2];

    tmp.setPosition(x, y, z);
    return tmp;
}

QChar Atom::iCode() const
{
    return _iCode;
}

QString Atom::shortLenghtName() const{ return _shortLenghtName;}
void Atom::setShortLenghtName(const QString &shortLenghtName){ _shortLenghtName = shortLenghtName;}

QString Atom::shortLenghtResidueName() const{ return _shortLenghtResidueName;}
void Atom::setShortLenghtResidueName(const QString &shortLenghtResidueName){ _shortLenghtResidueName = shortLenghtResidueName;}

//QString Atom::shortLenghtChainName() const{ return _shortLenghtChainName;}
//void Atom::setShortLenghtChainName(const QString &shortLenghtChainName){ _shortLenghtChainName = shortLenghtChainName;}

//float Atom::tFactor() const{ return _tFactor;}
//void Atom::setTFactor(float tFactor){ _tFactor = tFactor;}

const float * Atom::position() const{ return &_position[0];}
void Atom::setPosition(float x, float y, float z){
    _position[0] = x;
    _position[1] = y;
    _position[2] = z;
}

unsigned int Atom::seqNo() const{ return _seqNo;}
void Atom::setSeqNo(unsigned int seqNo){ _seqNo = seqNo;}

unsigned int Atom::seqResNo() const{ return _seqResNo;}
void Atom::setSeqResNo(unsigned int seqResNo){ _seqResNo = seqResNo;}

//unsigned int Atom::seqChainNo() const{ return _seqChainNo;}
//void Atom::setSeqChainNo(unsigned int seqChainNo){ _seqChainNo = seqChainNo;}

QDebug Bio::operator<<(QDebug dbg, Bio::Atom &atom){
    dbg.nospace()<<"Serial - Int: "<< atom.seqNo()<<"\n";
    dbg.nospace()<<"Atom Name - Str: "<< atom.shortLenghtName()<<"\n";
    dbg.nospace()<<"AltLoc - Char: "<< atom.altLoc()<<"\n";
    dbg.nospace()<<"resName - Str: "<< atom.shortLenghtResidueName()<<"\n";
    dbg.nospace()<<"ChainId - Char: "<< atom.chainID()<<"\n";
    dbg.nospace()<<"resSeq - Int: "<< atom.seqResNo()<<"\n";
    dbg.nospace()<<"iCode - Char: "<< atom.iCode()<<"\n";
    dbg.nospace()<<"X - Real 8.3: "<< atom.position()[0]<<"\n";
    dbg.nospace()<<"Y - Real 8.3: "<< atom.position()[1]<<"\n";
    dbg.nospace()<<"Z - Real 8.3: "<< atom.position()[2]<<"\n";
    dbg.nospace()<<"ocpancy - Real 6.2: "<< atom.occupancy()<<"\n";
    dbg.nospace()<<"tempFactor - Real 6.2: "<< atom.tempFactor()<<"\n";
    dbg.nospace()<<"additionalData: "<<atom.optionalData();
//    dbg.nospace()<<"Element: "<< atom.element()<<"\n";
//    dbg.nospace()<<"Charge: "<< atom.charge()<<"\n";

    return dbg.maybeSpace();
}

std::ostream & Bio::operator<<(std::ostream &os, const Atom &atom)
{
    os<<"Serial - Int: "<< atom.seqNo()<<"\n";
    os<<"Atom Name - Str: "<< qPrintable(atom.shortLenghtName())<<"\n";
    os<<"AltLoc - Char: "<< qPrintable(atom.altLoc())<<"\n";
    os<<"resName - Str: "<< qPrintable(atom.shortLenghtResidueName())<<"\n";
    os<<"ChainId - Char: "<< qPrintable(atom.chainID())<<"\n";
    os<<"resSeq - Int: "<< atom.seqResNo()<<"\n";
    os<<"iCode - Char: "<< qPrintable(atom.iCode())<<"\n";
    os<<"X - Real 8.3: "<< atom.position()[0]<<"\n";
    os<<"Y - Real 8.3: "<< atom.position()[1]<<"\n";
    os<<"Z - Real 8.3: "<< atom.position()[2]<<"\n";
    os<<"ocpancy - Real 6.2: "<< atom.occupancy()<<"\n";
    os<<"tempFactor - Real 6.2: "<< atom.tempFactor()<<"\n";
    os<<"additionaldata: "<<qPrintable(atom.optionalData());
//    os<<"Element: "<< qPrintable(atom.element())<<"\n";
//    os<<"Charge: "<< qPrintable(atom.charge())<<"\n";

    return os;
}
