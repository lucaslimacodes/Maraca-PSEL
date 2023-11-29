#include "def_behavior.h"

def_behavior::def_behavior(QVector<Player *> players, class SharedInfos *si){

    this->players = players;
    this->si = si;
    this->state = HELPING_GK;

}
QVector2D def_behavior::getGoalCircleCenter(){
    return (si->ourTeamColor == BLUE) ? si->map->ourGoalCenter() + QVector2D(0.09,0) : si->map->ourGoalCenter() - QVector2D(0.09,0);
}
void def_behavior::predictBall(int id){
    QVector2D ballToPlayer = players[id]->getPosition() - si->map->ballPosition();
    double angleBtP = atan2(ballToPlayer.y(), ballToPlayer.x());
    double angleSpeed = atan2(si->getBallSpeed().y(), si->getBallSpeed().x());
    if(angleBtP < 0) angleBtP += M_PI * 2;
    if(angleSpeed < 0) angleSpeed +=M_PI*2;
    double teta = fabs(angleBtP - angleSpeed);
    QVector2D p = si->getBallSpeed()/cos(teta) * fabs(ballToPlayer.length())/fabs(si->getBallSpeed().length()) - ballToPlayer;
    players[id]->goTo(players[id]->getPosition() + p);
}
void def_behavior::helpGoal(){
    QVector2D centerToGk = si->PlayersData[si->ourTeamColor][0][0] - getGoalCircleCenter();
    QVector2D CtG_45_CW = QVector2D(centerToGk.x()*cos(-M_PI/4) - centerToGk.y()*sin(-M_PI/4), centerToGk.x()*sin(-M_PI/4) + centerToGk.y()*cos(-M_PI/4));
    QVector2D CtG_45_CCW = QVector2D(centerToGk.x()*cos(M_PI/4) - centerToGk.y()*sin(M_PI/4), centerToGk.x()*sin(M_PI/4) + centerToGk.y()*cos(M_PI/4));
    players[0]->goTo(si->PlayersData[si->ourTeamColor][0][0]+ CtG_45_CCW/2);
    players[0]->dribble(true);
    players[1]->dribble(true);
    players[0]->kick(6,true);
    players[1]->kick(6,true);
    players[1]->goTo(si->PlayersData[si->ourTeamColor][0][0] + CtG_45_CW/2);
    players[0]->rotateTo(si->map->ballPosition());
    players[1]->rotateTo(si->map->ballPosition());
    if(fabs((si->map->ballPosition() - players[0]->getPosition()).length()) <= 0.25){
        players[0]->rotateTo(si->map->ballPosition());
        players[0]->goTo(si->map->ballPosition());
        players[0]->kick(6,true);
    }
    if(fabs((si->map->ballPosition() - players[1]->getPosition()).length()) <= 0.25){
        players[1]->rotateTo(si->map->ballPosition());
        players[1]->goTo(si->map->ballPosition());
        players[1]->kick(6,true);
    }

}
void def_behavior::run(){
    if(this->state == HELPING_GK) helpGoal();

}
