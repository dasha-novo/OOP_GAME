#ifndef GAME_GAME_STATE_H_
#define GAME_GAME_STATE_H_

enum class GameState {
  kChoosingAbility,
  kPlaying,
  kChoosingRankAbility,
  kChoosingDisplayMode,
  kVictory,
  kGameOver
};

enum class DisplayMode {
  kStandard,
  kEnemies,
  kImpassableCells,
  kStatusCells
};

#endif