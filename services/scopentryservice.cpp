#include "scopentryservice.h"

#include <QFile>
#include <QObject>
#include "loggerservice.h"
#include <QDebug>
#include <iostream>
#include "models/bio/atom.h"


SCOPEntryService::SCOPEntryService() {
}

void SCOPEntryService::loadLineIntoAtom(const QString &line, Atom* actualAtom) {
    actualAtom->setSeqNo(line.mid(6, 5).toUInt());
    actualAtom->setShortLenghtName(line.mid(12, 4).trimmed());
    if (! line.mid (16, 2).trimmed ().isEmpty ())
        actualAtom->setAltLoc (line.mid (16, 2).trimmed ().at (0));
    actualAtom->setShortLenghtResidueName(line.mid(17, 3).trimmed());
    if (! line.mid (21, 2).trimmed ().isEmpty ())
        actualAtom->setChainID (line.mid (21, 2).trimmed ().at (0));
    actualAtom->setSeqResNo(line.mid(22, 4).toUInt());
    if (! line.mid (26, 2).trimmed ().isEmpty ())
        actualAtom->setICode (line.mid (26, 2).trimmed ().at (0));

    actualAtom->setPosition(
                line.mid(30, 8).toFloat(),
                line.mid(38, 8).toFloat(),
                line.mid(46, 8).toFloat());
    actualAtom->setOccupancy(line.mid(54, 6).toUInt());
    actualAtom->setTempFactor(line.mid(60, 6).toFloat());
    actualAtom->setOptionalData(line.right(14));
//    actualAtom->setSegmentIdentifier(line.mid(72,4));
//    actualAtom->setElement(line.mid(76, 3).trimmed());
//    actualAtom->setCharge(line.mid(78, 3).trimmed());

//    qDebug()<<"Atom created and configured";
    //qDebug()<<*actualAtom;

}

void SCOPEntryService::loadAtomIntoResidue(Atom *atom, Residue *residue) {
    if (residue->shortLenghtName().isEmpty()){ //New residue requires init data
        residue->setChainId(atom->chainID());
        residue->setSeqNo(atom->seqResNo());
        residue->setShortLenghtName(atom->shortLenghtResidueName());
        residue->setStartAtomSeqNo(atom->seqNo());
    }

    // Add references from the residue to the atom and vice versa
    residue->addAtom(atom);
    residue->setAtomByName(atom->shortLenghtName(), atom);
    atom->setResidue(residue);
}

void SCOPEntryService::loadAtomIntoChain(Atom *atom, Chain* chain) {
    chain->addAtom(atom);

    if (chain->chainId ().isNull ()){
        chain->setChainId(atom->chainID());
    }

    if (NULL == chain->getResidueByName(atom->shortLenghtResidueName())){
        chain->addResidue(atom->residue());
        chain->setResidue(atom->shortLenghtResidueName(), atom->residue());
    }
}

void SCOPEntryService::closeChain(Chain* lastChain, Residue* lastResidue) {
    Atom *lastAtomLoaded = lastResidue->atoms().last();
    lastResidue->setEndAtomSeqNo(lastAtomLoaded->seqNo());
    lastResidue->setNumOfAtoms(lastResidue->atoms().size());

    lastChain->setEndAtomSeqlNo(lastAtomLoaded->seqNo());
    lastChain->setEndResidueSeqlNo(lastResidue->seqNo());
    lastChain->setTotalResidueNo(lastChain->residues().size());
    lastChain->setTotalAtomNo(lastChain->atoms().size());

}

SCOPEntry *SCOPEntryService::loadEntFile(const QString &pathToFile){
    SCOPEntry *scopEntry = NULL;

    QList<Atom *> atoms;
    QList<Residue *> residues;
    QList <Chain *> chains;

    QFile entFile (pathToFile);

//
//    HEADER    SCOP/ASTRAL domain d1dlwa_ [14982]      28-JUL-05   0000
//    REMARK  99
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

   QMap<QString, QString> scopAtributesMap;

    if (entFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&entFile);
        QString line;
        QStringList splitedLine;

        Atom * actualAtom = NULL;
        Residue * actualResidue = NULL;
        Chain * actualChain = NULL;


        while (! in.atEnd()){

            line = in.readLine();

            if (line.startsWith("HEADER")){

            } else if (line.startsWith("REMARK  99")){

                line.remove(0, 11);
                if(line.isEmpty()) continue;
                splitedLine = line.split(QRegExp("\\s+"));
                scopAtributesMap.insert(splitedLine.at(0), splitedLine.at(1));
            } else if (line.startsWith("ATOM")){

                if (actualAtom == NULL) actualAtom = new Atom;
                if (actualResidue == NULL) actualResidue = new Residue;
                if (actualChain == NULL) actualChain = new Chain;

                loadLineIntoAtom(line, actualAtom);

                if((! actualResidue->shortLenghtName().isEmpty()) &&
                    //Residue contains data and
                    //Is required to reset the residue
                        actualResidue->seqNo() != actualAtom->seqResNo()){
                    actualResidue->setEndAtomSeqNo(actualResidue->atoms().last()->seqNo());
                    actualResidue->setNumOfAtoms(actualResidue->atoms().size());
                    residues.push_back(actualResidue);

                    //Closed residue with ending data
                    actualResidue = NULL; // new residue
                    actualResidue = new Residue;

                }

                loadAtomIntoResidue(actualAtom, actualResidue);
                loadAtomIntoChain(actualAtom, actualChain);
                atoms.push_back(actualAtom);

                actualAtom = NULL;
            } else if (line.startsWith("TER")){
                residues.push_back(actualResidue);
                closeChain(actualChain, actualResidue);

                chains.push_back(actualChain);

                actualChain = NULL;
                actualResidue = NULL;
//                qDebug()<<"RecordName: "<<line.mid(0, 6); //RecordName
//                qDebug()<<"Serial - Int: "<<line.mid(6, 5); //serial - Int
//                qDebug()<<"resName - Str: "<<line.mid(17, 3); //resName - Str
//                qDebug()<<"ChainId - Char: "<<line.mid(21, 2); //chainID - Char
//                qDebug()<<"resSeq - Int: "<<line.mid(22, 4); //resSeq - Int
//                qDebug()<<"iCode - Char: "<<line.mid(26, 2); //iCode - Char
            } else continue;
        }

        entFile.close();


    } else {
        qDebug()<<"No pude abrir el archivo "<<pathToFile;

        LoggerService log;
        log.logError(QString("No se pudo abrir el archivo: ").append(
                         pathToFile));
        return NULL;
    }

    scopEntry = new SCOPEntry(
        pathToFile,
        scopAtributesMap.value("SCOP-sid:", "Not Provided"),
        scopAtributesMap.value("SCOP-sun:", "Not Provided"),
        scopAtributesMap.value("ASTRAL-version:", "0").toFloat(),
        scopAtributesMap.value("SCOP-sccs:", "Not Provided"),
        scopAtributesMap.value("Source-PDB:", "Not Provided"),
        scopAtributesMap.value("Source-PDB-REVDAT:", "Not Provided"),
        scopAtributesMap.value("Region:", "Not Provided"),
        scopAtributesMap.value("ASTRAL-SPACI:", "0").toFloat(),
        scopAtributesMap.value("ASTRAL-AEROSPACI:", "0").toFloat(),
        scopAtributesMap.value("Data-updated-release:", "0").toFloat()
    );

    scopEntry->setAtoms(atoms);
    scopEntry->setResidues(residues);
    scopEntry->setChains(chains);

    return scopEntry;
}

SCOPEntry *SCOPEntryService::loadOnlyMetaData(const QString &pathToFile){
    SCOPEntry *scopEntry = NULL;

    QList<Atom *> atoms;
    QList<Residue *> residues;
    QList <Chain *> chains;

    QFile entFile (pathToFile);

//
//    HEADER    SCOP/ASTRAL domain d1dlwa_ [14982]      28-JUL-05   0000
//    REMARK  99
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

   QMap<QString, QString> scopAtributesMap;

    if (entFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&entFile);
        QString line;
        QStringList splitedLine;

        while (! in.atEnd()){

            line = in.readLine();

            if (line.startsWith("HEADER")){

            } else if (line.startsWith("REMARK  99")){

                line.remove(0, 11);
                if(line.isEmpty()) continue;
                splitedLine = line.split(QRegExp("\\s+"));
                scopAtributesMap.insert(splitedLine.at(0), splitedLine.at(1));
            } else continue;
        }

        entFile.close();


    } else {
        qDebug()<<"No pude abrir el archivo "<<pathToFile;

        LoggerService log;
        log.logError(QString("No se pudo abrir el archivo: ").append(
                         pathToFile));
        return NULL;
    }

    scopEntry = new SCOPEntry(
        pathToFile,
        scopAtributesMap.value("SCOP-sid:", "Not Provided"),
        scopAtributesMap.value("SCOP-sun:", "Not Provided"),
        scopAtributesMap.value("ASTRAL-version:", "0").toFloat(),
        scopAtributesMap.value("SCOP-sccs:", "Not Provided"),
        scopAtributesMap.value("Source-PDB:", "Not Provided"),
        scopAtributesMap.value("Source-PDB-REVDAT:", "Not Provided"),
        scopAtributesMap.value("Region:", "Not Provided"),
        scopAtributesMap.value("ASTRAL-SPACI:", "0").toFloat(),
        scopAtributesMap.value("ASTRAL-AEROSPACI:", "0").toFloat(),
        scopAtributesMap.value("Data-updated-release:", "0").toFloat()
    );

    return scopEntry;
}
