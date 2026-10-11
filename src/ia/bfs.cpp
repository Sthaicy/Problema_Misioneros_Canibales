// Busqueda 1: Primero en anchura (BFS)
#include "ia/bfs.h"

#include <algorithm>
#include <array>
#include <queue>
#include <vector>

#include "utils/metricas.h"

using namespace std;

Resultado bfs() {
    auto t0 = Reloj::now();
    Resultado r;
    r.metodo = "BFS";

    vector<Nodo> nodos;
    nodos.push_back({ESTADO_INICIAL, -1, 0, 0, 0, 0});
    r.generados = 1;

    queue<int> frontera;
    array<bool, N_CLAVES> visitado{};
    frontera.push(0);
    visitado[clave(ESTADO_INICIAL)] = true;
    r.maxFrontera = 1;

    while (!frontera.empty()) {
        int idx = frontera.front();
        frontera.pop();
        r.expandidos++;
        Nodo actual = nodos[idx];  // copia (el vector puede crecer)

        for (const Sucesor& s : sucesores(actual.estado)) {
            if (visitado[clave(s.estado)]) continue;
            nodos.push_back({s.estado, idx, s.dm, s.dc, actual.g + 1, actual.profundidad + 1});
            int hijo = (int)nodos.size() - 1;
            r.generados++;
            if (esMeta(s.estado)) {  // test de meta al generar
                r.maxFrontera = max(r.maxFrontera, frontera.size() + 1);
                r.exito = true;
                r.ruta = reconstruir(nodos, hijo);
                r.costo = nodos[hijo].g;
                r.profundidad = nodos[hijo].profundidad;
                r.tiempoMs = msDesde(t0);
                return r;
            }
            visitado[clave(s.estado)] = true;
            frontera.push(hijo);
        }
        r.maxFrontera = max(r.maxFrontera, frontera.size());
    }
    r.tiempoMs = msDesde(t0);
    return r;
}
