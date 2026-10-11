@echo off
if not exist build mkdir build
g++ -O2 -std=c++17 -Wall -Iinclude src\main.cpp src\core\estado.cpp src\ia\bfs.cpp src\ia\astar.cpp src\ia\heuristicas.cpp src\render\pantalla.cpp src\utils\metricas.cpp -o build\misioneros.exe
