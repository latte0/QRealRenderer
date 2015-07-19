#ifndef URLUTIL
#define URLUTIL

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtCore/QStringList>

namespace QRR{
namespace Util{
QUrl urlFromUserInput(const QString& userInput);

QUrl startupUrl();
}
}

#endif // URLUTIL

