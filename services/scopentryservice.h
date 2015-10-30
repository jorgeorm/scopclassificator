#ifndef SCOPENTRYSERVICE_H
#define SCOPENTRYSERVICE_H
#include "models/scopentry.h"

class SCOPEntryService
{
public:
    SCOPEntryService();

    /**
     * @brief loadEntFile parses and loads ent files into SCOPEntry model,
     *  which will contain both SCOP data and PDB information
     * @param pathToFile
     * @return
     */
    SCOPEntry *loadEntFile(const QString &pathToFile);

    /**
     * @brief loadOnlyMetaData parses and loads ent file into SCOPEntry Model,
     *  which will contain only SCOP data.
     * @param pathToFile
     * @return
     */
    SCOPEntry *loadOnlyMetaData(const QString &pathToFile);

protected:
    void loadLineIntoAtom(const QString &line, Atom* actualAtom);

    void loadAtomIntoResidue(Atom* atom, Residue* residue);

    void loadAtomIntoChain(Atom* atom, Chain* chain);

    void closeChain(Chain* lastChain, Residue* lastResidue);
};

#endif // SCOPENTRYSERVICE_H
