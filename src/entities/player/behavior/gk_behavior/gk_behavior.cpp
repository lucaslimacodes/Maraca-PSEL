#include "gk_behavior.h"

gk_behavior::gk_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam){
    this->map = map;
    this->players = players;
    this->ourTeamColor = ourTeam;
    this->state = POINTING_AT_BALL;
    this->receiver = NULL;

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
        if((map->ballPosition() - getGoalCircleCenter()).x() <= 0.0){
            if((map->ballPosition() - getGoalCircleCenter()).y() > 0){
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
        if((map->ballPosition() - getGoalCircleCenter()).x() >= 0.0){
            if((map->ballPosition() - getGoalCircleCenter()).y() > 0){
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
    if(ourTeamColor == BLUE && getBallSpeed().x() >= 0){
        return false;
    }
    if(ourTeamColor == YELLOW && getBallSpeed().x() <= 0){
        return false;
    }
    if(ourTeamColor == BLUE){
        double ballSpeedAngle = atan2(getBallSpeed().y(), getBallSpeed().x());
        if(ballSpeedAngle < 0) ballSpeedAngle+=M_PI*2;
        QVector2D ballToLeft = map->ourGoalLeftPost() - map->ballPosition();
        QVector2D ballToRight = map->ourGoalRightPost() - map->ballPosition();
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
        double ballSpeedAngle = atan2(getBallSpeed().y(), getBallSpeed().x());
        QVector2D ballToLeft = map->ourGoalLeftPost() - map->ballPosition();
        QVector2D ballToRight = map->ourGoalRightPost() - map->ballPosition();
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
    QVector2D ballToPlayer = players[0]->getPosition() - map->ballPosition();
    double angleBtP = atan2(ballToPlayer.y(), ballToPlayer.x());
    double angleSpeed = atan2(getBallSpeed().y(), getBallSpeed().x());
    if(angleBtP < 0) angleBtP += M_PI * 2;
    if(angleSpeed < 0) angleSpeed +=M_PI*2;
    double teta = fabs(angleBtP - angleSpeed);
    QVector2D p = getBallSpeed()/cos(teta) * fabs(ballToPlayer.length())/fabs(getBallSpeed().length()) - ballToPlayer;
    players[0]->goTo(players[0]->getPosition() + p);
}

void gk_behavior::run(){
    updateReceiver(); //MANDATORY
    updateBallPoss(); //MANDATORY
    players[0]->dribble(true);
    if(state == POINTING_AT_BALL){
        pointAtBall();
        if(willBallHitGoal()){
            this->state = PREDICTING_BALL;
        }
        if(fabs((players[0]->getPosition() - map->ballPosition()).length()) < 0.18){
            this->state = KICKING_BALL_AWAY;
        }
    }
    if(state == PREDICTING_BALL){
        predictBall();
        if(!willBallHitGoal()){
            this->state = POINTING_AT_BALL;
        }
        if(fabs((players[0]->getPosition() - map->ballPosition()).length()) < 0.18){
            this->state = KICKING_BALL_AWAY;
        }
    }
    if(state == KICKING_BALL_AWAY){
        players[0]->kick(6,true);
        this->state = POINTING_AT_BALL;
    }

}
