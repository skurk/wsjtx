#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H 1

#include <QWebSocketServer>
#include <QWebSocket>
#include <QMap>

namespace Ui {
  class WebSockets;
}


class WebSockets : public QObject
{
  Q_OBJECT;

  public:
    explicit WebSockets(quint16 port, QObject *parent = nullptr);
    ~WebSockets();
    void closeServer();
    void closeAllConnections();
    void writeToClient(QString message);
    QMap<QString,QString> wsjtx_settings;
  private:
    QWebSocketServer *socketServer;
    QWebSocket *current;
    QList<QWebSocket *> clients;
  public slots:
    void sendLocalEvent(QString, QString);
  signals:
    void closed();
    void sendRemoteEvent(QString);
    void autoButtonClicked();
    void haltButtonClicked();
    void settingsRequested();
    void setTxFreq(int txf);
    void setRxFreq(int rxf);
  private slots:
    void onNewConnection();
    void textMessageReceived(QString message);
    void socketDisconnected();
};

#endif
