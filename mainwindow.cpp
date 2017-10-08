#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//查找文件 按钮
//文件查找
void MainWindow::on_pushButton_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        ui->lineEdit->setText(fileName);
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file"));
    }
}

//文本 同源性检测
void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = ui->lineEdit->text();
    if(!fileName.isEmpty()){
        QByteArray ba=fileName.toLatin1();
        char *name = ba.data();
        if(!ui->checkBox->checkState()){
            QString fileName2 = ui->lineEdit_6->text();
            if(!fileName2.isEmpty()){
                QByteArray baa=fileName2.toLatin1();
                char *name2 = baa.data();
                TEXT thetext;
                double result=thetext.similar(name,name2);
                QString num=QString::number(result);
                ui->lineEdit_2->setText(num);//daichuli
            }else{
                QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file ")+fileName2);
            }
        }else{
            TEXT thetext;
            char  aimfilename[100];
            double result = thetext.similars(name,aimfilename);
            QString aimfileString = QString(QLatin1String(aimfilename));
            QString num=QString::number(result);
            ui->lineEdit_2->setText(num);//daichuli
            ui->lineEdit_3->setText(aimfileString);//待处理的地方
        }
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file ")+fileName);
    }
}

//CFG 同源性
void MainWindow::on_pushButton_3_clicked()
{
    QString fileName = ui->lineEdit->text();
    if(!fileName.isEmpty()){
        int type;
        type = ui->comboBox->currentIndex();
        QByteArray ba=fileName.toLatin1();
        char *name = ba.data();
        if(!ui->checkBox->checkState()){
            QString fileName2 = ui->lineEdit_6->text();
            if(!fileName2.isEmpty()){
                QByteArray baa=fileName2.toLatin1();
                char *name2 = baa.data();
                CFG thetext;
                int type2 = ui->comboBox_2->currentIndex();
                double result=thetext.similar(name,type+1,name2,type2+1);
                QString num=QString::number(result);
                ui->lineEdit_4->setText(num);//daichuli
            }else{
                QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file ")+fileName2);
            }
        }else{
            CFG thetext;
            char aimfilename[100];
            double result = thetext.similars(name,type+1,aimfilename);
            QString aimfileString = QString(QLatin1String(aimfilename));
            QString num=QString::number(result);
            ui->lineEdit_4->setText(num);//daichuli
            ui->lineEdit_5->setText(aimfileString);//待处理的地方
        }
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file")+fileName);
    }
}

//漏洞检测
void MainWindow::on_pushButton_4_clicked()
{
    QString fileName = ui->lineEdit->text();
    if(!fileName.isEmpty()){
        QByteArray ba=fileName.toLatin1();
        char *name = ba.data();
        BUG thetext;
        BG_t temp = thetext.find(name);//待处理的地方
        QStandardItemModel *model = new QStandardItemModel();
        model->setColumnCount(4);
        model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("Line"));
        model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Type"));
        model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("Rank"));
        model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("Name"));
        if(temp){
            int i=0;
            temp=temp->next;
            while(temp){
                model->setItem(i,0,new QStandardItem(QString::number(temp->line,10)));
                QString type;
                switch (temp->bugtype)
                {
                case HEAP:type= "heap"; break;
                case STACK:type= "stack"; break;
                case IN_LEN:type= "int_length"; break;
                case IN_OPEA:type= "int_opera"; break;
                case IN_SYM:type= "int_sym"; break;
                default:
                    break;
                }
                model->setItem(i,1,new QStandardItem(type));
                QString rankthe;
                switch (temp->rank) {
                case 'A':
                    rankthe="A";
                    break;
                case 'B':
                    rankthe="B";
                    break;
                case 'C':
                    rankthe="C";
                    break;
                default:
                    rankthe="D";
                    break;
                }
                model->setItem(i,2,new QStandardItem(rankthe));
                model->setItem(i,3,new QStandardItem( QString(QLatin1String(temp->name))));
                i++;
                temp=temp->next;
            }
        }
        ui->tableView->setModel(model);
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file"));
    }
}

//对比的查找文件
void MainWindow::on_pushButton_5_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        ui->lineEdit_6->setText(fileName);
    }else{
        QMessageBox::warning(this,tr("ByCLQ"),tr("Cannot find file"));
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    newui.show();
    //this->hide();
}
