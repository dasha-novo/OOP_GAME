#include "burn_status.h"

BurnStatus::BurnStatus(int runtime) : runtime_(runtime > 0 ? runtime : 0) {}

StatusType BurnStatus::GetType() const {
    return StatusType::kBurn;
}

int BurnStatus::GetValue() const {
    return runtime_;
}

void BurnStatus::Add(int amount) {
    if (amount > 0) {
        runtime_ += amount;
    }
}

bool BurnStatus::BlocksMovement() const {
    return false;
}

bool BurnStatus::BlocksAbility(AbilityType) const {
    return false;
}

void BurnStatus::Spend() {
}

void BurnStatus::OnTurnStart(Robot& robot) {
    if (runtime_ > 0) {
        robot.TakeDamage(kOneDamage);
        --runtime_;
    }
}

void BurnStatus::ModifyDamage(int&) {
}

bool BurnStatus::IsExpired() const {
    return runtime_ <= 0;
}

void BurnStatus::OnApply(Robot& robot) {
    if (runtime_ > 0) {
        robot.TakeDamage(kOneDamage);
        --runtime_;
    }
}