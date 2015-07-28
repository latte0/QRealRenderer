#ifndef SCENARIO_H
#define SCENARIO_H

#include <QList>
#include <QMap>
#include <QString>

#include <QOpenGLContext>

#include <memory>

#include "scene.h"
#include "scenerender.h"


class Scenario : public QObject
{
    Q_OBJECT

public:
    Scenario();
    ~Scenario();
    QTimer* timer;
    std::shared_ptr<SceneRender> m_scenerender;
    std::list<std::shared_ptr<Scene>> sceneobjects;
    std::shared_ptr<QOpenGLContext> m_context;

    std::shared_ptr<Scene> currentscene;

    std::map<std::string, Scene> scenemap;



private slots:
    void changeScene(const QString &scenename);
    void run();

};

#endif // SCENARIO_H
