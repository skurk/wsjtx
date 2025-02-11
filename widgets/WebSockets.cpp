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
  conn = socketServer->nextPendingConnection();
  connect(conn, &QWebSocket::textMessageReceived,
          this, &WebSockets::textMessageReceived);
  connect(conn, &QWebSocket::disconnected,
          this, &WebSockets::disconnected);
}

void WebSockets::writeToClient(QString message)
{
  if(conn)
  {
    conn->sendTextMessage(message);
    conn->flush();
  }
}

void WebSockets::textMessageReceived(QString message)
{
  printf("Message received: %s\n", message.toStdString().c_str());
//  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  if(conn)
  {
    if(message == "Event:Button:autoButton")
    {
      emit autoButtonClicked();
    }
    if(message == "Event:Button:haltButton")
    {
      emit haltButtonClicked();
    }
    conn->sendTextMessage("Ok");
    conn->flush();
  }
}

void WebSockets::disconnected()
{
//  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  conn->deleteLater();
}

