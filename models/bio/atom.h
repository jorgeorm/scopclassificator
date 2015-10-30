#ifndef ATOM_H
#define ATOM_H

#include "bio.h"

#include <QString>

class Bio::Atom
{
public:
    Atom();
    Atom(const Atom& other);
    virtual ~Atom();
    Atom& operator=(const Atom& other);

    QString shortLenghtName() const;
    void setShortLenghtName(const QString &shortLenghtName);

    QString shortLenghtResidueName() const;
    void setShortLenghtResidueName(const QString &shortLenghtResidueName);

//    QString shortLenghtChainName() const;
//    void setShortLenghtChainName(const QString &shortLenghtChainName);

//    float tFactor() const;
//    void setTFactor(float tFactor);

    const float * position() const;
    void setPosition(float x, float y, float z);

    unsigned int seqNo() const;
    void setSeqNo(unsigned int seqNo);

    unsigned int seqResNo() const;
    void setSeqResNo(unsigned int seqResNo);

//    unsigned int seqChainNo() const;
//    void setSeqChainNo(unsigned int seqChainNo);

    Bio::Residue *residue() const;
    void setResidue(Bio::Residue *residue);

    Bio::Chain *chain() const;
    void setChain(Bio::Chain *chain);

    QChar altLoc() const;
    void setAltLoc(const QChar &altLoc);

    QChar chainID() const;
    void setChainID(const QChar &chainID);

    unsigned int occupancy() const;
    void setOccupancy(unsigned int occupancy);

    float tempFactor() const;
    void setTempFactor(float tempFactor);

    QChar iCode() const;
    void setICode(const QChar &iCode);

//    QString element() const;
//    void setElement(const QString &element);

//    QString charge() const;
//    void setCharge(const QString &charge);

//    QString segmentIdentifier() const;
//    void setSegmentIdentifier(const QString &segmentIdentifier);

    QString optionalData() const;
    void setOptionalData(const QString &optionalData);

    /**
     * @brief operator - Calculates distance between two atoms
     * @param other
     * @return
     */
    double operator-(Atom &other);

    /**
     * @brief operator * Calculates vector cross product
     * @return
     */
    Bio::Atom operator*(const Atom &other);

    /**
     * @brief operator <= Calculates a vector from the origin atom to this atom.
     * @param origin
     * @return
     */
    Bio::Atom operator<=(const Atom &origin);


    /**
     * @brief distanceSquare Calculates sqare distance between this and other atom.
     * @param other
     * @return
     */
    double distanceSquare(Bio::Atom &other);

    /**
     * @brief dotProduct Calculates dot product between this and other atom.
     * @param other
     * @return
     */
    double dotProduct(Bio::Atom &other);

    /**
     * @brief normalize Normalizes the vector of position
     */
    void normalize();

private:
    friend QDebug operator<<(QDebug dbg, Bio::Atom &atom);
    friend std::ostream & operator<<(std::ostream &os, const Bio::Atom& atom);

    unsigned int _seqNo; //Atom serial number.
    QString _shortLenghtName; //Atom name.
    QChar _altLoc;       //Alternate location indicator.
    unsigned int _seqResNo;//Residue sequence number.
    QString _shortLenghtResidueName;//Residue name.
    QChar _iCode; //Code for insertion of residues
    QChar _chainID; //Chain identifier.
    float _tempFactor; //Temperature factor.
//    QString _segmentIdentifier; //Segment identifier
    unsigned int _occupancy;//Occupancy
    float _position[3];//Orthogonal coordinates for X,Y,Z in Angstroms
    QString _optionalData;// Captures the element, charge and other data included.
//    QString _element;//Element symbol, right-justified
//    QString _charge;//Charge on the atom.
    Bio::Residue * _residue; //points to the residue it belongs.
    Bio::Chain * _chain; //points to the chain that it belongs.
};

#endif // ATOM_H
