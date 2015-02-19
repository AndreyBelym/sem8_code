#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QVector>
#include <QAbstractTableModel>


struct Dict{
    QChar sym;
    double prob,start,stop;
    Dict(QChar sym,double prob):
        sym(sym),prob(prob),start(0),stop(prob)
    {}
    Dict():
        sym('\0'),prob(0),start(0),stop(prob)
    {}

};


class Dictionary:public QVector<Dict>,public QAbstractTableModel
{
public:
    Dictionary();
    int rowCount ( const QModelIndex & parent) const ;
    int columnCount ( const QModelIndex & parent ) const;
    QVariant data ( const QModelIndex & index, int role ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags ( const QModelIndex & index ) const;

    friend QTextStream& operator >>(QTextStream& is,Dictionary& d);
};

bool operator ==(const Dict& d1,const QChar& c);

QTextStream& operator >>(QTextStream& is,Dict& d);

QTextStream& operator >>(QTextStream& is,Dictionary& d);

QVector<double> acode(const QString& in,QVector<Dict> &d,QString &res);
QString adecode(const QVector<double> a,const int n,QVector<Dict> &d);
#endif // DICTIONARY_H
