#include "att_behavior.h"

att_behavior::att_behavior(QVector<Player *> players, class SharedInfos *si){

    this->players = players;
    this->si = si;
    if(si->ourTeamColor == YELLOW){
        this->below_att_pos = QVector2D(-2.335,-1.964);
        this->above_att_pos = QVector2D(-2.335, 1.964);
    }
    else{
        this->below_att_pos = QVector2D(2.335,-1.964);
        this->above_att_pos = QVector2D(2.335, 1.964);
    }
    players[0]->state = WAITING_FOR_BALL;
    players[0]->hasBall = false;
    players[0]->isReceiver = false;
    players[1]->state = WAITING_FOR_BALL;
    players[1]->hasBall = false;
    players[1]->isReceiver = false;
    players[2]->state = PROTECTING_BALL;
    players[2]->hasBall = false;
    players[2]->isReceiver = false;
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

void att_behavior::protectBall(int id){
    QVector2D BallToCenter = si->map->ourGoalCenter() - si->map->ballPosition();
    players[id]->goTo(si->map->ballPosition() + BallToCenter/2);
}

bool att_behavior::isPlayerNextToBall(int id){
    if(fabs((players[id]->getPosition() - si->map->ballPosition()).length()) < 0.15) return true;
    else return false;
}
void att_behavior::run(){
    players[0]->dribble(true); //3: below
    players[1]->dribble(true); //4: above
    players[2]->dribble(true); //5
    for(int i=0;i<3;i++){
        if(players[i]->state == WAITING_FOR_BALL){
            players[i]->rotateTo(si->map->ballPosition());
            if(i == 0) players[i]->goTo(this->below_att_pos);
            if(i == 1) players[i]->goTo(this->above_att_pos);
            if(players[i]->isReceiver) {
                players[i]->state = PREDICTING_BALL;
                players[i]->timeWaiting = 0;
            }
        }
        if(players[i]->state == PREDICTING_BALL){
            predictBall(i);
            players[i]->timeWaiting++;
            if(fabs((players[i]->getPosition() - si->map->ballPosition()).length()) <= 0.18){
                players[i]->hasBall = true;
                players[i]->isReceiver = false;
                players[i]->state = FINDING_OPPORTUNITY_TO_KICK;
                players[i]->timeWaiting = 0;

            }
            if(players[i]->timeWaiting > 360){
                players[i]->isReceiver = false;
                players[i]->state = WAITING_FOR_BALL;

            }
        }
        if(players[i]->state == FINDING_OPPORTUNITY_TO_KICK){
            players[i]->timeWaiting++;
            if(fabs((players[i]->getPosition() - si->map->ballPosition()).length()) >= 0.15 && fabs(players[i]->getOrientation() - atan2((si->map->ballPosition() - players[i]->getPosition()).y(), (si->map->ballPosition() - players[i]->getPosition()).x())) >= 0.05){
                players[i]->goTo(si->map->ballPosition() + (si->map->ballPosition() - players[i]->getPosition()));
                players[i]->rotateTo(si->map->ballPosition());
                if(si->ourTeamColor == YELLOW){
                    if(si->map->ballPosition().x() > 0){
                        players[i]->hasBall = false;
                        players[i]->timeWaiting = 0;
                        players[i]->state = WAITING_FOR_BALL;
                    }
                }
                else{
                    if(si->map->ballPosition().x() < 0){
                        players[i]->hasBall = false;
                        players[i]->timeWaiting = 0;
                        players[i]->state = WAITING_FOR_BALL;
                    }
                }
            }
            else if(players[i]->timeWaiting >= 25){
                players[i]->rotateTo(si->map->theirGoalCenter());
                if(i == 0) players[i]->goTo(this->above_att_pos);
                if(i == 1) players[i]->goTo(this->below_att_pos);
                if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalCenter(), {players[i]->getPlayerId()}) == false){
                    players[i]->state = KICKING_BALL_CENTER;
                    players[i]->timeWaiting = 0;
                }
                if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalLeftPost(), {players[i]->getPlayerId()}) == false){
                    players[i]->state = KICKING_BALL_LEFT;
                    players[i]->timeWaiting = 0;
                }
                if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalRightPost(), {players[i]->getPlayerId()}) == false){
                    players[i]->state = KICKING_BALL_RIGHT;
                    players[i]->timeWaiting = 0;
                }
                if(i == 0){
                    if(fabs((players[i]->getPosition() - this->above_att_pos).length()) <= 0.1){
                        players[i]->state = KICKING_BALL_CENTER;
                        players[i]->timeWaiting = 0;
                    }
                }
                if(i == 1){
                    if(fabs((players[i]->getPosition() - this->below_att_pos).length()) <= 0.1){
                        players[i]->state = KICKING_BALL_CENTER;
                        players[i]->timeWaiting = 0;
                    }
                }

            }
        }
        if(players[i]->state == KICKING_BALL_CENTER){
            players[i]->sendPacket(0,0);
            players[i]->timeWaiting++;
            si->kickBall(players[i], si->map->theirGoalCenter());
            if(players[i]->hasBall == false) {
                players[i]->state = WAITING_FOR_BALL;
                if(i==2){
                    players[i]->state = PROTECTING_BALL;
                    players[i]->timeWaiting = 0;
                    this->available = NULL;
                }
            }
        }
        if(players[i]->state == KICKING_BALL_LEFT){
            players[i]->sendPacket(0,0);
            players[i]->timeWaiting++;
            si->kickBall(players[i], si->map->theirGoalLeftPost());
            if(players[i]->hasBall == false) {
                players[i]->state = WAITING_FOR_BALL;
                if(i==2){
                    players[i]->state = PROTECTING_BALL;
                    players[i]->timeWaiting = 0;
                    this->available = NULL;
                }
            }
        }
        if(players[i]->state == KICKING_BALL_RIGHT){
            players[i]->sendPacket(0,0);
            players[i]->timeWaiting++;
            si->kickBall(players[i], si->map->theirGoalRightPost());
            if(players[i]->hasBall == false) {
                players[i]->state = WAITING_FOR_BALL;
                if(i==2){
                    players[i]->state = PROTECTING_BALL;
                    players[i]->timeWaiting = 0;
                    this->available = NULL;
                }
            }
        }
        if(players[i]->state == PROTECTING_BALL){
            protectBall(i);
            players[i]->rotateTo(si->map->ballPosition());
            if(si->anyEnemyNextToBall() == false && si->anyAllyNextToBall(players[i]->getPlayerId()) == false){
                players[i]->state = ADVANCING_TO_BALL;
            }
            if(si->ourTeamColor == YELLOW){
                if(si->map->ballPosition().x() > 2.5){
                    players[i]->state = ADVANCING_TO_BALL;
                }
            }
            else{
                if(si->map->ballPosition().x() < -2.5){
                    players[i]->state = ADVANCING_TO_BALL;
                }
            }

        }
        if(players[i]->state == ADVANCING_TO_BALL){
            if(fabs((players[i]->getPosition() - si->map->ballPosition()).length()) < 0.17)players[i]->timeWaiting++;
            players[i]->goTo(si->map->ballPosition());
            players[i]->rotateTo(si->map->ballPosition());
            if(isPlayerNextToBall(i) && si->anyEnemyNextToBall() == false && players[i]->timeWaiting > 20){
                players[i]->state = CHECKING_AVAILABLE;
                players[i]->timeWaiting = 0;
                players[i]->hasBall = true;
            }
            if(si->anyEnemyNextToBall() || si->anyAllyNextToBall(players[i]->getPlayerId())){
                players[i]->state = PROTECTING_BALL;
            }

        }
        if(players[i]->state == CHECKING_AVAILABLE){
            if(si->ourTeamColor == YELLOW){
                if(players[i]->getPosition().x() < 0){
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalCenter(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_CENTER;
                        players[i]->timeWaiting = 0;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalLeftPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_LEFT;
                        players[i]->timeWaiting = 0;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalRightPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_RIGHT;
                        players[i]->timeWaiting = 0;
                    }
                }
            }
            else{
                if(players[i]->getPosition().x()>0){
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalCenter(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_CENTER;
                        players[i]->timeWaiting = 0;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalLeftPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_LEFT;
                        players[i]->timeWaiting = 0;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalRightPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_RIGHT;
                        players[i]->timeWaiting = 0;
                    }
                }
            }
            if(si->isPathBlocked(players[i]->getPosition(), players[0]->getPosition(), {players[i]->getPlayerId(), players[0]->getPlayerId()}) == false){
                this->available = players[0];
            }
            else if(si->isPathBlocked(players[i]->getPosition(), players[1]->getPosition(), {players[i]->getPlayerId(), players[1]->getPlayerId()}) == false){
                this->available = players[1];
            }
            else{
                this->available = players[0];
            }
            players[i]->state = PASSING_BALL_5;
            if(si->ourTeamColor == YELLOW){
                if(players[i]->getPosition().x() < 0){
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalCenter(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_CENTER;
                        players[i]->timeWaiting = 0;
                        this->available = NULL;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalLeftPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_LEFT;
                        players[i]->timeWaiting = 0;
                        this->available = NULL;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalRightPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_RIGHT;
                        players[i]->timeWaiting = 0;
                        this->available = NULL;
                    }
                }
            }
            else{
                if(players[i]->getPosition().x()>0){
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalCenter(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_CENTER;
                        players[i]->timeWaiting = 0;
                        this->available = NULL;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalLeftPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_LEFT;
                        players[i]->timeWaiting = 0;
                        this->available = NULL;
                    }
                    if(si->isPathBlocked(si->map->ballPosition(), si->map->theirGoalRightPost(), {players[i]->getPlayerId()}) == false){
                        players[i]->state = KICKING_BALL_RIGHT;
                        players[i]->timeWaiting = 0;
                        this->available = NULL;
                    }
                }
            }
        }
        if(players[i]->state == PASSING_BALL_5){
            si->passBall(players[i], this->available);
            if(players[i]->hasBall == false){
                players[i]->state = PROTECTING_BALL;
                this->available = NULL;
            }
        }




    }



}
