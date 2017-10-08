#ifndef MANY_H
#define MANY_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QList>
#include <QStandardItemModel>
#include <QProcess>
#include "workerthread.h"

typedef struct{
    bool text;
    bool cfg;
    bool state;
    int aimfileNum;
    const QStringList *aimfileNameList;
    const QString *filename;
    QString *AimfilenameText;
    QString *AimfilenameCFG;
    double similarText;
    double similarCFG;
}THREAD,*THREAD_t;

namespace Ui {
class Many;
}

class Many : public QMainWindow
{
    Q_OBJECT

public:
    explicit Many(QWidget *parent = 0);
    ~Many();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_2_doubleClicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

private:
   // static void similarRUN(LPVOID argue);
    QStandardItemModel *standardItemModel_1;
    QStandardItemModel *standardItemModel_2;
    QStringList strList_1;
    QStringList strList_2;
    Ui::Many *ui;
};

#endif // MANY_H
