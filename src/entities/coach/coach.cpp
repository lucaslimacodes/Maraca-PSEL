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
    timeCount = 0;
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
    /* /////////FIRST IMPLEMENTATION\\\\\\\\\\\\\\
    float radius = 0.6; //define radius of turn
    float tanSpeed = 1.0; // define tangent speed;
    float angularSpeed = tanSpeed/radius;

    //Please, mind the 5m/s angular speed limit
    //spin action only on blue robot with id = 0
    getPlayer(BLUE, 0).value()->sendPacket(tanSpeed,0,angularSpeed);
*/

    /*////////SECOND IMPLEMENTATION\\\\\\\\
    QVector2D center;
    center.setX(0);
    center.setY(0);

    QVector2D playerToCenter;
    playerToCenter = center - getPlayer(BLUE,0).value()->getPosition();

    QVector2D tangentUnit;
    tangentUnit.setY(playerToCenter.x() * -1);
    tangentUnit.setX(playerToCenter.y());
    tangentUnit = tangentUnit/tangentUnit.length();
    QVector2D nextPosition = getPlayer(BLUE,0).value()->getPosition() + tangentUnit;
    getPlayer(BLUE, 0).value()->goTo(nextPosition);

*/

    QVector2D center;
    center.setX(0);
    center.setY(0);
    float radius = 2;
    float tanSpeed = 2;
    std::cout << getPlayer(BLUE,0).value()->getState() << "||" << fabs((center - getPlayer(BLUE,0).value()->getPosition()).length()) << '\n';
    if(getPlayer(BLUE,0).value()->getState() == GO_TO_RADIUS){
        getPlayer(BLUE,0).value()->rotateTo(center);
        getPlayer(BLUE,0).value()->goTo(center);
        if(fabs((center - getPlayer(BLUE,0).value()->getPosition()).length()) <= radius){
            getPlayer(BLUE,0).value()->setState(ADJUST_RADIUS);
            getPlayer(BLUE,0).value()->sendPacket(0,0,0);


        }

    }
    else if(getPlayer(BLUE,0).value()->getState() == ADJUST_RADIUS){
        if(fabs((center - getPlayer(BLUE,0).value()->getPosition()).length()) <= radius){
            getPlayer(BLUE,0).value()->sendPacket(-0.3,0,0);
        }
        else{
            getPlayer(BLUE,0).value()->setState(TURN_TO_TANGENT);
            getPlayer(BLUE,0).value()->sendPacket(0,0,0);
        }
    }
    else if(getPlayer(BLUE,0).value()->getState() == TURN_TO_TANGENT){
        QVector2D playerToCenter;
        playerToCenter = center - getPlayer(BLUE,0).value()->getPosition();
        QVector2D tangentUnit;
        tangentUnit.setY(playerToCenter.x() * -1);
        tangentUnit.setX(playerToCenter.y());
        tangentUnit = tangentUnit + getPlayer(BLUE,0).value()->getPosition();
        getPlayer(BLUE,0).value()->rotateTo(tangentUnit);
        timeCount++;
        if(timeCount>=100){
            getPlayer(BLUE,0).value()->setState(START_SPIN);
            getPlayer(BLUE,0).value()->sendPacket(0,0,0);
        }

    }
    else{
        getPlayer(BLUE,0).value()->sendPacket(tanSpeed, 0, tanSpeed/radius);
    }






}
