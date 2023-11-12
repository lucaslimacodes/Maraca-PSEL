#include "att_behavior.h"

att_behavior::att_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam){
    this->map = map;
    this->players = players;
    this->ourTeamColor = ourTeam;
    this->receiver = NULL;
}

void att_behavior::run(){
    updateReceiver(); //MANDATORY
    updateBallPoss(); //MANDATORY
    //TODO
}
