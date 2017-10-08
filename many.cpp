#include "many.h"
#include "ui_many.h"

Many::Many(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Many)
{
    standardItemModel_1=new QStandardItemModel(this);
    standardItemModel_2=new QStandardItemModel(this);
    ui->setupUi(this);
}

Many::~Many()
{
    delete ui;
}

//添加文件 1
void Many::on_pushButton_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        strList_1.append(fileName);
        QStandardItem *item = new QStandardItem(fileName);
        standardItemModel_1->appendRow(item);
        ui->listView->setModel(standardItemModel_1);
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file"));
    }
}

void Many::on_pushButton_2_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        strList_2.append(fileName);
        QStandardItem *item = new QStandardItem(fileName);
        standardItemModel_2->appendRow(item);
        ui->listView_2->setModel(standardItemModel_2);
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file"));
    }
}

void Many::on_listView_doubleClicked(const QModelIndex &index)
{
    int i=index.row();
    strList_1.removeAt(i);
    standardItemModel_1->removeRow(i);
    ui->listView->setModel(standardItemModel_1);
}

void Many::on_listView_2_doubleClicked(const QModelIndex &index)
{
    int i=index.row();
    strList_2.removeAt(i);
    standardItemModel_2->removeRow(i);
    ui->listView_2->setModel(standardItemModel_2);
}

void Many::on_pushButton_3_clicked()
{
    bool textState=ui->checkBox->isChecked();
    bool CFGState=ui->checkBox_2->isChecked();
    if(textState||CFGState){
        int num_1=strList_1.length();
        THREAD_t argument;
        argument=(THREAD_t)malloc(sizeof(THREAD)*num_1);
        int num_2=strList_2.length();
        if(!argument){
            QMessageBox::warning(this,tr("ByCLQ"),tr("No work NULL"));
            return;
        }
        for(int i=0;i<num_1;++i){
            argument[i].state=false;
            argument[i].similarText=0;
            argument[i].AimfilenameText=NULL;
            argument[i].similarCFG=0;
            argument[i].AimfilenameCFG=NULL;
            argument[i].text=textState;
            argument[i].cfg=CFGState;
            argument[i].filename=&(strList_1.at(i));
            argument[i].aimfileNameList=&strList_2;
            argument[i].aimfileNum=num_2;
            //QMessageBox::warning(this,tr("ByCLQ"),tr("No work  wfa"));
            //thiswork.start();
            //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)similarRUN, (LPVOID)&argument[i], 0, NULL);
        }
        WorkerThread **work;
        work=(WorkerThread **)malloc(sizeof(WorkerThread *)*num_1);
        for(int i=0;i<num_1;++i){
            work[i]=new WorkerThread;
            work[i]->Thisargument=(void *)&argument[i];
            work[i]->start();
        }
        bool *state;
        state=(bool *)malloc(sizeof(bool)*num_1);
        for(int i=0;i<num_1;++i){
            state[i]=true;
        }
        int num=0;
        QStandardItemModel *model = new QStandardItemModel();
        model->setColumnCount(5);
        model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("FileName"));
        model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("MaxTEXT"));
        model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("MaxFileTEXT"));
        model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("MaxCFG"));
        model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("MaxFileCFG"));
        while(num<num_1){
            std::cout<<num<<"waiceng"<<std::endl;
            for(int i=0;i<num_1;++i){
                std::cout<<i<<"neiceng"<<std::endl;
                if(state[i]){
                    if(argument[i].state){
                        state[i]=false;
                        model->setItem(num,0,new QStandardItem(*(argument[i].filename)));
                        if(textState){
                            QString result=QString::number(argument[i].similarText);
                            model->setItem(num,1,new QStandardItem(result));
                            model->setItem(num,2,new QStandardItem(*(argument[i].AimfilenameText)));
                        }
                        if(CFGState){
                            QString result=QString::number(argument[i].similarCFG);
                            model->setItem(num,3,new QStandardItem(result));
                            model->setItem(num,4,new QStandardItem(*(argument[i].AimfilenameCFG)));
                        }
                        num++;
                    }
                }
            }
        }
        free(argument);
        ui->tableView->setModel(model);
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("No work"));
    }
}

//void Many::similarRUN(LPVOID argue){
//    THREAD_t argument=(THREAD_t)argue;
//    argument->state=true;
//    argument->AimfilenameCFG="xixihaha";
//    argument->similarCFG=0.25;
//}
