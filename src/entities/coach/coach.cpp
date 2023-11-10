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

Coach::Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap, bool teamColor)
    : _players(players), _worldMap(worldMap)
{
    // Create QTimer and connects to the runCoach() slot
    _actuatorTimer = new QTimer(this);
    QObject::connect(_actuatorTimer, &QTimer::timeout, this, &Coach::runCoach);
    _actuatorTimer->start(COACH_ITERATION_INTERVAL_MS);
    before.resize(5);
    before = {{0,0},{0,0},{0,0},{0,0},{0,0}}; // pos, vel, accel, ...
    now.resize(5);
    now = {{0,0},{0,0},{0,0},{0,0},{0,0}};
    this->teamColor = teamColor;
    this->ab = new att_behavior(getWorldMap(), {getPlayer(teamColor,3).value(), getPlayer(teamColor,4).value(), getPlayer(teamColor,5).value()});
    this->db = new def_behavior(getWorldMap(), {getPlayer(teamColor,2).value(), getPlayer(teamColor,1).value()});
    this->gb = new gk_behavior(getWorldMap(), {getPlayer(teamColor, 0).value()});


}

Coach::~Coach(){
    //deleting all behavior instances
    delete ab;
    delete db;
    delete gb;
}

void Coach::printData(){
    std::cout << "+--------------------------------------------------------------+" << '\n';
    std::cout << "position: " << "(" << now[0].x() << "," << now[0].y() << ")" << '\n';
    std::cout << "velocity: " << "(" << now[1].x() << "," << now[1].y() << ")" << '\n';
    std::cout << "acceleration: " << "(" << now[2].x() << "," << now[2].y() << ")" << '\n';
    std::cout << "da/dt: " << "(" << now[3].x() << "," << now[3].y() << ")" << '\n';
    std::cout << "d²a/dt²: " << "(" << now[4].x() << "," << now[4].y() << ")" << '\n';
    std::cout << "+--------------------------------------------------------------+" << '\n';
}
void Coach::updateDataBall(){
    now[0] = getWorldMap()->ballPosition();
    for(int i=1;i<=4;i++){
        now[i] = (now[i-1] - before[i-1])/double(1.0/60.0);

    }
    printData();
    for(int i=0;i<=4;i++){
        before[i] = now[i];
    }

}

QVector2D Coach::getBallVelocity(){
    return now[1];
}
QVector2D Coach::getBallAcceleration(){
    return now[2];
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
    updateDataBall(); //MANDATORY



    //all behavior running
    ab->run();
    db->run();
    gb->run();

}
