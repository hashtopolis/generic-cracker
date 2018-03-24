#include "runthread.h"
#include <utility>
using namespace std;

RunThread::RunThread(int type, QString attack, QString hashlist, long long skip, long long length, int timeout){
    this->type = type;
    this->attack = std::move(attack);
    this->skip = skip;
    this->length = length;
    this->hashlist = std::move(hashlist);
    this->timeout = timeout;
}

void RunThread::run(){
    if(this->type != 1 && this->type != 2){
        cerr << "Invalid attack type!" << endl;
        return;
    }
    else if(this->attack.length() == 0){
        cerr << "Attack type with invalid input!" << endl;
        return;
    }
    else if(this->hashlist.length() == 0){
        cerr << "No hashlist provided!" << endl;
        return;
    }

    // load hashes
    //qDebug() << "Loading hashes...";
    QFile file(this->hashlist);
    if(!file.exists()){
        cerr << "Hashlist file does not exist: " << this->hashlist.toStdString() << endl;
        return;
    }
    long long int hashCounter=0;
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString line;
    while( !in.atEnd()){
        line = in.readLine();
        if(line.length() == 0){
            continue;
        }
        this->hashes.append(line);
        hashCounter++;
    }
    file.close();
    //qDebug() << hashCounter << "hashes loaded!";

    if(this->type == 2){
        //qDebug() << "Open and prepare wordlist!";
        this->wordlistFile = new QFile(this->attack);
        if(!this->wordlistFile->exists()){
            cerr << "Wordlist file does not exist: " << this->attack.toStdString() << endl;
            return;
        }
        this->wordlistFile->open(QIODevice::ReadOnly);
        this->inputStream = new QTextStream(this->wordlistFile);
        if(this->skip > 0){
            if(this->gotoSkipFile() != 0){
                return;
            }
        }
    }

    //qDebug() << "Starting attack...";
    QString combo;
    QString hash;
    long long int lengthCounter = 0;
    long long int crackedCounter = 0;
    time_t lastUpdate = time(NULL);
    long long int lastCounter = 0;
    time_t startTime = time(NULL);
    while(this->getNext(combo, lengthCounter)){
        // calculate hash of combo
        hash = QString(QCryptographicHash::hash(combo.toUtf8(), QCryptographicHash::Md5).toHex());

        // check if in hashlist
        if(this->hashes.contains(hash)){
            // found
            cout << hash.toStdString() << ":" << combo.toStdString() << endl;
            this->hashes.removeOne(hash);
            crackedCounter++;
        }

        if(time(NULL) - lastUpdate >= 5){
            // show update
            cout << "STATUS " << (int)floor((double)lengthCounter/this->length*10000) << " " << (int)(((double)(lengthCounter - lastCounter))/(time(NULL) - lastUpdate)) << endl;
            lastCounter = lengthCounter;
            lastUpdate = time(NULL);
        }

        lengthCounter++;
        if(lengthCounter >= this->length){
            break; // we reached length limit
        }
        else if(timeout > 0 && time(NULL) - startTime > timeout){
            cout << "STATUS " << (int)floor((double)lengthCounter/this->length*10000) << " " << (int)(((double)(lengthCounter - lastCounter))/(time(NULL) - lastUpdate)) << endl;
            return;
        }
    }
    cout << "STATUS 10000 0" << endl;
    //qDebug() << "Finished!";
    //qDebug() << "Cracked:" << crackedCounter;
}

int RunThread::gotoSkipFile(){
    for(long long int i=0;i<this->skip;i++){
        if(this->inputStream->atEnd()){
            cerr << "Skip value too large for wordlist!" << endl;
            return -1;
        }
        this->inputStream->readLine();
    }
    return 0;
}

bool RunThread::getNext(QString &combo, long long int pos){
    if(this->type == 1){
        // TODO mask combinations
        return false;
    }
    else{
        if(this->inputStream->atEnd()){
            cerr << "Wordlist not large enough to satisfy length!" << endl;
            return false;
        }
        combo = this->inputStream->readLine();
        return true;
    }
}
