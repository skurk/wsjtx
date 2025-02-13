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
    ~WebSockets();
    void writeToClient(QString message);
  private:
    QWebSocketServer *socketServer;
    QWebSocket *current;
    QList<QWebSocket *> clients;
  signals:
    void autoButtonClicked();
    void haltButtonClicked();
    void settingsRequested();
    void setTxFreq(int txf);
    void setRxFreq(int rxf);
  private slots:
    void onNewConnection();
    void textMessageReceived(QString message);
    void disconnected();
};

#endif
