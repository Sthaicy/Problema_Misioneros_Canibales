// Cronometro, despachador de metodos, comparacion y exportacion a CSV
#include "utils/metricas.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <stdexcept>

#include "ia/astar.h"
#include "ia/bfs.h"

using namespace std;

double msDesde(Reloj::time_point t0) {
    return chrono::duration<double, milli>(Reloj::now() - t0).count();
}

string nombreMetodo(const Resultado& r) {
    return r.metodo + (r.heuristica.empty() ? "" : " (h " + r.heuristica + ")");
}

Resultado resolver(const string& metodo, const string& heuristica) {
    if (metodo == "bfs") return bfs();
    if (metodo == "astar") return aEstrella(heuristica);
    throw invalid_argument("Metodo desconocido: " + metodo);
}

vector<Resultado> comparar(int repeticiones) {
    vector<pair<string, string>> configs = {{"bfs", ""}, {"astar", "simple"}, {"astar", "mejorada"}};
    vector<Resultado> filas;
    for (auto& [metodo, heur] : configs) {
        Resultado r = resolver(metodo, heur.empty() ? "simple" : heur);
        double total = 0;
        for (int i = 0; i < repeticiones; ++i) total += resolver(metodo, heur.empty() ? "simple" : heur).tiempoMs;
        r.tiempoMs = total / repeticiones;
        filas.push_back(r);
    }
    return filas;
}

// AGREGA filas; las 4 primeras columnas coinciden con Pac-Man
void guardarCSV(const vector<Resultado>& filas, const string& ruta) {
    filesystem::path p(ruta);
    if (p.has_parent_path()) filesystem::create_directories(p.parent_path());

    bool existe;
    {
        ifstream f(ruta);
        existe = f.good();
    }
    ofstream out(ruta, ios::app);
    if (!existe)
        out << "Algoritmo,Nodos_Expandidos,Tiempo_ms,Resultado,Nodos_Generados,Costo,Profundidad,Frontera_Max\n";
    for (const auto& r : filas) {
        out << nombreMetodo(r) << "," << r.expandidos << "," << fixed << setprecision(5) << r.tiempoMs << ","
            << (r.exito ? "Solucion en " + to_string(r.costo) + " cruces" : "Sin solucion") << ","
            << r.generados << "," << r.costo << "," << r.profundidad << "," << r.maxFrontera << "\n";
    }
}
