#include "SignalMonitor.hpp"

#include <QMetaMethod>
#include <QDebug>

//QMap<QObject*,QString> widgetIndex;

SignalMonitor::SignalMonitor()  {}

void SignalMonitor::Notify()
{

  auto i = senderSignalIndex();
  auto sig = sender()->metaObject()->method(i);
  QObject *s = sender();
  QString kuk = s->objectName();
  QString w = sig.methodSignature();
  if(!w.contains("destroy") && !w.contains("water"))
  {
    printf("SignalMonitor: %d %s %s\n", i, kuk.toStdString().c_str(), w.toStdString().c_str());
    emit sendLocalEvent(kuk, w);
  }

}

