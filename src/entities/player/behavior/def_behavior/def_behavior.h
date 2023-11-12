#ifndef DEF_BEHAVIOR_H
#define DEF_BEHAVIOR_H
#include <QObject>
#include <QObject>
#include <QVector>
#include <QVector2D>
#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <cmath>
#include <src/utils/utils.h>
#include <src/entities/player/SharedInfos/sharedinfos.h>
#define ENEMY 100
#define ALLY 101
#define CONFLICT 102
#define DEFENSE 103
#define ATTACK 104
#define BLUE true
#define YELLOW false
#define HELPING_GK 201
#define ADVANCING_TOWARDS_BALL 202
#define ATTACKING 203
class def_behavior : public QObject
{
    Q_OBJECT
public:
    def_behavior(QVector<Player *> players, SharedInfos *si);
    void run();
    void helpGoal();
    int state;
    QVector<Player *> players;
    SharedInfos *si;

    QVector2D getGoalCircleCenter();

};

#endif // DEF_BEHAVIOR_H
