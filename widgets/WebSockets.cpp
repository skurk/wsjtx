#include <QWebSocketServer>
#include <QWebSocket>
#include "WebSockets.hpp"

WebSockets::WebSockets(quint16 port, QObject *parent) :
           QObject(parent),
           socketServer(new QWebSocketServer("Websocket server",
                                             QWebSocketServer::NonSecureMode,
                                             this))
{

  if(socketServer->listen(QHostAddress::Any, port))
  {
    printf("WebSocket constructor: init port=%d\n", (int)port);
    connect(socketServer, &QWebSocketServer::newConnection,
            this, &WebSockets::onNewConnection);
  }
}

void WebSockets::onNewConnection()
{
  QWebSocket *ws = socketServer->nextPendingConnection();
  connect(ws, &QWebSocket::textMessageReceived,
          this, &WebSockets::textMessageReceived);
  connect(ws, &QWebSocket::disconnected,
          this, &WebSockets::disconnected);
}

void WebSockets::textMessageReceived(QString message)
{
  printf("Message received: %s\n", message.toStdString().c_str());
  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  if(c)
  {
    c->sendTextMessage("Ok");
    c->flush();
  }
}

void WebSockets::disconnected()
{
  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  c->deleteLater();
}

