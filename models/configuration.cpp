#include "configuration.h"

Configuration::Configuration(QObject *parent): QObject(parent)
{

}

Configuration::Configuration(QString ipRserve,
                             QString workdirPath,
                             QString ffWorkdirPath,
                             qlonglong maxBytesFF,
                             qlonglong maxBytesFBatch,
                             int maxResiduesInLocalMem,
                             int maxSimThread) {

    _ipRserve = ipRserve;
    _workdirPath = workdirPath;
    _ffWorkdirPath = ffWorkdirPath;
    _maxBytesFF = maxBytesFF;
    _maxBytesFBatch = maxBytesFBatch;
    _maxResiduesInLocalMem = maxResiduesInLocalMem;
    _maxSimThread = maxSimThread;
}

QString Configuration::ipRserve() const
{
    return _ipRserve;
}

void Configuration::setIpRserve(const QString &ipRserve)
{
    _ipRserve = ipRserve;
}

QString Configuration::workdirPath() const
{
    return _workdirPath;
}

void Configuration::setWorkdirPath(const QString &workdirPath)
{
    _workdirPath = workdirPath;
}

QString Configuration::ffWorkdirPath() const
{
    return _ffWorkdirPath;
}

void Configuration::setFfWorkdirPath(const QString &ffWorkdirPath)
{
    _ffWorkdirPath = ffWorkdirPath;
}

long long Configuration::maxBytesFF() const
{
    return _maxBytesFF;
}

void Configuration::setMaxBytesFF(long long maxBytesFF)
{
    _maxBytesFF = maxBytesFF;
}

long long Configuration::maxBytesFBatch() const
{
    return _maxBytesFBatch;
}

void Configuration::setMaxBytesFBatch(long long maxBytesFBatch)
{
    _maxBytesFBatch = maxBytesFBatch;
}

int Configuration::maxSimThread() const
{
    return _maxSimThread;
}

void Configuration::setMaxSimThread(int maxSimThread)
{
    _maxSimThread = maxSimThread;
}

int Configuration::maxResiduesInLocalMem() const
{
    return _maxResiduesInLocalMem;
}

void Configuration::setMaxResiduesInLocalMem(int maxResiduesInLocalMem)
{
    _maxResiduesInLocalMem = maxResiduesInLocalMem;
}

