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
    QString combo;
    long long int lengthCounter = 0;
    long long int crackedCounter = 0;
    time_t lastUpdate = time(NULL);
    long long int lastCounter = 0;
    time_t startTime = time(NULL);
    int m = 0;
    long long int hashingCounter = 0;
    while(this->getNext(combo, lengthCounter)){
        // try to verify every hash of combo
        for(BcryptHash *h: this->hashes){
            m = BCrypt::validatePassword(combo.toStdString(), h->original.toStdString());
            hashingCounter++;
            if(m == 1){
                // found
                cout << h->original.toStdString() << ":" << combo.toStdString() << endl;
                delete h;
                this->hashes.removeOne(h);
                crackedCounter++;
            }

            if(time(NULL) - lastUpdate >= 5){
                // show update
                cout << "STATUS " << (int)floor((double)lengthCounter/this->length*10000) << " " << (int)(((double)(hashingCounter - lastCounter))/(time(NULL) - lastUpdate)) << endl;
                lastCounter = hashingCounter;
                lastUpdate = time(NULL);
            }
        }

        lengthCounter++;
        if(lengthCounter >= this->length){
            break; // we reached length limit
        }
        else if(timeout > 0 && time(NULL) - startTime > timeout){
            cout << "STATUS " << (int)floor((double)lengthCounter/this->length*10000) << " " << (int)(((double)(hashingCounter - lastCounter))/(time(NULL) - lastUpdate)) << endl;
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
