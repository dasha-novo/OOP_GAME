#ifndef GAME_HEAL_ABILITY_H_
#define GAME_HEAL_ABILITY_H_

#include "ability.h"

class HealAbility : public Ability {
  public:
  HealAbility(int heal_amount, int energy_cost);

  bool UseAbility(Robot& robot, GameField& field, int goal_x, int goal_y) override;
  int GetEnergyCost() const override;
  void Upgrade() override;
  const char* GetNameAbility() const override;
  AbilityType GetType() const override;

  private:
  int heal_amount_;
  int energy_cost_;

};

#endif