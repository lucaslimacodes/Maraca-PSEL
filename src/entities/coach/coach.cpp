/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "coach.h"

Coach::Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap)
    : _players(players), _worldMap(worldMap)
{
    // Create QTimer and connects to the runCoach() slot
    _actuatorTimer = new QTimer(this);
    QObject::connect(_actuatorTimer, &QTimer::timeout, this, &Coach::runCoach);
    _actuatorTimer->start(COACH_ITERATION_INTERVAL_MS);
    getPlayer(BLUE,0).value()->setState(GO_TOWARDS_BALL);
}

std::optional<Player*> Coach::getPlayer(const bool& isTeamBlue, const quint8& playerId) {
    // Get list of players
    QList<Player*> playersForColor = _players.value(isTeamBlue);

    // Iterate searching for playerId
    for(const auto& player : playersForColor) {
        if(player->getPlayerId() == playerId) {
            return player;
        }
    }

    // If could not found, just return std::nullopt (should trait later)
    return std::nullopt;
}

WorldMap* Coach::getWorldMap() {
    return _worldMap;
}

void Coach::runCoach() {

    //ROBOTS USED: BLUE, 0; BLUE, 1; BLUE, 2

    for(int i=0;i<=2;i++){
        getPlayer(BLUE,i).value()->dribble(true);
        if(getPlayer(BLUE,i).value()->getState() == GO_TOWARDS_BALL){

            getPlayer(BLUE,i).value()->goTo(getWorldMap()->ballPosition());
            getPlayer(BLUE,i).value()->rotateTo(getWorldMap()->ballPosition());
            if(fabs((getWorldMap()->ballPosition() - getPlayer(BLUE,i).value()->getPosition()).length()) <= 0.15){
                getPlayer(BLUE,i).value()->setState(SPIN_WITH_BALL);
                getPlayer(BLUE,i).value()->sendPacket(0,0,0);
            }

        }
        if(getPlayer(BLUE,i).value()->getState() == SPIN_WITH_BALL){
            if(i!=2){
                getPlayer(BLUE,i).value()->rotateTo(getPlayer(BLUE,i+1).value()->getPosition());
            }
            else {
                getPlayer(BLUE,i).value()->rotateTo(getWorldMap()->ourGoalCenter());
            }
            double targetOrientaion = (i != 2) ? atan2(getPlayer(BLUE,i+1).value()->getPosition().y() - getPlayer(BLUE,i).value()->getPosition().y(), getPlayer(BLUE,i+1).value()->getPosition().x() - getPlayer(BLUE,i).value()->getPosition().x()) : atan2(getWorldMap()->ourGoalCenter().y() - getPlayer(BLUE,i).value()->getPosition().y(), getWorldMap()->ourGoalCenter().x()  - getPlayer(BLUE,i).value()->getPosition().x());
            if(fabs(getPlayer(BLUE,i).value()->_orientation - targetOrientaion) <= 0.001){
                getPlayer(BLUE,i).value()->setState(KICK_BALL);
                getPlayer(BLUE,i).value()->sendPacket(0,0,0);

            }

        }
        if(getPlayer(BLUE,i).value()->getState() == KICK_BALL){
            getPlayer(BLUE,i).value()->kick(5,false);
            getPlayer(BLUE,i).value()->setState(0);
            getPlayer(BLUE,i).value()->sendPacket(0,0,0);
        }
        if(getPlayer(BLUE,i).value()->getState() == WAIT_FOR_BALL){
            getPlayer(BLUE,i).value()->rotateTo(getWorldMap()->ballPosition());
            if(fabs((getWorldMap()->ballPosition() - getPlayer(BLUE,i).value()->getPosition()).length()) <= 0.15){
                getPlayer(BLUE,i).value()->setState(SPIN_WITH_BALL);
                getPlayer(BLUE,i).value()->sendPacket(0,0,0);
            }
        }
        if(getPlayer(BLUE,i).value()->getState() == 0){
            getPlayer(BLUE,i).value()->sendPacket(0,0,0);
            getPlayer(BLUE,i).value()->kick(0,false);
        }
    }


}
