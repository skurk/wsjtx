#include <QMetaMethod>
#include <QDebug>

#include "SignalMonitor.hpp"

SignalMonitor::SignalMonitor()  {}

void SignalMonitor::Notify()
{

  auto i = senderSignalIndex();
  auto sig = sender()->metaObject()->method(i);
  QObject *s = sender();
  QString objName = s->objectName();
  QString methodSig = sig.methodSignature();

  // default ignore null objects and destroy signals to reduce overhead
  if(!objName.isEmpty() && !methodSig.contains("destroy"))
  {
    emit sendLocalEvent(objName, methodSig);
  }
}

