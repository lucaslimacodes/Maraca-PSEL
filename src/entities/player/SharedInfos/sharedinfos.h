#ifndef SHAREDINFOS_H
#define SHAREDINFOS_H

#include <QObject>
#include <QVector>
#include <QVector2D>
#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <cmath>
#include <optional>
#include <src/utils/utils.h>
#define ENEMY 100
#define ALLY 101
#define CONFLICT 102
#define DEFENSE 103
#define ATTACK 104
#define BLUE true
#define YELLOW false
class SharedInfos : public QObject
{
    Q_OBJECT
public:
    WorldMap *map;
    QVector<QVector<QVector<QVector2D>>> PlayersData;
    QVector<QVector2D> ballData;
    QVector<Player *> allies;
    QVector<Player *> enemies;
    int ballPoss;
    int teamStrategy;
    int ourTeamColor;
    Player *ballHolder;
    Player *receiver;
    QVector2D getBallSpeed();
    void updateBallPoss();
    bool anyEnemyNextToBall();
    bool anyAllyNextToBall();
    void updateReceiver();
    void updateBallHolder();
    bool isPathBlocked(QVector2D start, QVector2D end);
    void passBall(Player *p_start, Player *p_end);
    void run();
    SharedInfos(WorldMap *map, bool ourTeamColor, QVector<Player *> allies, QVector<Player *> enemies);

};

#endif // SHAREDINFOS_H
