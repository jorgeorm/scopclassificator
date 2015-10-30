#ifndef CHAIN_H
#define CHAIN_H

#include "bio.h"

#include <QList>
#include <QMap>
#include <QString>



class Bio::Chain
{
public:
    Chain();
    Chain(const Chain& other);
    virtual ~Chain();
    Chain& operator=(const Chain& other);


//GETTERS
//    QString name() const;

//    int seqlNo() const;

    int startAtomSeqlNo() const;
    int endAtomSeqlNo() const;
    int totalAtomNo() const;
    int startResidueSeqlNo() const;
    int endResidueSeqlNo() const;
    int totalResidueNo() const;
    QList<Bio::Residue *> residues() const;
    QList<Bio::Atom *> atoms() const;
    QMap<QString, Bio::Residue *> mapResidue() const;
    Bio::Residue * getResidueByName(QString resName) const;

//SETERS
//    void setName(const QString &name);
//    void setSeqlNo(int seqlNo);
    void setStartAtomSeqlNo(int startAtomSeqlNo);
    void setEndAtomSeqlNo(int endAtomSeqlNo);
    void setTotalAtomNo(int totalAtomNo);
    void setStartResidueSeqlNo(int startResidueSeqlNo);
    void setEndResidueSeqlNo(int endResidueSeqlNo);
    void setTotalResidueNo(int totalResidueNo);
    void addResidue(Bio::Residue * presiude);
    void addAtom(Bio::Atom * patom);
    void setAtoms(const QList<Atom*> &atoms);
    void setResidue(QString name, Residue* presidue);
    bool isActive() const;
    void setActive(bool active);

    QChar chainId() const;
    void setChainId(const QChar &chainId);

private:
    QChar _chainId;
    int _startAtomSeqlNo;
    int _endAtomSeqlNo;
    int _totalAtomNo;
    int _startResidueSeqlNo;
    int _endResidueSeqlNo;
    int _totalResidueNo;
    QList<Bio::Residue *> _residues;  // a vector containing all residues
    QList<Bio::Atom *> _atoms;
    QMap<QString, Bio::Residue *> _mapResidue; // map a residue's PDB No to its object

    // other variables
    bool _active; // status of this chain
};

#endif // CHAIN_H
