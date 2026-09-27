CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude

LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

OBJECTS = main.o game.o game_window.o cell.o game_field.o \
          robot.o player_robot.o enemy_robot.o healer_robot.o renderer.o

game : $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDLIBS) -o $@

main.o : src/main.cpp include/game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

game.o : src/game.cpp include/game.h include/game_window.h \
         include/game_field.h include/renderer.h \
         include/player_robot.h include/enemy_robot.h \
         include/healer_robot.h include/game_state.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_window.o : src/game_window.cpp include/game_window.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

cell.o : src/cell.cpp include/cell.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

game_field.o : src/game_field.cpp include/game_field.h \
               include/cell.h include/robot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

robot.o : src/robot.cpp include/robot.h
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
             include/game_state.h include/robot.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	rm -f $(OBJECTS) game

.PHONY: clean