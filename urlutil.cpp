#include "urlutil.h"

QUrl QRR::Util::urlFromUserInput(const QString& userInput)
{
    QFileInfo fileInfo(userInput);
    if (fileInfo.exists())
        return QUrl(fileInfo.absoluteFilePath());
    return QUrl::fromUserInput(userInput);
}

QUrl QRR::Util::startupUrl()
{
    QUrl ret;
    QStringList args(qApp->arguments());
    args.takeFirst();
    Q_FOREACH (const QString& arg, args) {
        if (arg.startsWith(QLatin1Char('-')))
             continue;
        ret = urlFromUserInput(arg);
        if (ret.isValid())
            return ret;
    }
    return QUrl(QStringLiteral("http://qt.io/"));
}
