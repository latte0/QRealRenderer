#ifndef SCENARIO_H
#define SCENARIO_H

#include <QList>
#include <QMap>
#include <QString>

#include "scene.h"
#include "scenerender.h"


class Scenario : QObject
{
    Q_OBJECT

public:
    Scenario();
    ~Scenario();
    Scene *currentscene;

    QMap<std::string, Scene> scenemap;

public slots:
    void changeScene(const QString &scenename);

};

#endif // SCENARIO_H
