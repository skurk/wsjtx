#ifndef SIGNALMONITOR_H
#define SIGNALMONITOR_H 1

#include <QObject>

namespace Ui {
  class SignalMonitor;
}


class SignalMonitor: public QObject
{
  Q_OBJECT;

  public:
    SignalMonitor();
  public slots:
    void Notify();
};

#endif
