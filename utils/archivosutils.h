#ifndef ARCHIVOSUTILS_H
#define ARCHIVOSUTILS_H
#include <QFile>

class ArchivosUtils
{
public:
    /**
     * @brief ArchivosUtils
     */
    ArchivosUtils();

    /**
     * @brief copiarQFile
     * @param archivo
     * @param pathSalida
     * @return
     */
    static bool copiarQFile(QFile * archivo, QString pathSalida);

    /**
     * @brief copiarArchivo
     * @param pathEntrada
     * @param pathSalida
     * @return
     */
    static bool copiarArchivo(QString pathEntrada, QString pathSalida);
};

#endif // ARCHIVOSUTILS_H
