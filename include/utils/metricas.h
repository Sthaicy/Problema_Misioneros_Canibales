#ifndef METRICAS_H
#define METRICAS_H
#include <chrono>
#include <string>
#include <vector>
#include "core/estado.h"

using Reloj = std::chrono::steady_clock;

double msDesde(Reloj::time_point t0);
std::string nombreMetodo(const Resultado& r);

// Despachador: metodo = "bfs" | "astar"
Resultado resolver(const std::string& metodo, const std::string& heuristica = "simple");

// Ejecuta los 3 casos varias veces y promedia el tiempo
std::vector<Resultado> comparar(int repeticiones = 1000);

// AGREGA filas al CSV (crea la carpeta si no existe)
void guardarCSV(const std::vector<Resultado>& filas,
                const std::string& ruta = "reportes/resultados_misioneros.csv");

#endif
