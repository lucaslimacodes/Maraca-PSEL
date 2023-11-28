#include "sharedinfos.h"

QVector2D SharedInfos::getBallSpeed(){
    return ballData[1];
}

bool SharedInfos::anyEnemyNextToBall(){
    for(int i=0;i<6;i++){
        if(fabs((PlayersData[!ourTeamColor][i][0] - map->ballPosition()).length()) <= 0.18){
            return true;
        }
    }
    return false;
}

bool SharedInfos::anyAllyNextToBall(){
    for(int i=0;i<6;i++){
        if(fabs((PlayersData[ourTeamColor][i][0] - map->ballPosition()).length()) <= 0.18){
            return true;
        }
    }
    return false;
}
void SharedInfos::updateBallPoss(){
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
void SharedInfos::updateReceiver(){
    if(receiver!=NULL){
        if(anyEnemyNextToBall()) receiver = NULL;
        if(fabs((map->ballPosition() - receiver->getPosition()).length()) <= 0.18){
            this->ballHolder = receiver;
            receiver = NULL;

        }
    }
}

void SharedInfos::updateBallHolder(){
    if(this->ballPoss == CONFLICT) this->ballHolder = NULL;
    else if(this->ballPoss == ALLY){
        for(int i=0;i<6;i++){
            if(fabs((this->allies[i]->getPosition() - this->map->ballPosition()).length()) <= 0.18 && allies[i]!=NULL){
                this->ballHolder = allies[i];
                break;
            }
        }
    }
    else if(this->ballPoss == ENEMY){
        for(int i=0;i<6;i++){
            if(fabs((this->enemies[i]->getPosition() - this->map->ballPosition()).length()) <= 0.18 && enemies[i]!=NULL){
                this->ballHolder = enemies[i];
                break;
            }
        }
    }
}

bool SharedInfos::isPathBlocked(QVector2D start, QVector2D end, QVector<int> ignoreAlly_Ids){
    QVector2D StE = end - start;

    for(int i=0;i<2;i++){
        for(int j=0;j<6;j++){
            if(PlayersData[i][j][0].x() > 10.0) continue;
            if(i == this->ourTeamColor && ignoreAlly_Ids.contains(j)) continue;
            QVector2D StP = PlayersData[i][j][0] - start;
            double teta = Utils::angleBetweenVectors(StP,StE);
            double x = cos(teta)*fabs(StP.length());
            double d = fabs(StP.length())*sin(teta);

            if(d>0.18) continue;
            if(x < 0 || x > fabs(StE.length())) continue;
            std::cout << i << "," << j << '\n';
            return true;
        }
    }
    return false;
}

void SharedInfos::run(){
    updateReceiver();
    updateBallPoss();
    updateBallHolder();
}
SharedInfos::SharedInfos(WorldMap *map, bool ourTeamColor, QVector<Player *> allies, QVector<Player *> enemies){
    this->map = map;
    this->ourTeamColor = ourTeamColor;
    this->receiver = NULL;
    this->ballHolder = NULL;
    this->allies = allies;
    this->enemies = enemies;

}
void SharedInfos::passBall(Player *p_start, Player *p_end){
    QVector2D StE = p_end->getPosition() - p_start->getPosition();
    double angle = atan2(StE.y(), StE.x());
    if(fabs(p_start->getOrientation() - angle) > 0.05){
        p_start->rotateTo(p_end->getPosition());
    }
    else{
        p_start->kick(4, false);
    }
    this->receiver = p_end;

}
