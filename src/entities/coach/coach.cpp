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
    this->teamColor = teamColor;
    this->ab = new att_behavior(getWorldMap(), {getPlayer(teamColor,3).value(), getPlayer(teamColor,4).value(), getPlayer(teamColor,5).value()}, this->teamColor);
    this->db = new def_behavior(getWorldMap(), {getPlayer(teamColor,2).value(), getPlayer(teamColor,1).value()}, this->teamColor);
    this->gb = new gk_behavior(getWorldMap(), {getPlayer(teamColor, 0).value()}, this->teamColor);
    now.resize(2);
    for(int i=0;i<2;i++){
        now[i].resize(6);
        for(int j=0;j<6;j++){
            now[i][j].resize(3);
            now[i][j] = {{0,0} , {0,0} , {0,0}};
        }
    }
    before.resize(2);
    for(int i=0;i<2;i++){
        before[i].resize(6);
        for(int j=0;j<6;j++){
            before[i][j].resize(3);
            before[i][j] = {{0,0} , {0,0} , {0,0}};
        }
    }
    before_ball.resize(3);
    before_ball = {{0,0} , {0,0} , {0,0}};
    now_ball.resize(3);
    now_ball = {{0,0} , {0,0} , {0,0}};
    state_test = 0;
    frameCounter_test = 0;

}

Coach::~Coach(){
    //deleting all behavior instances
    delete ab;
    delete db;
    delete gb;
}


void Coach::updateData(){

    now_ball[0] = getWorldMap()->ballPosition();
    for(int i=1;i<=2;i++){
        now_ball[i] = (now_ball[i-1] - before_ball[i-1])/double(1.0/60.0);

    }
    for(int i=0;i<=2;i++){
        before_ball[i] = now_ball[i];
    }

    for(int i=0;i<2;i++){ // 0 = yellow , 1 = blue
        for(int j=0;j<6;j++){
            now[i][j][0] = (i==0) ? getPlayer(YELLOW,j).value()->getPosition() : getPlayer(BLUE,j).value()->getPosition();
            for(int k=1;k<=2;k++){
                now[i][j][k] = (now[i][j][k-1] - before[i][j][k-1]) * 60.0;
            }
            for(int k=0;k<=2;k++){
                before[i][j][k] = now[i][j][k];
            }
        }
    }


}

void Coach::updateDataToBehaviors(){
    updateData();
    //this->gb->PlayersData = now;
    this->gb->ballData = now_ball;
    //this->db->PlayersData = now;
    this->db->ballData = now_ball;
    //this->ab->PlayersData= now;
    this->ab->ballData = now_ball;
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
    updateDataToBehaviors(); //MANDATORY
    getPlayer(!teamColor,1).value()->dribble(true);
    if(state_test == 0){
        getPlayer(!teamColor, 1).value()->rotateTo(getWorldMap()->ourGoalRightPost() + QVector2D(0,0.05));
        frameCounter_test++;
        if(frameCounter_test >= 120){
            state_test = 1;
        }
    }
    if(state_test == 1){
        getPlayer(!teamColor, 1).value()->kick(6,false);
    }


    //all behavior running
    ab->run();
    db->run();
    gb->run();

}
