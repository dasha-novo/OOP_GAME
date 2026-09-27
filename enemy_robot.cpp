#include "enemy_robot.h"

bool EnemyRobot::IsEnemy() const {
  return true; 
}

RobotType EnemyRobot::GetType() const {
    return RobotType::kEnemy;
}