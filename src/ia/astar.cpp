// Busqueda 2: A*   f(n) = g(n) + h(n)
#include "ia/astar.h"

#include <algorithm>
#include <array>
#include <functional>
#include <queue>
#include <tuple>
#include <vector>

#include "ia/heuristicas.h"
#include "utils/metricas.h"

using namespace std;

Resultado aEstrella(const string& nombreH) {
    int (*h)(const Estado&) = (nombreH == "mejorada") ? hMejorada : hSimple;

    auto t0 = Reloj::now();
    Resultado r;
    r.metodo = "A*";
    r.heuristica = nombreH;

    vector<Nodo> nodos;
    nodos.push_back({ESTADO_INICIAL, -1, 0, 0, 0, 0});
    r.generados = 1;

    // (f, contador de desempate, indice de nodo)  -> min-heap
    using Item = tuple<int, long long, int>;
    priority_queue<Item, vector<Item>, greater<Item>> frontera;
    long long contador = 0;
    frontera.push({h(ESTADO_INICIAL), contador, 0});

    array<int, N_CLAVES> mejorG;
    mejorG.fill(INF);
    mejorG[clave(ESTADO_INICIAL)] = 0;
    r.maxFrontera = 1;

    while (!frontera.empty()) {
        int idx = get<2>(frontera.top());
        frontera.pop();
        Nodo actual = nodos[idx];

        if (actual.g > mejorG[clave(actual.estado)]) continue;  // nodo obsoleto

        if (esMeta(actual.estado)) {  // test de meta al expandir
            r.exito = true;
            r.ruta = reconstruir(nodos, idx);
            r.costo = actual.g;
            r.profundidad = actual.profundidad;
            r.tiempoMs = msDesde(t0);
            return r;
        }
        r.expandidos++;

        for (const Sucesor& s : sucesores(actual.estado)) {
            int gNuevo = actual.g + 1;
            if (gNuevo < mejorG[clave(s.estado)]) {
                mejorG[clave(s.estado)] = gNuevo;
                nodos.push_back({s.estado, idx, s.dm, s.dc, gNuevo, actual.profundidad + 1});
                r.generados++;
                frontera.push({gNuevo + h(s.estado), ++contador, (int)nodos.size() - 1});
            }
        }
        r.maxFrontera = max(r.maxFrontera, frontera.size());
    }
    r.tiempoMs = msDesde(t0);
    return r;
}
