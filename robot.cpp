#include "robot.h"

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
    SetHealth(health_ - damage);
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