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
#define WAITING_FOR_BALL 801
#define PREDICTING_BALL 203
#define PASSING_BALL 803
#define FINDING_OPPORTUNITY_TO_KICK 804
#define KICKING_BALL_CENTER 805
#define KICKING_BALL_LEFT 806
#define KICKING_BALL_RIGHT 807
class att_behavior : public QObject
{
    Q_OBJECT
public:
    att_behavior(QVector<Player *> players, SharedInfos *si);
    int state;
    QVector<Player *> players;
    SharedInfos *si;
    QVector2D below_att_pos;
    QVector2D above_att_pos;
    void predictBall(int id);
    void run();
};

#endif // ATT_BEHAVIOR_H
