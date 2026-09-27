#ifndef GAME_ROBOT_H_
#define GAME_ROBOT_H_

enum class RobotType {
  kPlayer,
  kHealer,
  kEnemy
};

class Robot {
  public:
  Robot(int max_health, int attack, int max_energy, int experience_to_rank_up);
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

  private:
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

  void SetHealth(int health);
  void LevelUp();

  static int ControlValue(int value, int min, int max);
};

#endif