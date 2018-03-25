#include "keyspacethread.h"
#include <utility>
using namespace std;

KeyspaceThread::KeyspaceThread(int type, QString value){
    this->type = type;
    this->value = std::move(value);
}

void KeyspaceThread::run(){
    if(this->type != 1 && this->type != 2){
        cerr << "Invalid attack type!" << endl;
        return;
    }
    else if(this->value.length() == 0){
        cerr << "Attack type with invalid input!" << endl;
        return;
    }

    if(this->type == 2){
        QFile file(this->value);
        if(!file.exists()){
            cerr << "Wordlist file does not exist: " << this->value.toStdString() << endl;
            return;
        }
        long long int counter=0;
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        while( !in.atEnd()){
            in.readLine();
            counter++;
        }
        file.close();
        cout << counter << endl;
    }
    else{
        // TODO: mask attack keyspace
    }
}
