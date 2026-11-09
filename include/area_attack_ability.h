#ifndef GAME_AREA_ATTACK_ABILITY_H_
#define GAME_AREA_ATTACK_ABILITY_H_

#include "ability.h"

class AreaAttackAbility : public Ability {
  public:
  AreaAttackAbility(int radius_area, int energy_cost);

  bool UseAbility(Robot& robot, GameField& field, int goal_x, int goal_y) override;
  int GetEnergyCost() const override;
  void Upgrade() override;
  AbilityType GetType() const override;

  private:
  int energy_cost_;
  int radius_area_;

  static constexpr int kRadiusAreaAttack = 1;
};

#endif