#include "runthread.h"
using namespace std;

RunThread::RunThread(int type, QString attack, QString hashlist, long long skip, long long length, int timeout){
    this->type = type;
    this->attack = attack;
    this->skip = skip;
    this->length = length;
    this->hashlist = hashlist;
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
    BcryptHash *hashLoading = NULL;
    while( !in.atEnd()){
        line = in.readLine();
        if(line.length() == 0){
            continue;
        }
        else if(line.length() != 60){
            continue;
        }
        hashLoading = new BcryptHash();
        hashLoading->original = line;
        strcpy(hashLoading->salt, line.mid(0, 29).toStdString().c_str());
        strcpy(hashLoading->hash, line.mid(29).toStdString().c_str());
        hashes.append(hashLoading);
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
    atomic<long long int> lengthCounter(0);
    atomic_llong crackedCounter(0);
    time_t lastUpdate = time(NULL);
    atomic_llong lastCounter(0);
    time_t startTime = time(NULL);
    atomic_llong hashingCounter(0);
    QMutex mutex;
    QList<CoreThread*> threads;
    for(int i=0;i<8;i++){
        CoreThread *t = new CoreThread(&lengthCounter, &crackedCounter, &hashingCounter, timeout, startTime, this->length, this, &this->hashes, &mutex);
        threads.append(t);
        t->start();
    }

    bool run = true;
    while(run){
        usleep(50000);
        run = false;
        for(CoreThread *t: threads){
            if(t->isRunning()){
                run = true;
                break;
            }
        }

        if(time(NULL) - lastUpdate >= 5){
            // show update
            cout << "STATUS " << (int)floor((double)lengthCounter/this->length*10000) << " " << (int)(((double)(hashingCounter - lastCounter))/(time(NULL) - lastUpdate)) << endl;
            lastCounter.store(hashingCounter);
            lastUpdate = time(NULL);
        }
        else if(lengthCounter >= this->length){
            break; // we reached length limit
        }
        else if(timeout > 0 && time(NULL) - startTime > timeout){
            cout << "STATUS " << (int)floor((double)lengthCounter/this->length*10000) << " " << (int)(((double)(hashingCounter - lastCounter))/(time(NULL) - lastUpdate)) << endl;
            return;
        }
    }
    for(CoreThread *t: threads){
        t->wait();
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
