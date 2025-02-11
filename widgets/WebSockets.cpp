#include <QWebSocketServer>
#include <QWebSocket>
#include "WebSockets.hpp"

WebSockets::WebSockets(quint16 port, QObject *parent) :
           QObject(parent),
           socketServer(new QWebSocketServer("Websocket server",
                                             QWebSocketServer::NonSecureMode,
                                             this))
{
  printf("WebSocket constructor: init port=%d\n", (int)port);
}

