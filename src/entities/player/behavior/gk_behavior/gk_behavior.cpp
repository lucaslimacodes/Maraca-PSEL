#include "gk_behavior.h"

gk_behavior::gk_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam){
    this->map = map;
    this->players = players;
    this->ourTeamColor = ourTeam;
    this->state = IDLE;

}
QVector2D gk_behavior::getGoalCircleCenter(){
    return (ourTeamColor == BLUE) ? map->ourGoalCenter() + QVector2D(0.09,0) : map->ourGoalCenter() - QVector2D(0.09,0);
}
void gk_behavior::pointAtBall(){
    players[0]->rotateTo(map->ballPosition());
    QVector2D target;
    QVector2D centerToBall = map->ballPosition() - getGoalCircleCenter();
    double a = centerToBall.y()/centerToBall.x();
    double x1 = sqrt(GOAL_RADIUS*GOAL_RADIUS/(1+a*a));
    double x2 = -x1;
    if(ourTeamColor == BLUE){
        double y = a*x1;
        target = getGoalCircleCenter() + QVector2D(x1,y);
    }
    else{
        double y = a*x2;
        target = getGoalCircleCenter() + QVector2D(x2,y);
    }
    players[0]->goTo(target);

}

void gk_behavior::run(){
    //TODO
    pointAtBall();


}
