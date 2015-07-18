
#include <QGuiApplication>
#include <QQuickView>
int main(int argc, char* argv[])
{
    QGuiApplication app(argc,argv);
    QQuickView view;
    view.setSource(QUrl("qrc:///" QWEBKIT_EXAMPLE_NAME ".qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();
    return app.exec();
}
