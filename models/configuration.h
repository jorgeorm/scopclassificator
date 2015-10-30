#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QString>



class Configuration : QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ipRserve READ ipRserve WRITE setIpRserve NOTIFY ipRserveChanged)
    Q_PROPERTY(QString workdirPath READ workdirPath WRITE setWorkdirPath NOTIFY workdirPathChanged)
    Q_PROPERTY(QString ffWorkdirPath READ ffWorkdirPath WRITE setFfWorkdirPath NOTIFY ffWorkdirPathChanged)
    Q_PROPERTY(long long maxBytesFF READ maxBytesFF WRITE setMaxBytesFF NOTIFY maxBytesFFChanged)
    Q_PROPERTY(long long maxBytesFBatch READ maxBytesFBatch WRITE setMaxBytesFBatch NOTIFY maxBytesFBatchChanged)
    Q_PROPERTY(int maxSimThread READ maxSimThread WRITE setMaxSimThread NOTIFY maxSimThreadChanged)
    Q_PROPERTY(int maxResiduesInLocalMem READ maxResiduesInLocalMem WRITE setMaxResiduesInLocalMem NOTIFY maxResiduesInLocalMemChanged)

public:
    explicit Configuration(QObject *parent = 0);

    Configuration(
            QString ipRserve,
            QString workdirPath,
            QString ffWorkdirPath,
            qlonglong maxBytesFF,
            qlonglong maxBytesFBatch,
            int maxResiduesInLocalMem,
            int maxSimThread);

    QString ipRserve() const;
    void setIpRserve(const QString &ipRserve);

    QString workdirPath() const;
    void setWorkdirPath(const QString &workdirPath);

    QString ffWorkdirPath() const;
    void setFfWorkdirPath(const QString &ffWorkdirPath);

    long long maxBytesFF() const;
    void setMaxBytesFF(long long maxBytesFF);

    long long maxBytesFBatch() const;
    void setMaxBytesFBatch(long long maxBytesFBatch);

    int maxSimThread() const;
    void setMaxSimThread(int maxSimThread);

    int maxResiduesInLocalMem() const;
    void setMaxResiduesInLocalMem(int maxResiduesInLocalMem);

signals:
    void ipRserveChanged ();
    void workdirPathChanged ();
    void ffWorkdirPathChanged ();
    void maxBytesFFChanged ();
    void maxBytesFBatchChanged ();
    void maxSimThreadChanged ();
    void maxResiduesInLocalMemChanged ();
public slots:

private:
    QString _ipRserve, _workdirPath, _ffWorkdirPath;
    long long _maxBytesFF, _maxBytesFBatch;
    int _maxSimThread, _maxResiduesInLocalMem;
};

#endif // CONFIGURATION_H
