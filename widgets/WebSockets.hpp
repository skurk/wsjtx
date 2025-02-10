#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H 1

class WebSockets : public QObject
{
  public:
    explicit WebSockets(quint16 port, QObject *parent = nullptr);
  private:
//    QWebSocketServer *socketServer;
//    QWebSocket *socket;
  signals:
//    void closed();
  private slots:
//    void onNewConnection();
//    void processTextMessage(QString message);
//    void socketDisconnected();
};

#endif
