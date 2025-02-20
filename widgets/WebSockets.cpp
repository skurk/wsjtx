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
    connect(socketServer, &QWebSocketServer::newConnection,
            this, &WebSockets::onNewConnection);
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
  connect(conn, &QWebSocket::disconnected,
          this, &WebSockets::disconnected);
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
  foreach(QWebSocket *c,  clients)
  {
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
    emit sendRemoteEvent(message);
  }
}

void WebSockets::disconnected()
{
  QWebSocket *c = qobject_cast<QWebSocket *>(sender());
  QMetaObject::invokeMethod(c, "close", Qt::DirectConnection);
  c->deleteLater();
}

void WebSockets::closeAllConnections()
{
  foreach(QWebSocket *w, clients)
  {
    QMetaObject::invokeMethod(w, "close", Qt::DirectConnection);
  }
}

void WebSockets::sendLocalEvent(QString, QString) {}

