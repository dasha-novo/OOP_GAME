#include <algorithm>

#include "robot.h"
#include "ability_constants.h"
#include "teleport_ability.h"
#include "game_field.h"

TeleportAbility::TeleportAbility(int energy_cost)
    : energy_cost_(energy_cost) {}

bool TeleportAbility::UseAbility(Robot& robot, GameField& field, int goal_x, int goal_y) {
    if (robot.GetEnergy() < energy_cost_) return false;
    if (field.TeleportRobot(robot, goal_x, goal_y, kRadiusTeleport)) {
        return robot.SpendEnergy(energy_cost_);
    }
    return false;
}

int TeleportAbility::GetEnergyCost() const {
    return energy_cost_;
}

void TeleportAbility::Upgrade() {
    energy_cost_ -= kAbilityUpgradeStep;
    if (energy_cost_ <= 0) energy_cost_ = 1;
}

const char* TeleportAbility::GetNameAbility() const {
    return "Teleport";
}

AbilityType TeleportAbility::GetType() const {
    return AbilityType::kTeleport;
}