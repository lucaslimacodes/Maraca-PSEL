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

#ifndef COACH_H
#define COACH_H

#include <QTimer>
#include <QMap>
#include <cmath>
#include <optional>

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <src/entities/player/behavior/att_behavior/att_behavior.h>
#include <src/entities/player/behavior/def_behavior/def_behavior.h>
#include <src/entities/player/behavior/gk_behavior/gk_behavior.h>
using namespace std::chrono;
#include <spdlog/spdlog.h>

#define COACH_ITERATION_INTERVAL_MS 16
#define YELLOW false
#define BLUE true

/*!
 * \brief The Coach class provides an interface to control the available Players in the field.
 */
class Coach : public QObject
{
    Q_OBJECT
public:
    QVector<QVector<QVector<QVector2D>>> before;
    QVector<QVector<QVector<QVector2D>>> now;
    QVector<QVector2D> before_ball;
    QVector<QVector2D> now_ball;
    bool teamColor;
    att_behavior *ab;
    def_behavior *db;
    gk_behavior *gb;
    int state_test;
    int frameCounter_test;


    /*!
     * \brief Coach class constructor. It receives a list of the registered players.
     * \param players The given list of registered players.
     * \param worldMap The given worldmap pointer.
     */
    Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap, bool teamColor);
    ~Coach();

    void updateData();

    QVector2D getBallVelocity();
    QVector2D getBallAcceleration();
    void updateDataToBehaviors();

protected:
    /*!
     * \brief Get a Player instance pointer with given parameters.
     * \param isTeamBlue A boolean that express if the Player team is blue or not.
     * \param playerId The searched player id.
     * \return A std::optional<> object that may contains a pointer to a Player instance
     * object if it exists or it should contains std::nullopt otherwise.
     */
    std::optional<Player*> getPlayer(const bool& isTeamBlue, const quint8& playerId);

    /*!
     * \return Return WorldMap pointer.
     */
    WorldMap* getWorldMap();

private:
    // QTimer implementation to enable loop
    QTimer *_actuatorTimer;

    // Internal variables
    QMap<bool, QList<Player*>> _players;
    WorldMap* _worldMap;

private slots:
    /*!
     * \brief Iterate coach entity.
     * \note This method should be called/connected with the QTimer timeout signal.
     */
    void runCoach();

};

#endif // COACH_H
