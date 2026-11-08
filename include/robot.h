#ifndef GAME_ROBOT_H_
#define GAME_ROBOT_H_

#include <memory>
#include <vector>

#include "ability_type.h"
#include "ability.h"
#include "status.h"

enum class RobotType {
  kPlayer,
  kHealer,
  kEnemy
};

class GameField;

class Robot {
  public:
  Robot(int max_health, int attack, int max_energy, int experience_to_rank_up);
  Robot(Robot&&) noexcept = default;
  Robot& operator=(Robot&&) noexcept = default;
  
  virtual ~Robot() = default;

  virtual bool IsEnemy() const = 0; 
  virtual RobotType GetType() const = 0;
      
  int GetMaxHealth() const;
  int GetHealth() const;
  int GetAttack() const;
  int GetMaxEnergy() const;
  void SetEnergy(int energy);
  int GetEnergy() const;
  int GetExperience() const;
  int GetExperienceToRankUp() const;
  int GetRank() const;
  void TakeDamage(int damage);
  void Heal(int heal);
  bool Interact(Robot& other);
  void GainExperience(int points);
  bool SpendEnergy(int piece);
  void RestoreEnergy(int piece);
  bool IsAlive() const;
  int GetSpeed() const;
  void SetSpeed(int speed);

  void AddAbility(AbilityType type);
  bool UpgradeAbility(AbilityType type);
  bool UseAbility(AbilityType type, GameField& field, int goal_x, int goal_y);
  bool HasAbility(AbilityType type) const;
  void AddStatus(StatusType type, int amount);
  bool HasStatus(StatusType type) const;
  int GetStatusValue(StatusType type) const;
  bool TryBlockMovement();
  bool TryBlockAbility(AbilityType type);
  void UpdateStatuses();

  private:
  std::vector<std::unique_ptr<Ability>> abilities_;
  std::vector<std::unique_ptr<Status>> statuses_;

  int max_health_;
  int health_;
  int attack_;
  int max_energy_;
  int energy_;
  int experience_;
  int experience_to_rank_up_;
  int rank_;
  int speed_;

  static constexpr int kMaxValue = 1000;
  static constexpr int kRankUpValue = 100;
  static constexpr int kMaxAttack = 500;
  static constexpr int kAttackPerRank = 50;
  static constexpr int kHeal = 20;
  static constexpr int kEnergyPerInteract = 25;
  static constexpr int kMaxSpeed = 3;
  static constexpr int kAbilityEnergyCost = 25;
  static constexpr int kInitialAreaAttackRadius = 1;

  void SetHealth(int health);
  void LevelUp();

  static int ControlValue(int value, int min, int max);
};

#endif