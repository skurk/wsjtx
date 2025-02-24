#include <QWebSocketServer>
#include <QWebSocket>
#include <QColor>
#include <QVector>
#include "WebSockets.hpp"

extern QVector<QColor> g_ColorTbl;


WebSockets::WebSockets(quint16 port, QObject *parent) :
           QObject(parent),
           socketServer(new QWebSocketServer("Websocket server",
                                             QWebSocketServer::NonSecureMode,
                                             this))
{
  if(socketServer->listen(QHostAddress::Any, port))
  {
    printf("WebSocket constructor: init port=%d\n", (int)port);
    connect(socketServer, &QWebSocketServer::newConnection, this, &WebSockets::onNewConnection);
    connect(socketServer, &QWebSocketServer::closed, this, &WebSockets::closed);
  }
}

WebSockets::~WebSockets()
{
  closeServer();
}

void WebSockets::closeServer()
{
  socketServer->close();
}

void WebSockets::onNewConnection()
{
  QWebSocket *conn = socketServer->nextPendingConnection();
  current = conn;
  connect(conn, &QWebSocket::textMessageReceived,
          this, &WebSockets::textMessageReceived);
//  connect(conn, &QWebSocket::disconnected,
//          this, &WebSockets::disconnected);
  clients << conn;

  // Init connection by dumping settings to client

  for(auto i=wsjtx_settings.cbegin(), end=wsjtx_settings.cend(); i != end; ++i)
  {
    QString message = QString("Settings:"+i.key()+":"+i.value());
    writeToClient(message);
    printf("%s\n", message.toStdString().c_str());
  }

  // Palette for waterfall

  QString response = "Palette:";
  for(int i=0; i<g_ColorTbl.size(); i++)
  {
    response += g_ColorTbl[i].name();
    response += ",";
  }
  writeToClient(response);

  // Tell client to boot

  writeToClient("Boot");
}

void WebSockets::writeToClient(QString message)
{
  if(!message.contains("WaterFall"))
    printf("writeToClient(\"%s\");\n", message.toStdString().c_str());

  foreach(QWebSocket *c,  clients)
  {
    if(c && c->isValid())
    {
      c->sendTextMessage(message);
      c->flush();
    }
    else
    {
//      printf("Unable to send to client (socket not open)\n");
    }
  }
}

void WebSockets::textMessageReceived(QString message)
{
  printf("Message received: %s\n", message.toStdString().c_str());
  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  if(c)
  {
    emit sendRemoteEvent(message);
  }
}

void WebSockets::socketDisconnected()
{
  printf("Client disconnected\n");
  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  if(c)
  {
    clients.removeAll(c);
    c->deleteLater();
  }
}

void WebSockets::closeAllConnections()
{
  foreach(QWebSocket *c,  clients)
  {
    if(c)
    {
      clients.removeAll(c);
      c->deleteLater();
    }
  }
}

void WebSockets::sendLocalEvent(QString, QString) {}

