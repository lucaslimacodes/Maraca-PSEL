#ifndef ATT_BEHAVIOR_H
#define ATT_BEHAVIOR_H

#include <src/entities/player/behavior/behavior.h>
class att_behavior : public behavior
{
public:
    att_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam);
    void run();
};

#endif // ATT_BEHAVIOR_H
