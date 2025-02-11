#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H 1

#include <QWebSocketServer>
#include <QWebSocket>

namespace Ui {
  class WebSockets;
}


class WebSockets : public QObject
{
  Q_OBJECT;

  public:
    explicit WebSockets(quint16 port, QObject *parent = nullptr);
    void writeToClient(QString message);
  private:
    QWebSocketServer *socketServer;
    QWebSocket *conn;
  signals:
    void autoButtonClicked();
    void haltButtonClicked();
    void settingsRequested();
  private slots:
    void onNewConnection();
    void textMessageReceived(QString message);
    void disconnected();
};

#endif
