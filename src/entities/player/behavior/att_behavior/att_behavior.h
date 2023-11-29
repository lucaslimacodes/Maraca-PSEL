#ifndef ATT_BEHAVIOR_H
#define ATT_BEHAVIOR_H
#include <QObject>
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
#define MOVING_TOWARD_TARGET 501
#define WAIT_FOR_BALL 502
#define PREDICTING_BALL 203
#define KICK_BALL_TO_GOAL 504
class att_behavior : public QObject
{
    Q_OBJECT
public:
    att_behavior(QVector<Player *> players, SharedInfos *si);
    int state;
    int attack_state;
    QVector<QVector2D> attTarget;
    QVector<Player *> players;
    SharedInfos *si;
    void predictBall(int id);
    void attack();
    void run();
};

#endif // ATT_BEHAVIOR_H
