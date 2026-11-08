#include "robot.h"
#include "ability_constants.h"
#include "heal_ability.h"

HealAbility::HealAbility(int heal_amount, int energy_cost)
    : heal_amount_(heal_amount),
      energy_cost_(energy_cost) {}

bool HealAbility::UseAbility(Robot& robot, GameField&, int, int) {
    if (!robot.SpendEnergy(energy_cost_)) {
        return false;
    }

    int missing_health = robot.GetMaxHealth() - robot.GetHealth();
    robot.Heal(heal_amount_);
    int over_heal = heal_amount_ - missing_health;

    if (over_heal > 0) {
        robot.AddStatus(StatusType::kShield, over_heal);
    }

    return true;
}

int HealAbility::GetEnergyCost() const {
    return energy_cost_;
}

void HealAbility::Upgrade() {
    heal_amount_ += kAbilityUpgradeStep;
}

const char* HealAbility::GetNameAbility() const {
    return "Heal";
}

AbilityType HealAbility::GetType() const {
    return AbilityType::kHeal;
}