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
            double s1=0,s2=0,s3=0;
            for(auto &i:d){
                s1+=i.code.length()*i.prob;
                s2+=pow(0.5,i.code.length());
                s3+=i.prob*log2(i.prob);
            }
            s3+=s1;
            ui->avg->setText(tr("Средняя длина слова: %1").arg(s1));
            ui->kraft->setText(tr("Неравенство Крафта %1: %2<=1").arg(s2<=1?"верно":"неверно").arg(s2));
            ui->overhead->setText(tr("Избыточность: %1").arg(s3));
        }
    }
}

void MainWindow::on_code_toggled(bool checked)
{
    if(checked){
        QString s;
        s=ui->src_path->text();
        ui->src_path->setText(ui->dst_path->text());
        ui->dst_path->setText(s);
        ui->srctext->setPlainText("");
        ui->dsttext->setPlainText("");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->src_path->setText(QFileDialog::getOpenFileName(this));
}

void MainWindow::on_decode_toggled(bool checked)
{
    if(checked){
        QString s;
        s=ui->src_path->text();
        ui->src_path->setText(ui->dst_path->text());
        ui->dst_path->setText(s);
        ui->srctext->setPlainText("");
        ui->dsttext->setPlainText("");
    }
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
            qDebug()<<ui->srctext->toPlainText();
            if(ui->code->isChecked())
                ui->dsttext->setPlainText(code(ui->srctext->toPlainText(),d));
            else
                ui->dsttext->setPlainText(decode(ui->srctext->toPlainText(),d));
            qDebug()<<decode(ui->srctext->toPlainText(),d);
            out<<ui->dsttext->toPlainText();
        }
    }
}
