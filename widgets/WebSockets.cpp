#include <QWebSocketServer>
#include <QWwbSocket>

WebSocket::WebSocket(quint16 port, QObject *parent) :
           QObject(parent),
           socketServer(new QWebSocketServer("Websocket server",
                                             QWebSocketServer::NonSecureMode,
                                             this))
{
  printf("WebSocket constructor\n");
}

