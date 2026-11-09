#ifndef GAME_TELEPORT_ABILITY_H_
#define GAME_TELEPORT_ABILITY_H_

#include "ability.h"

class TeleportAbility : public Ability {
  public:
  TeleportAbility(int energy_cost);

  bool UseAbility(Robot& robot, GameField& field, int goal_x, int goal_y) override;
  int GetEnergyCost() const override;
  void Upgrade() override;
  AbilityType GetType() const override;

  private:
  int energy_cost_;

  static constexpr int kRadiusTeleport = 5;
};

#endif