#ifndef SIA_SERVER_H
#define SIA_SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QMap>

enum SOCKET_STATE{discon=0,connect_and_game=1, connect_and_w8=2};
class sia_server;
class sia_clients;
//--------------------------------------------------------------------------------------------
//--SIA_SERVER_cl-----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

class sia_server : public QObject
{
    Q_OBJECT
private:
    int uniq_key;
    QTcpServer * mTcpServer;
    QMap<quint32,sia_clients*> sockets;

    void analyse_new_client(sia_clients *clnt);

public:
    explicit sia_server(QObject *parent = nullptr);

signals:

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected(const sia_clients &C);
};

//--------------------------------------------------------------------------------------------
//--SIA_CLIENTS_cl----------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

class sia_clients : public QObject
{
    Q_OBJECT
private:
    void parse_data(const QString& str );

signals:
    void discon_client_signal(const sia_clients &C);

public:

    explicit sia_clients(QObject *parent = nullptr);


    quint32 uniq_key;
    QTcpSocket *sock;
    QString name;
    QString password;
    SOCKET_STATE state;

    int field_MAP[10][10];

public slots:
    void slotClientDisconnected();
    void slotServerRead();


};

#endif // SIA_SERVER_H
