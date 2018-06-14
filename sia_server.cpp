#include "sia_server.h"
#include <QVector>
#include <QTime>

//--------------------------------------------------------------------------------------------
//--SIA_SERVER_cl-----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
sia_server::sia_server(QObject *parent) : QObject(parent),
    uniq_key(0),
    mTcpServer(new QTcpServer(this))

{
    connect(mTcpServer, &QTcpServer::newConnection, this, &sia_server::slotNewConnection);
    if(!mTcpServer->listen(QHostAddress::Any, 1234)){
        qDebug() <<QTime::currentTime()<<"  " << "server is not started";
       // emit prinf_something("[Server: !server is not started]",0);
       // Mywnd->print_to_list(new QString("server is not started"));
    } else {
       // emit prinf_something("[Server: server is started]",0);
       // Mywnd->print_to_list(new QString("server is started"));
        qDebug() <<QTime::currentTime()<<"  " << "server is started";
    }

}

void sia_server::slotNewConnection(){
    sia_clients *clnt = new sia_clients;
    clnt->sock = mTcpServer->nextPendingConnection();
    analyse_new_client(clnt);
    connect(clnt, &sia_clients::discon_client_signal, this, &sia_server::slotClientDisconnected);
    connect(clnt->sock, &QTcpSocket::readyRead, clnt, &sia_clients::slotServerRead);
    connect(clnt->sock, &QTcpSocket::disconnected, clnt, &sia_clients::slotClientDisconnected);



    //...
    sockets.insert(uniq_key, clnt);    
    //освобождение памяти делать в соотв. месте: при дисконекте
    // delete sockets[n].second; sockets[n].erase();


    uniq_key++;

    qDebug() <<QTime::currentTime()<<"  " << "New connection! All clients = " << sockets.size();
}

void sia_server::analyse_new_client(sia_clients *clnt){
    /*поиск партнера тут*/
    clnt->uniq_key = uniq_key;
    clnt->state = connect_and_w8;

}

void sia_server::slotServerRead(){

}

void sia_server::slotClientDisconnected(const sia_clients &C){
    int siz = sockets.size();
    sockets.erase(sockets.find(C.uniq_key));

    if(sockets.size()==siz-1) qDebug() <<QTime::currentTime()<<"  " << "{!} Client disconnecting. All clients = " << sockets.size();
    else qDebug() <<QTime::currentTime()<<"  " << "[x] Socket delete error {sia_server::slotClientDisconnected}; \n All clients = " << sockets.size();
}



//--------------------------------------------------------------------------------------------
//--SIA_CLIENTS_cl----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
sia_clients::sia_clients(QObject *parent) : QObject(parent)
{
    sock = new QTcpSocket(this);
    uniq_key = 0;
    //QTcpSocket *sock;
    name = "";
    password = "";
    state = discon;

    //sock = new QTcpSocket();
}

void sia_clients::slotClientDisconnected(){
    emit discon_client_signal(*this);

}

void sia_clients::slotServerRead(){

    parse_data(sock->readAll());

    /*QByteArray Ba;
    Ba.append(":connect:" + QString::number(uniq_key) + ":");
    sock->write(Ba);*/

}

void sia_clients::parse_data(const QString &str){
    qDebug() <<QTime::currentTime()<<"  " << str;
    QRegExp rx(":(\\w+):");
    if(rx.indexIn(str)!=1){//если комманда
        //qDebug() << rx.cap(1);
        if(rx.cap(1) == "connect"){//:connect:login:passwoed:
            QRegExp con(":connect:(\\w+):(\\w+):");
            con.indexIn(str);
            name = con.cap(1);
            password = con.cap(2);
            //qDebug() << "login: "<< name;
            //qDebug() << "password: "<<password;
            QByteArray Ba;
            Ba.append(":connect:" + QString::number(uniq_key) + ":");
            sock->write(Ba); //:connect:identificator:
        }
        else if(rx.cap(1) == "field"){//:field:bitmap:
            QRegExp fld(":field:(\\d+):");
            fld.indexIn(str);
            QByteArray fld_map;
            fld_map.append(fld.cap(1));
            if(fld_map.size()==100){
                for(int r(0); r < 10; r++)
                    for(int c(0); c < 10; c++)
                        field_MAP[r][c]=fld_map[r*10 + c];
            }
            else qDebug() <<"error: client field bitmap != 100";

            //qDebug() << fld.cap(1);

        }
        else if(1) {}//доделать парсинг данных на сервере и клиенте, сделать поиск соперника
    }


}
