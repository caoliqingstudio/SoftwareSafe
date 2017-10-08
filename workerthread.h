#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "many.h"
#include <QMessageBox>
#include "text.h"
#include "BUG.h"

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    WorkerThread();
    void * Thisargument;
protected:
    virtual void run();
private:

};

#endif // WORKERTHREAD_H
