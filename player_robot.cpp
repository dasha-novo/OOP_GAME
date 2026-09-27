#include "player_robot.h"

bool PlayerRobot::IsEnemy() const {
    return false; 
}

RobotType PlayerRobot::GetType() const {
    return RobotType::kPlayer;
}