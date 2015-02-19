#include "dictionary.h"

Dictionary::Dictionary()
{
}

int Dictionary::rowCount ( const QModelIndex & parent = QModelIndex() ) const {
    return 3;
}

int Dictionary::columnCount ( const QModelIndex & parent = QModelIndex() ) const{
    return size();
}

QVariant Dictionary::data ( const QModelIndex & index, int role = Qt::DisplayRole ) const{
    switch(role){
        case Qt::DisplayRole:
            if(index.column()<size()){
                switch(index.row()){
                case 0:
                    return QVariant(this->at(index.column()).sym);
                case 1:
                    return QVariant(this->at(index.column()).prob);
                case 2:
                    return QVariant(this->at(index.column()).code);
                default:
                    return QVariant(QVariant::Invalid);
                }
            } else
                return QVariant(QVariant::Invalid);
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignRight|Qt::AlignVCenter);
        default:
            return QVariant(QVariant::Invalid);
    }


}

QVariant Dictionary::headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const{
    switch(role){
        case Qt::DisplayRole:
            if(orientation==Qt::Horizontal){
                return QVariant(section+1);
            } else if (orientation==Qt::Vertical){
                switch(section){
                case 0:
                    return QVariant(tr("Символ"));
                case 1:
                    return QVariant(tr("Вероятность"));
                case 2:
                    return QVariant(tr("Код"));
                default:
                    return QVariant(QVariant::Invalid);
                }
            } else
                return QVariant(QVariant::Invalid);
        default:
            return QVariant(QVariant::Invalid);
    }


}

Qt::ItemFlags Dictionary::flags ( const QModelIndex & index ) const{
    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QDebug operator <<(QDebug dbg,const Dict& d){
    dbg.space()<<"char:"<<d.sym<<"code:"<<d.code;
    return dbg.space();
}
bool operator <(const Dict& d1,const Dict& d2){
    return d1.prob>d2.prob||(d1.prob==d2.prob&&d1.sym<d2.sym);
}

bool operator ==(const Dict& d1,const QChar& c){
    return d1.sym==c;
}

bool operator ==(const Dict& d1,const QString& code){
    return d1.code==code;
}

QTextStream& operator >>(QTextStream& is,Dict& d){
    QChar c; double p;
    is>>c; d.sym=c;
    ws(is);
    is>>p; d.prob=p;
    return is;
}

QTextStream& operator >>(QTextStream& is,Dictionary& d){
    Dict t;
    if(d.size()){
        d.beginRemoveColumns(QModelIndex(),0,d.size()-1);
        d.clear();
        d.endRemoveColumns();
    }
    while(!is.atEnd()){
        is>>t>>ws;
        d.beginInsertColumns(QModelIndex(),d.size(),d.size());
        d<<t;
        d.endInsertColumns();
    }
    shennon_fano(d);
    return is;
}

void shennon_fano(QVector<Dict> &v,unsigned start,unsigned end){
    double p=0,s=0;
    int n=start;
    for(unsigned i=start;i<end;i++){
        p+=v[i].prob;
    }
    for(unsigned i=start;i<end&&s<p/2;i++){
        v[i].code+='1';
        s+=v[i].prob;
        n++;
    }
    for(unsigned i=n;i<end;i++){
        v[i].code+='0';
    }
    if(n-start>1)
        shennon_fano(v,start,n);
    if(end-n>1)
        shennon_fano(v,n,end);
}

void shennon_fano(QVector<Dict> &v){
    qStableSort(v);
    shennon_fano(v,0,v.length());
}
QString code(const QString &src,QVector<Dict> &v){
    QString s;
    for(auto &i:src){
        auto p=qFind(v,i);
        s+=p->code;
    }
    return s;
}

QString decode(const QString &src,QVector<Dict> &v){
    QString s,t;
    for(auto &i:src){
        t+=i;
        auto p=qFind(v,t);
        if(p!=v.end()){
            s+=p->sym;
            t="";
        }
    }
    return s;
}
