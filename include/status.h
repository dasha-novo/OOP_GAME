#ifndef GAME_STATUS_H_
#define GAME_STATUS_H_

#include "ability_type.h"
#include "status_type.h"

class Robot;

class Status {
  public:
    virtual ~Status() = default;

    virtual StatusType GetType() const = 0;
    virtual int GetValue() const = 0;
    virtual void Add(int amount) = 0;
    virtual bool BlocksMovement() const = 0;
    virtual bool BlocksAbility(AbilityType type) const = 0;
    virtual void Spend() = 0;
    virtual void OnTurnStart(Robot& robot) = 0;
    virtual void ModifyDamage(int& damage) = 0;
    virtual bool IsExpired() const = 0;
    virtual void OnApply(Robot& robot) = 0;
};

#endif