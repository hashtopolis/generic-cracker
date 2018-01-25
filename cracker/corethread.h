#ifndef CORETHREAD_H
#define CORETHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QList>

#include <iostream>

#include "libbcrypt-master/include/bcrypt/BCrypt.hpp"

struct BcryptHash {
    QString original;
    int cost;
    bool found = false;
    char salt[BCRYPT_HASHSIZE];
    char hash[BCRYPT_HASHSIZE];
};

#include "runthread.h"

using namespace std;

class CoreThread : public QThread{
    Q_OBJECT
public:
    explicit CoreThread(atomic_llong *lengthCounter, atomic_llong *crackedCounter, atomic_llong *hashingCounter, int timeout, int startTime, long long int length, RunThread *parent, QList<BcryptHash*> *hashes, QMutex *mutex);

private:
    void run();

    atomic_llong *lengthCounter;
    atomic_llong *crackedCounter;
    atomic_llong *hashingCounter;
    int timeout;
    int startTime;
    long long int length;
    RunThread *parent;
    QList<BcryptHash*> *hashes;
    QMutex *mutex;
};

#endif // CORETHREAD_H
