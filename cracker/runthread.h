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
#include <QMutex>

#include "libbcrypt-master/include/bcrypt/bcrypt.h"
#include "libbcrypt-master/include/bcrypt/BCrypt.hpp"

class RunThread;

#include "corethread.h"

class RunThread : public QThread
{
    Q_OBJECT
public:
    RunThread(int type, QString attack, QString hashlist, long long int skip, long long int length, int timeout);
    bool getNext(QString &combo, long long pos);

private:
    int type;
    QString attack;
    long long int skip;
    long long int length;
    QString hashlist;
    int timeout;

    QFile *wordlistFile;
    QTextStream *inputStream;

    QList<BcryptHash*> hashes;

    void run() override;
    int gotoSkipFile();
};

#endif // RUNTHREAD_H
