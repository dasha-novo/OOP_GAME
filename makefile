CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

OBJECTS = main.o game.o game_window.o cell.o game_field.o \
          robot.o player_robot.o enemy_robot.o healer_robot.o renderer.o \
          area_attack_ability.o heal_ability.o \
          longrange_attack_ability.o teleport_ability.o \
          slowdown_status.o overload_status.o burn_status.o shield_status.o

game : $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDLIBS) -o $@

main.o : src/main.cpp include/game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

game.o : src/game.cpp include/game.h include/game_window.h \
         include/game_field.h include/renderer.h \
         include/player_robot.h include/enemy_robot.h \
         include/healer_robot.h include/game_state.h \
         include/ability_type.h include/robot.h \
         include/status.h include/status_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_window.o : src/game_window.cpp include/game_window.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

cell.o : src/cell.cpp include/cell.h include/status_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_field.o : src/game_field.cpp include/game_field.h \
               include/cell.h include/robot.h include/ability.h \
               include/status_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

robot.o : src/robot.cpp include/robot.h include/ability.h \
          include/ability_type.h include/status.h \
          include/status_type.h include/area_attack_ability.h \
          include/heal_ability.h include/longrange_attack_ability.h \
          include/teleport_ability.h include/slowdown_status.h \
          include/overload_status.h include/burn_status.h \
          include/shield_status.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

player_robot.o : src/player_robot.cpp include/player_robot.h \
                 include/robot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

enemy_robot.o : src/enemy_robot.cpp include/enemy_robot.h \
                include/robot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

healer_robot.o : src/healer_robot.cpp include/healer_robot.h \
                 include/robot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

renderer.o : src/renderer.cpp include/renderer.h \
             include/game_field.h include/game_window.h \
             include/game_state.h include/robot.h \
             include/ability_type.h include/status_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

area_attack_ability.o : src/area_attack_ability.cpp \
                        include/area_attack_ability.h \
                        include/ability.h include/ability_constants.h \
                        include/robot.h include/game_field.h \
                        include/status_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

heal_ability.o : src/heal_ability.cpp include/heal_ability.h \
                 include/ability.h include/ability_constants.h \
                 include/robot.h include/status_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

longrange_attack_ability.o : src/longrange_attack_ability.cpp \
                              include/longrange_attack_ability.h \
                              include/ability.h include/ability_constants.h \
                              include/robot.h include/game_field.h \
                              include/status_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

teleport_ability.o : src/teleport_ability.cpp \
                     include/teleport_ability.h \
                     include/ability.h include/ability_constants.h \
                     include/robot.h include/game_field.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

slowdown_status.o : src/slowdown_status.cpp \
                    include/slowdown_status.h include/status.h \
                    include/status_type.h include/ability_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

overload_status.o : src/overload_status.cpp \
                    include/overload_status.h include/status.h \
                    include/status_type.h include/ability_type.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

burn_status.o : src/burn_status.cpp \
                include/burn_status.h include/status.h \
                include/status_type.h include/ability_type.h \
                include/robot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

shield_status.o : src/shield_status.cpp \
                  include/shield_status.h include/status.h \
                  include/status_type.h include/ability_type.h \
                  include/robot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	rm -f $(OBJECTS) game

.PHONY: clean