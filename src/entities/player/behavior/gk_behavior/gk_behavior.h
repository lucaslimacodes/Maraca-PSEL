#ifndef GK_BEHAVIOR_H
#define GK_BEHAVIOR_H

#include <QObject>
#include <src/entities/player/behavior/behavior.h>
#define GOAL_RADIUS 0.5
#define IDLE 201
#define POINTING_AT_BALL 202
#define PREDICTING_BALL 203
#define KICKING_BALL_AWAY 204

class gk_behavior : public behavior
{
public:
    int state;
    gk_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam);
    void pointAtBall();
    void run();
    QVector2D getGoalCircleCenter();

};

#endif // GK_BEHAVIOR_H
