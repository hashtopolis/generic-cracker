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

class RunThread : public QThread
{
    Q_OBJECT
public:
    RunThread(int type, QString attack, QString hashlist, long long int skip, long long int length);

private:
    int type;
    QString attack;
    long long int skip;
    long long int length;
    QString hashlist;

    QFile *wordlistFile;
    QTextStream *inputStream;

    QList<QString> hashes;

    void run();
    int gotoSkipFile();
    bool getNext(QString &combo, long long pos);
};

#endif // RUNTHREAD_H
