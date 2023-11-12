#include "att_behavior.h"

att_behavior::att_behavior(QVector<Player *> players, class SharedInfos *si){

    this->players = players;
    this->si = si;

}

void att_behavior::run(){
    if(si->receiver!=NULL) std::cout << si->receiver->getPosition().x() << '\n';
}
