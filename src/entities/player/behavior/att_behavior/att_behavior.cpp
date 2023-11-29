#include "att_behavior.h"

att_behavior::att_behavior(QVector<Player *> players, class SharedInfos *si){

    this->players = players;
    this->si = si;
    attTarget.resize(2);
    if(si->ourTeamColor == YELLOW){
        attTarget[0] = QVector2D(-2.95, -2.03);
        attTarget[1] = QVector2D(-2.95, 2.03);

    }
    else{
        attTarget[0] = QVector2D(2.95, -2.03);
        attTarget[1] = QVector2D(2.95, 2.03);

    }
    players[0]->state = 0;
    players[1]->state = 0;
    players[2]->state = 0;

}
void att_behavior::predictBall(int id){
    players[id]->rotateTo(si->map->ballPosition());
    QVector2D ballToPlayer = players[id]->getPosition() - si->map->ballPosition();
    double angleBtP = atan2(ballToPlayer.y(), ballToPlayer.x());
    double angleSpeed = atan2(si->getBallSpeed().y(), si->getBallSpeed().x());
    if(angleBtP < 0) angleBtP += M_PI * 2;
    if(angleSpeed < 0) angleSpeed +=M_PI*2;
    double teta = fabs(angleBtP - angleSpeed);
    QVector2D p = si->getBallSpeed()/cos(teta) * fabs(ballToPlayer.length())/fabs(si->getBallSpeed().length()) - ballToPlayer;
    players[id]->goTo(players[id]->getPosition() + p);
}

void att_behavior::attack(){
    for(int i=0;i<3;i++){
        if(players[i]->state == MOVING_TOWARD_TARGET){
            players[i]->rotateTo(si->map->ballPosition());
            players[i]->goTo(attTarget[i]);
            if(fabs((players[i]->getPosition() - attTarget[i]).length()) <= 0.05){
                players[i]->state = WAIT_FOR_BALL;
            }

        }
        if(players[i]->state == WAIT_FOR_BALL){
            players[i]->sendPacket(0,0);
            players[i]->rotateTo(si->map->ballPosition());
            if(si->receiver == players[i]){
                players[i]->state = PREDICTING_BALL;
            }

        }
        if(players[i]->state == PREDICTING_BALL){
            predictBall(i);
            if(si->ballHolder == players[i] && (si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalCenter(), {(quint8)i}) == false || si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalLeftPost(), {(quint8)i}) == false || si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalRightPost(), {(quint8)i}) == false)){
                players[i]->state = KICK_BALL_TO_GOAL;
            }
            if(si->ballHolder != players[i] && si->receiver != players[i]){
                players[i]->state = WAIT_FOR_BALL;
            }
        }
        if(players[i]->state == KICK_BALL_TO_GOAL){

        }
    }
}


void att_behavior::run(){
    players[0]->dribble(true);
    players[1]->dribble(true);
    players[2]->dribble(true);
    if(si->teamStrategy == ATTACK) attack();

}
