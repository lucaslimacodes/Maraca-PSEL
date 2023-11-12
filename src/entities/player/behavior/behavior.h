#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <cmath>
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
    QVector<QVector<QVector<QVector2D>>> PlayersData;
    QVector<QVector2D> ballData;
    int ballPoss;
    int teamStrategy;
    int ourTeamColor;
    int state;
    Player *receiver; //ally player that is going to receive a pass
    QVector2D getBallSpeed();
    void updateBallPoss();
    bool anyEnemyNextToBall();
    bool anyAllyNextToBall();
    void updateReceiver();
};

#endif // BEHAVIOR_H
