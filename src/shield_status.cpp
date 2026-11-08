#include <algorithm>

#include "shield_status.h"

ShieldStatus::ShieldStatus(int amount)
    : shield_(amount > 0 ? amount : 0) {}

StatusType ShieldStatus::GetType() const {
    return StatusType::kShield;
}

int ShieldStatus::GetValue() const {
    return shield_;
}

void ShieldStatus::Add(int amount) {
    if (amount > 0) {
        shield_ += amount;
    }
}

bool ShieldStatus::BlocksMovement() const {
    return false;
}

bool ShieldStatus::BlocksAbility(AbilityType) const {
    return false;
}

void ShieldStatus::Spend() {
}

void ShieldStatus::OnTurnStart(Robot&) {
}

void ShieldStatus::ModifyDamage(int& damage) {
    if (damage <= 0 || shield_ <= 0) {
        return;
    }

    int blocked_damage =  std::min(damage, shield_);
    shield_ -= blocked_damage;
    damage -= blocked_damage;
}

bool ShieldStatus::IsExpired() const {
    return shield_ <= 0;
}

void ShieldStatus::OnApply(Robot&) {
}