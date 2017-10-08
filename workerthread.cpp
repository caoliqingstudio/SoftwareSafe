#include "workerthread.h"

WorkerThread::WorkerThread()
{
}

void WorkerThread::run(){
    THREAD_t argument =(THREAD_t) Thisargument;
    QByteArray ba=(argument->filename)->toLatin1();
    char *name = ba.data();
    if(argument->text){
        TEXT thisFun;
        QString * thisname;
        int length=(argument->aimfileNameList)->length();
        double maxtext=0;
        for(int i=0;i<length;i++){
            QByteArray baa=((argument->aimfileNameList)->at(i)).toLatin1();
            char *aimname = baa.data();
            double result = thisFun.similar(name,aimname);
            if(maxtext<result){
                maxtext=result;
                thisname=(QString *)&(argument->aimfileNameList->at(i));
            }
        }
        argument->AimfilenameText=thisname;
        argument->similarText=maxtext;
    }
    if(argument->cfg){
        CFG thisFun;
        QString * thisname;
        int length=(argument->aimfileNameList)->length();
        double maxtext=0;
        for(int i=1;i<length;i++){
            QByteArray baa=(argument->aimfileNameList->at(i)).toLatin1();
            char *aimname = baa.data();
            double result = thisFun.similar(name,1,aimname,1);
            if(maxtext<result){
                maxtext=result;
                thisname=(QString *)&(argument->aimfileNameList->at(i));
            }
        }
        argument->AimfilenameCFG=thisname;
        argument->similarCFG=maxtext;
    }
    argument->state=true;
    //argument->AimfilenameCFG="xixihaha";
    //argument->similarCFG=0.25;
    //const QString filename =argument->filename;
   // std::cout<<filename.toStdString()<<std::endl;
}
