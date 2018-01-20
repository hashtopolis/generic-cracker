#ifndef KEYSPACETHREAD_H
#define KEYSPACETHREAD_H

#include <QString>
#include <iostream>
#include <QThread>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

class KeyspaceThread : public QThread
{
    Q_OBJECT
public:
    KeyspaceThread(int type, QString value);

private:
    int type;
    QString value;

    void run();
};

#endif // KEYSPACETHREAD_H
