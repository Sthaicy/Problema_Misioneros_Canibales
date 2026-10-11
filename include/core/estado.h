#ifndef ESTADO_H
#define ESTADO_H
#include <cstddef>
#include <limits>
#include <string>
#include <vector>

constexpr int TOTAL_M = 3;
constexpr int TOTAL_C = 3;
constexpr int CAPACIDAD_BARCA = 2;
constexpr int INF = std::numeric_limits<int>::max();

// Estado (M, C, B): misioneros y canibales en la orilla izquierda; B=1 barca a la izquierda
struct Estado {
    int m, c, b;
    bool operator==(const Estado& o) const { return m == o.m && c == o.c && b == o.b; }
};

inline const Estado ESTADO_INICIAL{TOTAL_M, TOTAL_C, 1};
inline const Estado ESTADO_META{0, 0, 0};

// Cada estado posible se codifica en un indice 0..31 (para tablas rapidas)
inline int clave(const Estado& e) { return (e.m * 4 + e.c) * 2 + e.b; }
constexpr int N_CLAVES = 32;

bool esValido(const Estado& e);
bool esMeta(const Estado& e);

struct Sucesor {
    int dm, dc;
    Estado estado;
};
std::vector<Sucesor> sucesores(const Estado& e);

// Nodo del arbol de busqueda, paso de la ruta y resultado de una busqueda
struct Nodo {
    Estado estado;
    int padre;   // indice en el vector de nodos (-1 = raiz)
    int dm, dc;  // accion que llevo hasta este nodo
    int g;
    int profundidad;
};

struct Paso {
    Estado estado;
    std::string descripcion;
};

struct Resultado {
    std::string metodo;
    std::string heuristica;  // vacio en BFS
    bool exito = false;
    std::vector<Paso> ruta;
    int costo = -1;
    int profundidad = -1;
    long long generados = 0;
    long long expandidos = 0;
    std::size_t maxFrontera = 0;
    double tiempoMs = 0.0;
};

std::string describir(int dm, int dc, const Estado& origen);
std::vector<Paso> reconstruir(const std::vector<Nodo>& nodos, int idx);

#endif
