#ifndef SCOPENTRY_H
#define SCOPENTRY_H

#include <QObject>

#include "bio/bio.h"
#include "bio/atom.h"
#include "bio/residue.h"
#include "bio/chain.h"

using namespace Bio;

class SCOPEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sid READ sid)
    Q_PROPERTY(QString sun READ sun)
    Q_PROPERTY(float astralVersion READ astralVersion)
    Q_PROPERTY(QString scss READ scss)
    Q_PROPERTY(QString sourcePdb READ sourcePdb)
    Q_PROPERTY(QString sourcePdbRevDat READ sourcePdbRevDat)
    Q_PROPERTY(QString region READ region)
    Q_PROPERTY(QString sourceFile READ sourceFile)
    Q_PROPERTY(unsigned int numResidues READ numResidues)
    Q_PROPERTY(unsigned int fileSize READ fileSize)

public:
    enum SCOPCLASSLEVEL {CLASS, FOLD, SUPER_FAMILY, FAMILY, SPECIES};
    explicit SCOPEntry(QObject *parent = 0);
    SCOPEntry(
        QString sourceFile,
        QString sin,
        QString sun,
        float astralVersion,
        QString scss,
        QString sourcePdb,
        QString sourcePdbRevDat,
        QString region,
        float spaci,
        float aerospaci,
        float dataUpdatedRelease
    );
    SCOPEntry(QString sourceFile);
    virtual ~SCOPEntry();

    //Getters
    QString sid() const;
    QString sun() const;
    float astralVersion() const;
    QString scss() const;
    QString scss(SCOPCLASSLEVEL level) const;
    QString sourcePdb() const;
    QString sourcePdbRevDat() const;
    QString region() const;

    float spaci() const;
    float aerospaci() const;
    float dataUpdatedRelease() const;


    unsigned int numResidues() const;
    unsigned int fileSize() const;
    QString sourceFile() const;

    void setAtoms(const QList<Bio::Atom *> &atoms);
    void setResidues(const QList<Bio::Residue *> &residues);
    void setChains(const QList<Bio::Chain *> &chains);

    QList<Bio::Atom *> atoms();
    QList<Bio::Residue *> residues();
    QList<Bio::Chain *> chains();

signals:

public slots:

private:
    // SCOP ATTRIBUTES IN HEADER AND REMARK
    //    REMARK  99 ASTRAL-version: 1.75
    //    REMARK  99 SCOP-sid: d1dlwa_
    //    REMARK  99 SCOP-sun: 14982
    //    REMARK  99 SCOP-sccs: a.1.1.1
    //    REMARK  99 Source-PDB: 1dlw
    //    REMARK  99 Source-PDB-REVDAT: 20-SEP-00
    //    REMARK  99 Region: a:
    //    REMARK  99 ASTRAL-SPACI: 0.66
    //    REMARK  99 ASTRAL-AEROSPACI: 0.66
    //    REMARK  99 Data-updated-release: 1.61
    QString _sid;
    QString _sun;
    float _astralVersion;
    QString _scss;
    QString _sourcePdb;
    QString _sourcePdbRevDat;
    QString _region;
    float _spaci;
    float _aerospaci;
    float _dataUpdatedRelease;
    //PDB quick access attributes
    unsigned int _numResidues;
    unsigned int _fileSize;
    QString _sourceFile;


    // PDB related variables
    QList<Bio::Atom *> _atoms;
    QList<Bio::Residue *> _residues;
    QList<Bio::Chain *> _chains;
};

#endif // SCOPENTRY_H
