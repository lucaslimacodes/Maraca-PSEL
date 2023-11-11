#include "def_behavior.h"

def_behavior::def_behavior(WorldMap *map, QVector<Player *> players, bool ourTeam){
    this->map = map;
    this->players = players;
    this->ourTeamColor = ourTeam;
}

void def_behavior::run(){
    //TODO
}
