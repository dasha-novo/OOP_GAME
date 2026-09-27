game : main.o game.o game_window.o cell.o game_field.o robot.o player_robot.o enemy_robot.o healer_robot.o renderer.o
	g++ main.o game.o game_window.o cell.o game_field.o robot.o player_robot.o enemy_robot.o healer_robot.o renderer.o -lsfml-graphics -lsfml-window -lsfml-system -o game

main.o : main.cpp game.h
	g++ -c main.cpp -o main.o

game.o : game.cpp game.h game_window.h game_field.h renderer.h player_robot.h enemy_robot.h healer_robot.h game_state.h
	g++ -c game.cpp -o game.o

game_window.o : game_window.cpp game_window.h
	g++ -c game_window.cpp -o game_window.o

cell.o : cell.cpp cell.h
	g++ -c cell.cpp -o cell.o

game_field.o : game_field.cpp game_field.h cell.h robot.h
	g++ -c game_field.cpp -o game_field.o

robot.o : robot.cpp robot.h
	g++ -c robot.cpp -o robot.o

player_robot.o : player_robot.cpp player_robot.h robot.h
	g++ -c player_robot.cpp -o player_robot.o

enemy_robot.o : enemy_robot.cpp enemy_robot.h robot.h
	g++ -c enemy_robot.cpp -o enemy_robot.o

healer_robot.o : healer_robot.cpp healer_robot.h robot.h
	g++ -c healer_robot.cpp -o healer_robot.o

renderer.o : renderer.cpp renderer.h game_field.h game_window.h game_state.h robot.h
	g++ -c renderer.cpp -o renderer.o

clean :
	rm -f *.o game