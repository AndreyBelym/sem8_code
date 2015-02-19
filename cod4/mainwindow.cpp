#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVector>
#include <QChar>
#include <QFile>
#include <QFileDialog>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(&d);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString path=QFileDialog::getOpenFileName(this);
    if(path!=""){

        QFile f(path);
        if(f.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream in(&f);
            in>>d;
            ui->probab_path->setText(path);
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->src_path->setText(QFileDialog::getOpenFileName(this));
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->dst_path->setText(QFileDialog::getSaveFileName(this));
}

void MainWindow::on_pushButton_4_clicked()
{
    if(d.size()>0){

        QFile fin(ui->src_path->text()),fout(ui->dst_path->text());
        if(fin.open(QIODevice::ReadOnly|QIODevice::Text)&&fout.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream in(&fin),out(&fout);
            ui->srctext->setPlainText(in.readAll());
            QString tmp;
            QVector<double> code=acode(ui->srctext->toPlainText(),d,tmp);
            ui->dsttext->setPlainText(tmp);
            ui->dcdtext->setPlainText(adecode(code,ui->srctext->toPlainText().length(),d));
            out<<ui->dsttext->toPlainText();
            out<<ui->dcdtext->toPlainText();
        }
    }
}
