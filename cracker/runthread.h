#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include <QObject>
#include <iostream>
#include <QThread>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QDebug>
#include <cmath>

class RunThread : public QThread
{
    Q_OBJECT
public:
    RunThread(int type, QString attack, QString hashlist, long long int skip, long long int length, int timeout);

private:
    int type;
    QString attack;
    long long int skip;
    long long int length;
    QString hashlist;
    int timeout;

    QFile *wordlistFile;
    QTextStream *inputStream;

    QList<QString> hashes;

    void run() override;
    int gotoSkipFile();
    bool getNext(QString &combo, long long pos);
};

#endif // RUNTHREAD_H
