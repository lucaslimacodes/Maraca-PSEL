#include "gk_behavior.h"

gk_behavior::gk_behavior(QVector<Player *> players, class SharedInfos *si){

    this->players = players;
    this->si = si;
    this->state = POINTING_AT_BALL;

}
QVector2D gk_behavior::getGoalCircleCenter(){
    return (si->ourTeamColor == BLUE) ? si->map->ourGoalCenter() + QVector2D(0.09,0) : si->map->ourGoalCenter() - QVector2D(0.09,0);
}
void gk_behavior::pointAtBall(){
    players[0]->rotateTo(si->map->ballPosition());
    QVector2D target;
    QVector2D centerToBall = si->map->ballPosition() - getGoalCircleCenter();
    double a = centerToBall.y()/centerToBall.x();
    double x1 = sqrt(GOAL_RADIUS*GOAL_RADIUS/(1+a*a));
    double x2 = -x1;
    if(si->ourTeamColor == BLUE){
        double y = a*x1;
        target = getGoalCircleCenter() + QVector2D(x1,y);
        if((si->map->ballPosition() - getGoalCircleCenter()).x() <= 0.0){
            if((si->map->ballPosition() - getGoalCircleCenter()).y() > 0){
                target = getGoalCircleCenter() + QVector2D(0,GOAL_RADIUS);
            }
            else{
                target = getGoalCircleCenter() - QVector2D(0,GOAL_RADIUS);
            }
        }
    }
    else{
        double y = a*x2;
        target = getGoalCircleCenter() + QVector2D(x2,y);
        if((si->map->ballPosition() - getGoalCircleCenter()).x() >= 0.0){
            if((si->map->ballPosition() - getGoalCircleCenter()).y() > 0){
                target = getGoalCircleCenter() + QVector2D(0,GOAL_RADIUS);
            }
            else{
                target = getGoalCircleCenter() - QVector2D(0,GOAL_RADIUS);
            }
        }


    }
    players[0]->goTo(target);

}

bool gk_behavior::willBallHitGoal(){
    if(si->ourTeamColor == BLUE && si->getBallSpeed().x() >= 0){
        return false;
    }
    if(si->ourTeamColor == YELLOW && si->getBallSpeed().x() <= 0){
        return false;
    }
    if(si->ourTeamColor == BLUE){
        double ballSpeedAngle = atan2(si->getBallSpeed().y(), si->getBallSpeed().x());
        if(ballSpeedAngle < 0) ballSpeedAngle+=M_PI*2;
        QVector2D ballToLeft = si->map->ourGoalLeftPost() - si->map->ballPosition();
        QVector2D ballToRight = si->map->ourGoalRightPost() - si->map->ballPosition();
        double leftAngle = atan2(ballToLeft.y(), ballToLeft.x());
        double rightAngle = atan2(ballToRight.y(), ballToRight.x());
        if(leftAngle < 0) leftAngle += M_PI*2;
        if(rightAngle < 0) rightAngle +=M_PI*2;
        if(rightAngle > leftAngle){
            if(ballSpeedAngle <= rightAngle && ballSpeedAngle >= leftAngle){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if(ballSpeedAngle >= rightAngle && ballSpeedAngle <= leftAngle){
                return true;
            }
            else{
                return false;
            }
        }
    }
    else{
        double ballSpeedAngle = atan2(si->getBallSpeed().y(), si->getBallSpeed().x());
        QVector2D ballToLeft = si->map->ourGoalLeftPost() - si->map->ballPosition();
        QVector2D ballToRight = si->map->ourGoalRightPost() - si->map->ballPosition();
        double leftAngle = atan2(ballToLeft.y(), ballToLeft.x());
        double rightAngle = atan2(ballToRight.y(), ballToRight.x());
        if(rightAngle > leftAngle){
            if(ballSpeedAngle <= rightAngle && ballSpeedAngle >= leftAngle){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if(ballSpeedAngle >= rightAngle && ballSpeedAngle <= leftAngle){
                return true;
            }
            else{
                return false;
            }
        }
    }

}

void gk_behavior::predictBall(){
    QVector2D ballToPlayer = players[0]->getPosition() - si->map->ballPosition();
    double angleBtP = atan2(ballToPlayer.y(), ballToPlayer.x());
    double angleSpeed = atan2(si->getBallSpeed().y(), si->getBallSpeed().x());
    if(angleBtP < 0) angleBtP += M_PI * 2;
    if(angleSpeed < 0) angleSpeed +=M_PI*2;
    double teta = fabs(angleBtP - angleSpeed);
    QVector2D p = si->getBallSpeed()/cos(teta) * fabs(ballToPlayer.length())/fabs(si->getBallSpeed().length()) - ballToPlayer;
    players[0]->goTo(players[0]->getPosition() + p);
}

void gk_behavior::run(){
    players[0]->dribble(true);
    if(state == POINTING_AT_BALL){
        pointAtBall();
        if(willBallHitGoal()){
            this->state = PREDICTING_BALL;
        }
        if(fabs((players[0]->getPosition() - si->map->ballPosition()).length()) < 0.18){
            this->state = KICKING_BALL_AWAY;
        }
    }
    if(state == PREDICTING_BALL){
        predictBall();
        if(!willBallHitGoal()){
            this->state = POINTING_AT_BALL;
        }
        if(fabs((players[0]->getPosition() - si->map->ballPosition()).length()) < 0.18){
            this->state = KICKING_BALL_AWAY;
        }
    }
    if(state == KICKING_BALL_AWAY){
        players[0]->kick(6,true);
        this->state = POINTING_AT_BALL;
    }

}
