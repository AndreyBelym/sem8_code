#include "dictionary.h"
#include <cmath>
Dictionary::Dictionary()
{
}

int Dictionary::rowCount ( const QModelIndex & parent = QModelIndex() ) const {
    return 2;
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


bool operator ==(const Dict& d1,const QChar& c){
    return d1.sym==c;
}


QTextStream& operator >>(QTextStream& is,Dict& d){
    QChar c; double p;
    is>>c; d.sym=c;
    ws(is);
    is>>p; d.prob=p;
    return is;
}

QTextStream& operator >>(QTextStream& is,Dictionary& d){
    Dict t;double s=0;
    if(d.size()){
        d.beginRemoveColumns(QModelIndex(),0,d.size()-1);
        d.clear();
        d.endRemoveColumns();
    }
    while(!is.atEnd()){
        is>>t>>ws;
        t.start=s;t.stop=s+t.prob;s+=t.prob;
        d.beginInsertColumns(QModelIndex(),d.size(),d.size());
        d<<t;
        d.endInsertColumns();
    }
    return is;
}

QVector<double> acode(const QString& in, QVector<Dict> &d, QString &res){
    double start=0,stop=1;
    int len=0;
    QVector<double> rs;
    QTextStream out(&res);
    out.setRealNumberPrecision(16);
    for(auto &i:in){
        if(len==12){
            rs<<start;
            len=0;
            start=0,stop=1;
        }
        auto p=qFind(d,i);
        double t;
        t=start+(stop-start)*p->start;
        stop=start+(stop-start)*p->stop;
        start=t;
        out<<"\""<<p->sym<<"\" - ["<<start<<", "<<stop<<"]\n";
        len++;
    }
    rs<<start;
    if(rs.size()==1){
        out<<"Entire text: "<<long(rs[0])<<"\n";
    }
    out<<"Entire text: {"<<"\n";
    for(auto &i:rs){
        out<<"\t"<<double(i)<<",\n";
    }
    out<<"}\n";
    return rs;
}

QString adecode(const QVector<double> a, const int n, QVector<Dict> &d){
    double gstart=0,gstop=1;
    QString res,t;
    QTextStream out(&res);
    out.setRealNumberPrecision(16);
    int len=0,J=0;

        for (int N=0;N<n;N++){
            if(len==12){
                J+=1;
                gstart=0;gstop=1;
                len=0;
            }
            for(auto &p:d){

                double start=gstart,stop=gstop;
                double tt;
                tt=start+(stop-start)*p.start;
                stop=start+(stop-start)*p.stop;
                start=tt;
                qDebug()<<gstart<<" "<<gstop;
                if((a[J]>=start)&&(a[J]<stop)){
                    t.append(p.sym);
                    out<<"["<<start<<", "<<stop<<"] - \""<<p.sym<<"\"\n";
                    gstart=start;gstop=stop;
                    break;
                }
            }
            len++;
        }

    out<<t<<"\n";
    return res;
}

