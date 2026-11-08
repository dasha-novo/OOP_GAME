#include "overload_status.h"

OverloadStatus::OverloadStatus() : active_(true) {}

StatusType OverloadStatus::GetType() const {
    return StatusType::kOverload;
}

int OverloadStatus::GetValue() const {
    return active_ ? 1 : 0;
}

void OverloadStatus::Add(int) {
    active_ = true;
}

bool OverloadStatus::BlocksMovement() const {
    return false;
}

bool OverloadStatus::BlocksAbility(AbilityType) const {
    return active_;
}

void OverloadStatus::Spend() {
    active_ = false;
}

void OverloadStatus::OnTurnStart(Robot&) {
}

void OverloadStatus::ModifyDamage(int&) {
}

bool OverloadStatus::IsExpired() const {
    return !active_;
}

void OverloadStatus::OnApply(Robot&) {
}