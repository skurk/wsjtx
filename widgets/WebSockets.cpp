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

WebSockets::~WebSockets()
{
  socketServer->close();
}

void WebSockets::onNewConnection()
{
  QWebSocket *conn = socketServer->nextPendingConnection();
  current = conn;
  connect(conn, &QWebSocket::textMessageReceived,
          this, &WebSockets::textMessageReceived);
  connect(conn, &QWebSocket::disconnected,
          this, &WebSockets::disconnected);

  clients << conn;
}

void WebSockets::writeToClient(QString message)
{
  foreach(QWebSocket *c,  clients)
  {
//    QWebSocket *c = qobject_cast<QWebSocket *>(sender());
    if(c)
    {
      c->sendTextMessage(message);
      c->flush();
    }
    else
    {
      printf("Unable to send to client (socket not open)\n");
    }
  }
}

void WebSockets::textMessageReceived(QString message)
{
  printf("Message received: %s\n", message.toStdString().c_str());
  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  if(c)
  {
    if(message == "Event:Button:autoButton")
    {
      emit autoButtonClicked();
    }
    if(message == "Event:Button:haltButton")
    {
      emit haltButtonClicked();
    }
    if(message == "Request:Settings")
    {
      emit settingsRequested();
    }
    c->sendTextMessage("Ok");
    c->flush();
  }
}

void WebSockets::disconnected()
{
  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  c->deleteLater();
}

