#include "healer_robot.h"

bool HealerRobot::IsEnemy() const {
    return false; 
}

RobotType HealerRobot::GetType() const {
    return RobotType::kHealer;
}