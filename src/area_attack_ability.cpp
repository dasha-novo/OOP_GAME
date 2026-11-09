#include "robot.h"
#include "ability_constants.h"
#include "area_attack_ability.h"
#include "game_field.h"

AreaAttackAbility::AreaAttackAbility(int radius_area, int energy_cost)
    : energy_cost_(energy_cost),
      radius_area_(radius_area) {}

bool AreaAttackAbility::UseAbility(Robot& robot, GameField& field, int, int) {
    int position_x{-1};
    int position_y{-1};

    bool attacked = false;

    if (!field.GetRobotPosition(robot, position_x, position_y)) return false;

    if (robot.GetEnergy() < energy_cost_) return false;

    for (int x = position_x - radius_area_; x <= position_x + radius_area_; x++) {
        for (int y = position_y - radius_area_; y <= position_y + radius_area_; y++) {
            Robot* current_robot = field.GetRobotForChange(x, y);
            if (current_robot != nullptr && current_robot != &robot) {
                current_robot->TakeDamage(robot.GetAttack());
                if (current_robot->IsAlive()) {
                    current_robot->AddStatus(StatusType::kOverload,1);
                }
                field.RemoveDeadRobot(x, y);
                attacked = true;
            }
        }
    }

    if (attacked) return robot.SpendEnergy(energy_cost_);
    return true;
}

int AreaAttackAbility::GetEnergyCost() const {
    return energy_cost_;
}

void AreaAttackAbility::Upgrade() {
    radius_area_ += kRadiusAreaAttack;
}

AbilityType AreaAttackAbility::GetType() const {
    return AbilityType::kAreaAttack;
}
