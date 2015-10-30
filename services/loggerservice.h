#ifndef LOGGERSERVICE_H
#define LOGGERSERVICE_H

#include "configurationservice.h"

#include <QObject>
#include <QTextStream>

class LoggerService : public QObject
{
    Q_OBJECT
public:
    explicit LoggerService(QObject *parent = 0);
    virtual ~LoggerService();

    void logError(QString message,
                  QStringList details = QStringList());
    void logInfo(QString message,
                 QStringList details = QStringList());
    void logWarning(QString message,
                    QStringList details = QStringList());
signals:

public slots:

protected:
    QTextStream *logStream();
private:
    QString _pathLog;
    QFile * _logFile;
};

#endif // LOGGERSERVICE_H
