﻿#ifndef RESIDUE_H
#define RESIDUE_H

#include "bio.h"
#include "atom.h"
#include <QMap>


class Bio::Residue
{
public:
    Residue();
    Residue(const Residue& other);
    virtual ~Residue();
    Residue& operator=(const Residue& other);


    QString shortLenghtName() const;
    void setShortLenghtName(const QString &shortLenghtName);

    QString mediumLengthName() const;
    void setMediumLengthName(const QString &mediumLengthName);

    QString fullLengthName() const;
    void setFullLengthName(const QString &fullLengthName);

    unsigned int seqNo() const;
    void setSeqNo(const unsigned int &seqNo);

//    int chainSeqNo() const;
//    void setChainSeqNo(int chainSeqNo);

//    QString chainName() const;
//    void setChainName(const QString &chainName);

    int numOfAtoms() const;
    void setNumOfAtoms(int numOfAtoms);

    int startAtomSeqNo() const;
    void setStartAtomSeqNo(int startAtomSeqNo);

    int endAtomSeqNo() const;
    void setEndAtomSeqNo(int endAtomSeqNo);

    QList<Bio::Atom*> atoms() const;
    void addAtom(Bio::Atom* atom);
    void setAtoms(const QList<Bio::Atom*> &atoms);

    Bio::Atom* getAtomByName(QString atomName);
    void setAtomByName(QString atomName, Bio::Atom* atom);
    QMap<QString, Bio::Atom *> mapAtoms() const;

    Bio::Chain *chain() const;
    void setChain(Bio::Chain *chain);

    QChar chainId() const;
    void setChainId(const QChar &chainId);

    /**
     * @brief normalVector Calculates the normal vector generated by the plain N-Ca-C as an unit vector.
     * @return
     */
    Bio::Atom normalVector() const;

    /**
     * @brief operator - Calculates distance to atom
     * @param other
     * @return
     */
    double operator-(const Residue& other);
    /**
     * @brief operator % Calculates the angle between two residues.
     * @param other
     * @return
     */
    double operator%(const Residue& other);
private:
    QString _shortLenghtName;
    QString _mediumLengthName;
    QString _fullLengthName;
    unsigned int _seqNo;

    QChar _chainId;
//    int _chainSeqNo;
//    QString _chainName;

    int _numOfAtoms;
    int _startAtomSeqNo;
    int _endAtomSeqNo;

    QList<Bio::Atom*> _atoms;
    QMap<QString, Bio::Atom*> _mapAtoms;

    Bio::Chain * _chain;

};

#endif // RESIDUE_H
