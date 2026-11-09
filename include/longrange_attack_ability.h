#ifndef GAME_LONGRANGE_ATTACK_ABILITY_H_
#define GAME_LONGRANGE_ATTACK_ABILITY_H_

#include "ability.h"

class LongrangeAttackAbility : public Ability {
  public:
  LongrangeAttackAbility(int damage, int energy_cost);

  bool UseAbility(Robot& robot, GameField& field, int goal_x, int goal_y) override;
  int GetEnergyCost() const override;
  void Upgrade() override;
  AbilityType GetType() const override;

  private:
  int damage_;
  int energy_cost_;

  static constexpr int kRadiusLongrangeAttack = 3;
};

#endif