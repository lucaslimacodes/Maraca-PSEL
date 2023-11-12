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
class att_behavior : public QObject
{
    Q_OBJECT
public:
    att_behavior(QVector<Player *> players, SharedInfos *si);
    int state;
    QVector<Player *> players;
    SharedInfos *si;
    void run();
};

#endif // ATT_BEHAVIOR_H
