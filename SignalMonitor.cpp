#include "SignalMonitor.hpp"

#include <QMetaMethod>
#include <QDebug>

void SignalMonitor::Notify()
{
  auto i = senderSignalIndex();
  auto sig = sender()->metaObject()->method(i);
  qDebug() << sender() << sig.methodSignature();

//  printf("%s: %s\n", QString(sender())->toStdString().c_str(),
//         QString(sig.methodSignature())->toStdString().c_str());
}

