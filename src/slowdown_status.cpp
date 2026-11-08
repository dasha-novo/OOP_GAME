#include "slowdown_status.h"

SlowdownStatus::SlowdownStatus(int turns)
    : remaining_turns_(turns > 0 ? turns : 0) {}

StatusType SlowdownStatus::GetType() const {
    return StatusType::kSlowdown;
}

int SlowdownStatus::GetValue() const {
    return remaining_turns_;
}

void SlowdownStatus::Add(int amount) {
    if (amount > 0) {
        remaining_turns_ += amount;
    }
}

bool SlowdownStatus::BlocksMovement() const {
    return remaining_turns_ > 0;
}

bool SlowdownStatus::BlocksAbility(AbilityType type) const {
    return remaining_turns_ > 0 && type == AbilityType::kTeleport;
}

void SlowdownStatus::Spend() {
    if (remaining_turns_ > 0) {
        --remaining_turns_;
    }
}

void SlowdownStatus::OnTurnStart(Robot&) {
}

void SlowdownStatus::ModifyDamage(int&) {
}

bool SlowdownStatus::IsExpired() const {
    return remaining_turns_ <= 0;
}

void SlowdownStatus::OnApply(Robot&) {
}