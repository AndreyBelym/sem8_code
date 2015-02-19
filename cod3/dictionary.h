#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QVector>
#include <QAbstractTableModel>


struct Dict{
    QChar sym;
    double prob;
    QString code;
    Dict(QChar sym,double prob):
        sym(sym),prob(prob),code("")
    {}
    Dict():
        sym('\0'),prob(0),code("")
    {}

};

void shennon_fano(QVector<Dict> &v,unsigned start,unsigned end);

void shennon_fano(QVector<Dict> &v);

void hilbert_moore(QVector<Dict> &v);

QString code(const QString &src,QVector<Dict> &v);

QString decode(const QString &src,QVector<Dict> &v);



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

QDebug operator <<(QDebug dbg,const Dict& d);

bool operator <(const Dict& d1,const Dict& d2);

bool operator ==(const Dict& d1,const QChar& c);

bool operator ==(const Dict& d1,const QString& code);

QTextStream& operator >>(QTextStream& is,Dict& d);

QTextStream& operator >>(QTextStream& is,Dictionary& d);

#endif // DICTIONARY_H
