#ifndef GAME_BURN_STATUS_H_
#define GAME_BURN_STATUS_H_

#include "status.h"
#include "robot.h"

class BurnStatus : public Status {
  public:
    explicit BurnStatus(int runtime);

    StatusType GetType() const override;
    int GetValue() const override;
    void Add(int amount) override;
    bool BlocksMovement() const override;
    bool BlocksAbility(AbilityType type) const override;
    void Spend() override;
    void OnTurnStart(Robot& robot) override;
    void ModifyDamage(int& damage) override;
    bool IsExpired() const override;
    void OnApply(Robot& robot) override;

  private:
    int runtime_;

    static constexpr int kOneDamage = 5;
};

#endif