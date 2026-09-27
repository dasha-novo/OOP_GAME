#ifndef GAME_ENEMY_ROBOT_H_
#define GAME_ENEMY_ROBOT_H_

#include "robot.h"

class EnemyRobot : public Robot {
  public:
  using Robot::Robot;

  bool IsEnemy() const override;
  RobotType GetType() const override;
};

#endif