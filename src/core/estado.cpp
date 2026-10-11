// Modelo del problema: restriccion, sucesores, test de meta y reconstruccion de ruta
#include "core/estado.h"

#include <algorithm>
#include <array>
#include <utility>

using namespace std;

bool esValido(const Estado& e) {
    if (e.m < 0 || e.m > TOTAL_M || e.c < 0 || e.c > TOTAL_C) return false;
    int mDer = TOTAL_M - e.m, cDer = TOTAL_C - e.c;
    if (e.m > 0 && e.c > e.m) return false;
    if (mDer > 0 && cDer > mDer) return false;
    return true;
}

bool esMeta(const Estado& e) { return e == ESTADO_META; }

static const array<pair<int, int>, 5> ACCIONES = {{{1, 0}, {0, 1}, {2, 0}, {0, 2}, {1, 1}}};

vector<Sucesor> sucesores(const Estado& e) {
    vector<Sucesor> res;
    for (auto [dm, dc] : ACCIONES) {
        if (dm + dc < 1 || dm + dc > CAPACIDAD_BARCA) continue;
        Estado nuevo = (e.b == 1) ? Estado{e.m - dm, e.c - dc, 0}
                                  : Estado{e.m + dm, e.c + dc, 1};
        if (esValido(nuevo)) res.push_back({dm, dc, nuevo});
    }
    return res;
}

string describir(int dm, int dc, const Estado& origen) {
    string s = "Cruzan ";
    if (dm) s += to_string(dm) + (dm > 1 ? " misioneros" : " misionero");
    if (dm && dc) s += " y ";
    if (dc) s += to_string(dc) + (dc > 1 ? " canibales" : " canibal");
    s += origen.b == 1 ? " (izquierda -> derecha)" : " (derecha -> izquierda)";
    return s;
}

vector<Paso> reconstruir(const vector<Nodo>& nodos, int idx) {
    vector<int> cadena;
    for (int i = idx; i != -1; i = nodos[i].padre) cadena.push_back(i);
    reverse(cadena.begin(), cadena.end());
    vector<Paso> ruta;
    for (size_t k = 0; k < cadena.size(); ++k) {
        const Nodo& n = nodos[cadena[k]];
        string desc = (k == 0) ? "Estado inicial"
                               : describir(n.dm, n.dc, nodos[cadena[k - 1]].estado);
        ruta.push_back({n.estado, desc});
    }
    return ruta;
}
