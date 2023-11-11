#ifndef DEF_BEHAVIOR_H
#define DEF_BEHAVIOR_H

#include <QObject>
#include <src/entities/player/behavior/behavior.h>
class def_behavior : public behavior
{
public:
   def_behavior(WorldMap *map, QVector<Player *> players, bool ourTean);
    void run();

};

#endif // DEF_BEHAVIOR_H
