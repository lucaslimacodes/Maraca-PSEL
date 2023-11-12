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
    players[0]->dribble(true);
    players[1]->dribble(true);
    players[0]->kick(6,true);
    players[1]->kick(6,true);
    players[1]->goTo(gk->getPosition() + CtG_45_CW/2);
    players[0]->rotateTo(map->ballPosition());
    players[1]->rotateTo(map->ballPosition());
    if(fabs((map->ballPosition() - players[0]->getPosition()).length()) <= 0.25){
        players[0]->rotateTo(map->ballPosition());
        players[0]->goTo(map->ballPosition());
        players[0]->kick(6,true);
    }
    if(fabs((map->ballPosition() - players[1]->getPosition()).length()) <= 0.25){
        players[1]->rotateTo(map->ballPosition());
        players[1]->goTo(map->ballPosition());
        players[1]->kick(6,true);
    }

}
void def_behavior::run(){
    updateReceiver(); //MANDATORY
    updateBallPoss(); //MANDATORY
    helpGoal();
    std::cout << isPathBlocked(QVector2D(0,0) , map->ballPosition()) << '\n';
}
