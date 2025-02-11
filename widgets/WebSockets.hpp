#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H 1

#include <QWebSocketServer>
#include <QWebSocket>

namespace Ui {
  class WebSockets;
}


class WebSockets : public QObject
{
  public:
    explicit WebSockets(quint16 port, QObject *parent = nullptr);
  private:
    QWebSocketServer *socketServer;
//    QWebSocket *socket;
  signals:
//    void closed();
  private slots:
//    void onNewConnection();
//    void processTextMessage(QString message);
//    void socketDisconnected();
};

#endif
