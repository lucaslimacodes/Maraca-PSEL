#ifndef GK_BEHAVIOR_H
#define GK_BEHAVIOR_H

#include <QObject>
#include <src/entities/player/behavior/behavior.h>
class gk_behavior : public behavior
{
public:
    gk_behavior(WorldMap *map, QVector<Player *> players);
    void run();

};

#endif // GK_BEHAVIOR_H
