#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#define ENEMY 100
#define ALLY 101
#define CONFLICT 102
#define DEFENSE 103
#define ATTACK 104
#define BLUE true
#define YELLOW false
class behavior : public QObject
{
    Q_OBJECT
public:
    WorldMap *map;
    QVector<Player *> players; //players that are going to behave in this strategy
    int ballPoss;
    int teamStrategy;
    int ourTeamColor;

};

#endif // BEHAVIOR_H
