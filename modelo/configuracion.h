#ifndef CONFIGURACION_H
#define CONFIGURACION_H
#include <QString>
#include <QJsonObject>

/**
 * @brief La clase Configuracion mapea los atributos configurables en la aplicación.
 */
class Configuracion
{
public:
    /**
     * @brief Configuracion Constructor sin parametros
     */
    Configuracion();

    ~Configuracion(){

    }

    /**
     * @brief Configuracion es el constructor parametrizado
     */
    Configuracion(QString _qstr_ipRserve,
                  QString _qstr_pathTMP,
                  QString _qstr_pathFFTMP,
                  int _i_maxBytesFF,
                  int _i_maxResMem,
                  int _i_maxBytesFFLote,
                  int _i_numHilos);

    /**
     * @brief escribirConfig Escriben en objeto JSON la configuración.
     * @param <QJsonObject> jsonObj referencia a objeto JSON donde se parsea la configuració
     */
    void escribirConfig(QJsonObject & jsonObj);

    /**
     * @brief leerConfig Carga de un objeto JSON la configuración.
     * @param <QJsonObject> jsonObj referencia a objeto JSON de donde se extraeran los datos
     */
    void leerConfig(const QJsonObject & jsonObj);



    //Getters y setters
    QString getQstr_ipRserve() const;
    void setQstr_ipRserve(const QString &value);

    QString getQstr_pathTMP() const;
    void setQstr_pathTMP(const QString &value);

    QString getQstr_pathFFTMP() const;
    void setQstr_pathFFTMP(const QString &value);

    long long getI_maxBytesFF() const;
    void setI_maxBytesFF(long long value);

    long long getI_maxBytesFFLote() const;
    void setI_maxBytesFFLote(long long value);

    int getI_maxResMem() const;
    void setI_maxResMem(int value);

    int getI_maxHilos() const;
    void setI_maxHilos(int value);

private:
    QString qstr_ipRserve, qstr_pathTMP, qstr_pathFFTMP;///<QString> opciones ipRserve, pathTmp para resultados, pathTMP para FF en servidor RServe
    int i_maxResMem;///<int> Máxima cantidad de residuos en memoria
    long long i_maxBytesFFLote, i_maxBytesFF;///<int> Máxima memoria en bytes ocupada por proceso FF, máxima memoria en bytes utilizada para procesos por lotes en librería FF de R
    int i_maxHilos;
};

#endif // CONFIGURACION_H
