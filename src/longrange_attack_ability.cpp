#include <cstdlib>

#include "robot.h"
#include "ability_constants.h"
#include "longrange_attack_ability.h"
#include "game_field.h"

LongrangeAttackAbility::LongrangeAttackAbility(int damage, int energy_cost)
    : damage_(damage),
      energy_cost_(energy_cost) {}

bool LongrangeAttackAbility::UseAbility(Robot& robot, GameField& field, int goal_x, int goal_y) {
    int position_x{-1};
    int position_y{-1};
    if(field.GetRobotPosition(robot, position_x, position_y)
        && std::abs(goal_x - position_x) <= kRadiusLongrangeAttack && std::abs(goal_y - position_y) <= kRadiusLongrangeAttack) {
        
        Robot* goal_robot = field.GetRobotForChange(goal_x, goal_y);
        if(goal_robot == nullptr || goal_robot == &robot) {
            return false;
        }

        if (robot.GetEnergy() >= energy_cost_) {
            goal_robot->TakeDamage(damage_);
            if (goal_robot->IsAlive()) {
                goal_robot->AddStatus(StatusType::kBurn, 5);
            }
            field.RemoveDeadRobot(goal_x, goal_y);
            return robot.SpendEnergy(energy_cost_);
        }
    }
    return false;
}

int LongrangeAttackAbility::GetEnergyCost() const {
    return energy_cost_;
}

void LongrangeAttackAbility::Upgrade() {
    damage_ += kAbilityUpgradeStep;
}

AbilityType LongrangeAttackAbility::GetType() const {
    return AbilityType::kLongrangeAttack;
}