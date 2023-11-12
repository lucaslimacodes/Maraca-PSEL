#include "behavior.h"

QVector2D behavior::getBallSpeed(){
    return ballData[1];
}

bool behavior::anyEnemyNextToBall(){
    for(int i=0;i<6;i++){
        if(fabs((PlayersData[!ourTeamColor][i][0] - map->ballPosition()).length()) <= 0.18){
            return true;
        }
    }
    return false;
}

bool behavior::anyAllyNextToBall(){
    for(int i=0;i<6;i++){
        if(fabs((PlayersData[ourTeamColor][i][0] - map->ballPosition()).length()) <= 0.18){
            return true;
        }
    }
    return false;
}
void behavior::updateBallPoss(){
    if(anyEnemyNextToBall() && !anyAllyNextToBall() && receiver == NULL){
        ballPoss = ENEMY;
        teamStrategy = DEFENSE;
    }
    if((anyEnemyNextToBall() && anyAllyNextToBall()) || (!anyAllyNextToBall() && !anyEnemyNextToBall())){
        ballPoss = CONFLICT;
        teamStrategy = DEFENSE;
    }
    if((anyAllyNextToBall() && !anyEnemyNextToBall()) || receiver != NULL){
        ballPoss = ALLY;
        teamStrategy = ATTACK;

    }
    else{
        ballPoss = CONFLICT;
        teamStrategy = DEFENSE;
    }
}
void behavior::updateReceiver(){
    if(receiver!=NULL){
        if(anyEnemyNextToBall()) receiver = NULL;
        if(fabs((map->ballPosition() - receiver->getPosition()).length()) <= 0.18){
            receiver = NULL;
        }
    }
}
