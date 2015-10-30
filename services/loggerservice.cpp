#include "loggerservice.h"

#include <QTextStream>
#include <QDateTime>

LoggerService::LoggerService(QObject *parent) : QObject(parent) {
    ConfigurationService confServ;

    _pathLog = confServ.loadConfig()->workdirPath();
    _pathLog += "/scopclassification.log";

}

LoggerService::~LoggerService(){
    delete _logFile;
}

QTextStream * LoggerService::logStream(){
    _logFile = new QFile(_pathLog);

    QTextStream * logStream = NULL;

    bool logFileOpened = _logFile->open (
                QIODevice::WriteOnly|QIODevice::Append);

    if (logFileOpened) logStream = new QTextStream (_logFile);

    return logStream;
}

void LoggerService::logError(
        QString message,
        QStringList details){
    QTextStream * logId = this->logStream ();
    *logId << QDateTime::currentDateTime().toString();
    *logId << " (EE): ";
    *logId << message;
    *logId << tr("\n");

    if (! details.isEmpty ()) {
        foreach (QString detail, details) {
            *logId << qPrintable ("...."+detail+"\n");
        }
    }

    logId->flush ();
    _logFile->close ();
    delete logId;
}

void LoggerService::logInfo(
        QString message,
        QStringList details){
    QTextStream * logId = this->logStream ();
    *logId << QDateTime::currentDateTime().toString();
    *logId << " (II): ";
    *logId << message;
    *logId << tr("\n");

    if (! details.isEmpty()) {
        foreach (QString detail, details) {
            *logId << qPrintable ("...."+detail+"\n");
        }
    }

    logId->flush ();
    _logFile->close ();
    delete logId;

}

void LoggerService::logWarning(
        QString message,
        QStringList details){
    QTextStream * logId = this->logStream ();
    *logId << QDateTime::currentDateTime().toString();
    *logId << " (WW): ";
    *logId << message;
    *logId << tr("\n");

    if (! details.isEmpty()) {
        foreach (QString detail, details) {
            *logId << qPrintable ("...."+detail+"\n");
        }
    }

    logId->flush ();
    _logFile->close ();
    delete logId;

}
