#include "def_behavior.h"

def_behavior::def_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam, Player *gk){
    this->map = map;
    this->players = players;
    this->ourTeamColor = ourTeam;
    this->gk = gk;
    this->receiver = NULL;
}
QVector2D def_behavior::getGoalCircleCenter(){
    return (ourTeamColor == BLUE) ? map->ourGoalCenter() + QVector2D(0.09,0) : map->ourGoalCenter() - QVector2D(0.09,0);
}
void def_behavior::helpGoal(){
    QVector2D centerToGk = gk->getPosition() - getGoalCircleCenter();
    QVector2D CtG_45_CW = QVector2D(centerToGk.x()*cos(-M_PI/4) - centerToGk.y()*sin(-M_PI/4), centerToGk.x()*sin(-M_PI/4) + centerToGk.y()*cos(-M_PI/4));
    QVector2D CtG_45_CCW = QVector2D(centerToGk.x()*cos(M_PI/4) - centerToGk.y()*sin(M_PI/4), centerToGk.x()*sin(M_PI/4) + centerToGk.y()*cos(M_PI/4));
    players[0]->goTo(gk->getPosition() + CtG_45_CCW/2);
    players[1]->goTo(gk->getPosition() + CtG_45_CW/2);
    players[0]->rotateTo(map->ballPosition());
    players[1]->rotateTo(map->ballPosition());

}
void def_behavior::run(){
    updateReceiver(); //MANDATORY
    updateBallPoss(); //MANDATORY
    helpGoal();
}
