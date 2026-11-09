#ifndef GAME_ABILITY_H_
#define GAME_ABILITY_H_

#include "ability_type.h"

class Robot;
class GameField;

class Ability {
  public:
  virtual ~Ability() = default;

  virtual bool UseAbility(Robot& robot, GameField& field, int goal_x, int goal_y) = 0;
  virtual int GetEnergyCost() const = 0;
  virtual void Upgrade() = 0;
  virtual AbilityType GetType() const = 0;
};

#endif