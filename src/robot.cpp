#include "robot.h"
#include "area_attack_ability.h"
#include "heal_ability.h"
#include "longrange_attack_ability.h"
#include "teleport_ability.h"
#include "overload_status.h"
#include "slowdown_status.h"
#include "burn_status.h"
#include "shield_status.h"

int Robot::ControlValue(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Robot::Robot(int max_health, int attack, int max_energy, int experience_to_rank_up)
    : max_health_(ControlValue(max_health, 1, kMaxValue)),
      health_(max_health_),
      attack_(ControlValue(attack, 0, kMaxAttack)),
      max_energy_(ControlValue(max_energy, 0, kMaxValue)),
      energy_(max_energy_),
      experience_(0),
      experience_to_rank_up_(ControlValue(experience_to_rank_up, 1, kMaxValue)),
      rank_(1),
      speed_(kMaxSpeed) {}

int Robot::GetMaxHealth() const {
    return max_health_;
}

int Robot::GetHealth() const {
    return health_;
}

int Robot::GetAttack() const {
    return attack_;
}

int Robot::GetMaxEnergy() const {
    return max_energy_;
}

int Robot::GetEnergy() const {
    return energy_;
}

int Robot::GetExperience() const {
    return experience_;
}

int Robot::GetExperienceToRankUp() const {
    return experience_to_rank_up_;
}

int Robot::GetRank() const {
    return rank_;
}

void Robot::SetHealth(int health) {
    health_ = ControlValue(health, 0, max_health_);
}

void Robot::SetEnergy(int energy) {
    energy_ = ControlValue(energy, 0, max_energy_);
}

void Robot::TakeDamage(int damage) {
    if (damage <= 0) return;
    int damage_after_statuses = damage;
    for (auto& status : statuses_) {
        if (status != nullptr) {
            status->ModifyDamage(damage_after_statuses);
        }
    }
    SetHealth(health_ - damage_after_statuses);
}

void Robot::Heal(int heal) {
    if (heal <= 0) return;
    SetHealth(health_ + heal);
}

bool Robot::Interact(Robot& other) {
    if (&other == this) return false;

    if (!SpendEnergy(kEnergyPerInteract)) return false;

    if (IsEnemy() != other.IsEnemy()) {
        other.TakeDamage(attack_);
    } else {
        other.Heal(kHeal);
    }

    return true;
}

void Robot::GainExperience(int points) {
    if (points <= 0) return;
    experience_ += points;
    while (experience_ >= experience_to_rank_up_) {
        LevelUp();
    }
}

void Robot::LevelUp() {
    rank_ += 1;
    max_health_ = ControlValue(max_health_ + kRankUpValue, 1, kMaxValue);
    max_energy_ = ControlValue(max_energy_ + kRankUpValue, 0, kMaxValue);
    attack_ = ControlValue(attack_ + kAttackPerRank, 0, kMaxAttack);
    health_ = max_health_;
    energy_ = max_energy_;
    experience_ -= experience_to_rank_up_;
    experience_to_rank_up_ = ControlValue(experience_to_rank_up_ + kRankUpValue, 1, kMaxValue);
}

bool Robot::SpendEnergy(int piece) {
    if (piece <= 0 || piece > energy_) return false;
    energy_ -= piece;
    return true;
}

void Robot::RestoreEnergy(int piece) {
    if (piece <= 0) return;
    energy_ = ControlValue(energy_ + piece, 0, max_energy_);
}

bool Robot::IsAlive() const {
    return health_ > 0;
}

int Robot::GetSpeed() const {
    return speed_;
}

void Robot::SetSpeed(int speed) {
    speed_ = ControlValue(speed, 0, kMaxSpeed);
}

bool Robot::HasAbility(AbilityType type) const {
    for (const auto& ability : abilities_) {
        if (ability != nullptr && ability->GetType() == type) {
            return true;
        }
    }
    return false;
}

void Robot::AddAbility(AbilityType type) {
    if (HasAbility(type)) return;

    switch (type) {
        case AbilityType::kHeal:
            abilities_.push_back(std::make_unique<HealAbility>(kHeal, kAbilityEnergyCost));
            break;
        case AbilityType::kTeleport:
            abilities_.push_back(std::make_unique<TeleportAbility>(kAbilityEnergyCost));
            break;
        case AbilityType::kAreaAttack:
            abilities_.push_back(std::make_unique<AreaAttackAbility>(kInitialAreaAttackRadius, kAbilityEnergyCost));
            break;
        case AbilityType::kLongrangeAttack:
            abilities_.push_back(std::make_unique<LongrangeAttackAbility>(attack_, kAbilityEnergyCost));
            break;
        default:
            break;
    }
}

bool Robot::UpgradeAbility(AbilityType type) {
    for (auto& ability : abilities_) {
        if (ability != nullptr && ability->GetType() == type) {
            ability->Upgrade();
            return true;
        }
    }

    return false;
}

bool Robot::UseAbility(AbilityType type, GameField& field, int goal_x, int goal_y) {
    for (auto& ability : abilities_) {
        if (ability != nullptr && ability->GetType() == type) {
            if (TryBlockAbility(type)) {
                return false;
            }
            return ability->UseAbility(*this, field, goal_x, goal_y);
        }
    }

    return false;
}

void Robot::AddStatus(StatusType type, int amount) {
    if (amount <= 0) return;

    for (auto& status : statuses_) {
        if (status != nullptr && status->GetType() == type) {
            status->Add(amount);
            status->OnApply(*this);
            return;
        }
    }

    switch (type) {
        case StatusType::kSlowdown:
            statuses_.push_back(std::make_unique<SlowdownStatus>(amount));
            break;

        case StatusType::kOverload:
            statuses_.push_back(std::make_unique<OverloadStatus>());
            break;

        case StatusType::kBurn:
            statuses_.push_back(std::make_unique<BurnStatus>(amount));
            break;

        case StatusType::kShield:
            statuses_.push_back(std::make_unique<ShieldStatus>(amount));
            break;
    }

    statuses_.back()->OnApply(*this);
}

bool Robot::HasStatus(StatusType type) const {
    for (const auto& status : statuses_) {
        if (status != nullptr && status->GetType() == type && !status->IsExpired()) {
            return true;
        }
    }
    return false;
}

int Robot::GetStatusValue(StatusType type) const {
    for (const auto& status : statuses_) {
        if (status != nullptr && status->GetType() == type && !status->IsExpired()) {
            return status->GetValue();
        }
    }

    return 0;
}

bool Robot::TryBlockMovement() {
    for (auto i = statuses_.begin(); i != statuses_.end(); ++i) {
        Status& status = **i;

        if (status.BlocksMovement()) {
            status.Spend();
            if (status.IsExpired()) statuses_.erase(i);
            return true;
        }
    }

    return false;
}

bool Robot::TryBlockAbility(AbilityType type) {
    for (auto i = statuses_.begin(); i != statuses_.end(); ++i) {
        Status& status = **i;

        if (status.BlocksAbility(type)) {
            status.Spend();
            if (status.IsExpired()) statuses_.erase(i);
            return true;
        }
    }

    return false;
}

void Robot::UpdateStatuses() {
    for (auto i = statuses_.begin(); i != statuses_.end();) {
        Status& status = **i;
        status.OnTurnStart(*this);

        if (status.IsExpired()) {
            i = statuses_.erase(i);
        } else ++i;
    }
}

void Robot::Die() {
    SetHealth(0);
}