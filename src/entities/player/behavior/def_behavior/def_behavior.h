#ifndef DEF_BEHAVIOR_H
#define DEF_BEHAVIOR_H

#include <QObject>
#include <src/entities/player/behavior/behavior.h>
#define HELPING_GK 101
#define ADVANCING_TOWARDS_BALL 102
#define ATTACKING 103
class def_behavior : public behavior
{
public:
    def_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam, Player *gk);
    Player *gk;
    void run();
    void helpGoal();

    QVector2D getGoalCircleCenter();

};

#endif // DEF_BEHAVIOR_H
