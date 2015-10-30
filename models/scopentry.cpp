#include "scopentry.h"


#include <QFileInfo>
#include <QDebug>

SCOPEntry::SCOPEntry(QObject *parent) : QObject(parent){

    _numResidues = 0;
}

SCOPEntry::SCOPEntry(
    QString sourceFile,
    QString sid,
    QString sun,
    float astralVersion,
    QString scss,
    QString sourcePdb,
    QString sourcePdbRevDat,
    QString region,
    float spaci,
    float aerospaci,
    float dataUpdatedRelease) {

    SCOPEntry();

    _sid = sid;
    _sun = sun;
    _astralVersion = astralVersion;
    _scss = scss;
    _sourcePdb = sourcePdb;
    _sourcePdbRevDat = sourcePdbRevDat;
    _region = region;
    _spaci = spaci;
    _aerospaci = aerospaci;
    _dataUpdatedRelease = dataUpdatedRelease;
    _sourceFile = sourceFile;
    _numResidues = 0;

}

SCOPEntry::~SCOPEntry() {
    qDeleteAll(_chains);
    qDeleteAll(_residues);
    qDeleteAll(_atoms);
}

// GETTERS
QString SCOPEntry::sid() const{ return _sid; }
QString SCOPEntry::sun() const{ return _sun; }
float SCOPEntry::astralVersion() const{ return _astralVersion; }
QString SCOPEntry::scss() const{ return _scss; }

QString SCOPEntry::scss(SCOPEntry::SCOPCLASSLEVEL level) const{
    switch (level) {
    case CLASS:
        return _scss.left(1);
        break;
    case FOLD:
        return _scss.left(3);
        break;
    case SUPER_FAMILY:
        return _scss.left(5);
        break;
    case FAMILY:
        return _scss;
        break;
    default:
        return _scss;
        break;
    }
}
QString SCOPEntry::sourcePdb() const{ return _sourcePdb; }
QString SCOPEntry::sourcePdbRevDat() const{ return _sourcePdbRevDat; }
QString SCOPEntry::region() const{ return _region;}
QString SCOPEntry::sourceFile() const { return _sourceFile; }

void SCOPEntry::setAtoms(const QList<Atom *> &atoms){ _atoms = atoms; }

void SCOPEntry::setResidues(const QList<Residue *> &residues){
    _residues = residues;
    _numResidues = residues.size();
}

void SCOPEntry::setChains(const QList<Chain *> &chains){ _chains = chains; }

float SCOPEntry::spaci() const { return _spaci; }
float SCOPEntry::aerospaci() const { return _aerospaci; }
float SCOPEntry::dataUpdatedRelease() const { return _dataUpdatedRelease; }

unsigned int SCOPEntry::numResidues() const{ return _numResidues; }
unsigned int SCOPEntry::fileSize() const{ return _fileSize; }



QList<Atom *> SCOPEntry::atoms(){ return _atoms; }
QList<Residue *> SCOPEntry::residues(){ return _residues; }
QList<Chain *> SCOPEntry::chains(){ return _chains; }
