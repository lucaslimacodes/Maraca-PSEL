#ifndef GK_BEHAVIOR_H
#define GK_BEHAVIOR_H
#include <QObject>
#include <QObject>
#define GOAL_RADIUS 0.5
#define IDLE 201
#define POINTING_AT_BALL 202
#define PREDICTING_BALL 203
#define KICKING_BALL_AWAY 204
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

class gk_behavior : public QObject
{
    Q_OBJECT
public:
    gk_behavior(QVector<Player *> players, SharedInfos *si);
    bool willBallHitGoal();
    void pointAtBall();
    void predictBall();
    void run();
    int state;
    QVector<Player *> players;
    SharedInfos *si;


    QVector2D getGoalCircleCenter();

};

#endif // GK_BEHAVIOR_H
