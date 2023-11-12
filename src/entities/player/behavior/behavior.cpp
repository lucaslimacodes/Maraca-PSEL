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
}
void behavior::updateReceiver(){
    if(receiver!=NULL){
        if(anyEnemyNextToBall()) receiver = NULL;
        if(fabs((map->ballPosition() - receiver->getPosition()).length()) <= 0.18){
            receiver = NULL;
        }
    }
}

bool behavior::isPathBlocked(QVector2D start, QVector2D end){
    QVector2D StE = end - start;
    for(int i=0;i<2;i++){
        for(int j=0;j<6;j++){
            if(PlayersData[i][j][0].x() > 10.0) continue;
            QVector2D StP = PlayersData[i][j][0] - start;
            double teta = Utils::angleBetweenVectors(StP,StE);
            double x = cos(teta)*fabs(StP.length());
            double d = fabs(StP.length())*sin(teta);

            if(d>0.18) continue;
            if(x < 0 || x > fabs(StE.length())) continue;
            return true;
        }
    }
    return false;
}
