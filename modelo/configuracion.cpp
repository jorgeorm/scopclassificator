#include "configuracion.h"
#include <QVariant>
#include <QJsonValue>

Configuracion::Configuracion(){
}

Configuracion::Configuracion(QString _qstr_ipRserve,
                             QString _qstr_pathTMP,
                             QString _qstr_pathFFTMP,
                             int _i_maxBytesFF,
                             int _i_maxResMem,
                             int _i_maxBytesFFLote,
                             int _i_numHilos){
    qstr_ipRserve = _qstr_ipRserve;
    qstr_pathTMP = _qstr_pathTMP;
    qstr_pathFFTMP = _qstr_pathFFTMP;

    i_maxBytesFF = _i_maxBytesFF;
    i_maxResMem = _i_maxResMem;
    i_maxBytesFFLote = _i_maxBytesFFLote;
}

void Configuracion::escribirConfig(QJsonObject &jsonObj){
    jsonObj["ipRserve"] = qstr_ipRserve;
    jsonObj["pathTmp"] = qstr_pathTMP;
    jsonObj["pathFFtmp"] = qstr_pathFFTMP;
    jsonObj["maxBytesFF"] = QJsonValue::fromVariant(QVariant(i_maxBytesFF));
    jsonObj["maxResidMem"] = i_maxResMem;
    jsonObj["maxBytesFFlote"] = QJsonValue::fromVariant(QVariant(i_maxBytesFFLote));
    jsonObj["maxHilos"] = i_maxHilos;
}

//TODO: Cambiar tipo de dato long long a qlonglong o algo por el estilo
void Configuracion::leerConfig(const QJsonObject &jsonObj){
    qstr_ipRserve = jsonObj["ipRserve"].toString();
    qstr_pathTMP = jsonObj["pathTmp"].toString();
    qstr_pathFFTMP = jsonObj["pathFFtmp"].toString();
    i_maxBytesFF = jsonObj["maxBytesFF"].toVariant().toLongLong();
    i_maxResMem = jsonObj["maxResidMem"].toInt();
    i_maxBytesFFLote  = jsonObj["maxBytesFFlote"].toVariant().toLongLong();
    i_maxHilos = jsonObj["maxHilos"].toInt();
}


//GETTERS Y SETTERS
QString Configuracion::getQstr_ipRserve() const{
    return qstr_ipRserve;
}
void Configuracion::setQstr_ipRserve(const QString &value){
    qstr_ipRserve = value;
}

QString Configuracion::getQstr_pathTMP() const{
    return qstr_pathTMP;
}
void Configuracion::setQstr_pathTMP(const QString &value){
    qstr_pathTMP = value;
}

QString Configuracion::getQstr_pathFFTMP() const{
    return qstr_pathFFTMP;
}
void Configuracion::setQstr_pathFFTMP(const QString &value){
    qstr_pathFFTMP = value;
}

long long Configuracion::getI_maxBytesFF() const{
    return i_maxBytesFF;
}
void Configuracion::setI_maxBytesFF(long long value){
    i_maxBytesFF = value;
}

long long Configuracion::getI_maxBytesFFLote() const{
    return i_maxBytesFFLote;
}
void Configuracion::setI_maxBytesFFLote(long long value){
    i_maxBytesFFLote = value;
}

int Configuracion::getI_maxResMem() const{
    return i_maxResMem;
}
void Configuracion::setI_maxResMem(int value){
    i_maxResMem = value;
}
int Configuracion::getI_maxHilos() const
{
    return i_maxHilos;
}

void Configuracion::setI_maxHilos(int value)
{
    i_maxHilos = value;
}


