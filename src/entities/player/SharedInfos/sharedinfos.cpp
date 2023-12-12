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



bool SharedInfos::isPathBlocked(QVector2D start, QVector2D end, QVector<quint8> ignoreAlly_Ids){
    QVector2D StE = end - start;

    for(int i=0;i<2;i++){
        for(int j=0;j<6;j++){
            if(PlayersData[i][j][0].x() > 10.0) continue;
            if(i == this->ourTeamColor && ignoreAlly_Ids.contains(allies[j]->getPlayerId())) continue;
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

SharedInfos::SharedInfos(WorldMap *map, bool ourTeamColor, QVector<Player *> allies, QVector<Player *> enemies){
    this->map = map;
    this->ourTeamColor = ourTeamColor;

    this->allies = allies;
    this->enemies = enemies;


}
void SharedInfos::passBall(Player *p_start, Player *p_end){
    if(p_start->hasBall){
        QVector2D StE = p_end->getPosition() - p_start->getPosition();
        double dist = fabs(StE.length());
        double angle = atan2(StE.y(), StE.x());
        if(fabs(p_start->getOrientation() - angle) > 0.05){
            p_start->rotateTo(p_end->getPosition());
        }
        else{
            if(isPathBlocked(p_start->getPosition(), p_end->getPosition(), {p_start->getPlayerId(), p_end->getPlayerId()}) == true)
                p_start->kick(2+dist*0.85, true);
            else p_start->kick(2+dist*1.0, false);
            p_start->hasBall = false;
            p_end->isReceiver = true;
        }
        /*if(fabs((p_start->getPosition() - map->ballPosition()).length()) > 0.25){
            p_start->hasBall = false;
            p_end->isReceiver = true;
        }*/
    }

}

void SharedInfos::kickBall(Player *p, QVector2D target){
    if(ourTeamColor == YELLOW){
        if(target == map->theirGoalLeftPost()) target = map->theirGoalLeftPost() + QVector2D(0,OFFSET);
        if(target == map->theirGoalRightPost()) target = map->theirGoalRightPost() - QVector2D(0,OFFSET);
    }
    else{
        if(target == map->theirGoalLeftPost()) target = map->theirGoalLeftPost() - QVector2D(0,OFFSET);
        if(target == map->theirGoalRightPost()) target = map->theirGoalRightPost() + QVector2D(0,OFFSET);
    }
    if(p->hasBall){
        QVector2D StE = target - p->getPosition();

        double angle = atan2(StE.y(), StE.x());
        if(fabs(p->getOrientation() - angle) > 0.03){
            p->rotateTo(target);
        }
        else if(p->timeWaiting>30){
            p->kick(6, false);
            p->hasBall = false;
            p->timeWaiting = 0;
        }
    }
}
