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


class Utils : public QObject {
    Q_OBJECT
public:
    Utils(QObject* parent = 0) : QObject(parent) { }
    Q_INVOKABLE static QUrl fromUserInput(const QString& userInput) { return QRR::Util::urlFromUserInput(userInput); }
};


#endif // URLUTIL

