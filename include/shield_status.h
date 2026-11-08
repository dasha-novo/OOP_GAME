#ifndef GAME_SHIELD_STATUS_H_
#define GAME_SHIELD_STATUS_H_

#include "status.h"

class ShieldStatus : public Status {
  public:
    explicit ShieldStatus(int amount);

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
    int shield_;
    
};

#endif