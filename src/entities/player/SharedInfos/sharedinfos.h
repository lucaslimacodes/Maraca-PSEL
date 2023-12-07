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
#define OFFSET 0.12
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
    int ourTeamColor;
    QVector2D getBallSpeed();
    bool anyEnemyNextToBall();
    bool anyAllyNextToBall();
    bool isPathBlocked(QVector2D start, QVector2D end, QVector<quint8> ignore);
    void passBall(Player *p_start, Player *p_end);
    void kickBall(Player *p, QVector2D target);
    SharedInfos(WorldMap *map, bool ourTeamColor, QVector<Player *> allies, QVector<Player *> enemies);

};

#endif // SHAREDINFOS_H
