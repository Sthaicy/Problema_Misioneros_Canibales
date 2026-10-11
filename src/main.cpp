/*
 * Misioneros y canibales - BFS y A* (menu y argumentos de linea de comandos)
 *
 * Ejecutar desde la raiz del proyecto:
 *   build/misioneros                       menu interactivo
 *   build/misioneros --animar [ms]         animacion (por defecto BFS)
 *   build/misioneros --animar --metodo astar | mejorada | bfs
 *   build/misioneros --todo                los 3 casos + tabla + CSV
 *   build/misioneros --sincolor            sin colores ni secuencias ANSI
 * El CSV se AGREGA en reportes/resultados_misioneros.csv
 */
#include <cctype>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>

#include "colores.h"
#include "render/pantalla.h"
#include "utils/metricas.h"

using namespace std;

static Resultado resolverPorNombre(const string& nombre) {
    if (nombre == "bfs") return resolver("bfs");
    if (nombre == "mejorada") return resolver("astar", "mejorada");
    return resolver("astar", "simple");
}

static void pausa() {
    cout << "\nPresione Enter para continuar...";
    string s;
    getline(cin, s);
}

static void ejecutarYAnimar(const string& nombre, int ms) {
    Resultado r = resolverPorNombre(nombre);
    animar(r, ms);
    imprimirResultado(r);
    guardarCSV({r});
    cout << "Fila agregada a reportes/resultados_misioneros.csv\n";
}

static void menu(int ms) {
    while (true) {
        if (ANSI) cout << "\033[2J\033[H";
        cout << "\n  ===== MISIONEROS Y CANIBALES - Busqueda en IA =====\n\n"
             << "   1. Resolver con BFS (primero en anchura)\n"
             << "   2. Resolver con A* (heuristica simple)\n"
             << "   3. Resolver con A* (heuristica mejorada)\n"
             << "   4. Comparar los 3 casos y exportar CSV\n"
             << "   0. Salir\n\n  Opcion: " << flush;
        string linea;
        if (!getline(cin, linea) || linea == "0") return;
        if (linea == "1") { ejecutarYAnimar("bfs", ms); pausa(); }
        else if (linea == "2") { ejecutarYAnimar("simple", ms); pausa(); }
        else if (linea == "3") { ejecutarYAnimar("mejorada", ms); pausa(); }
        else if (linea == "4") {
            vector<Resultado> filas = comparar();
            cout << "\n";
            imprimirTabla(filas);
            guardarCSV(filas);
            cout << "\nFilas agregadas a reportes/resultados_misioneros.csv\n";
            pausa();
        }
    }
}

int main(int argc, char** argv) {
    habilitarANSI();
    signal(SIGINT, alInterrumpir);

    bool modoAnimar = false, modoTodo = false;
    int ms = 600;
    string metodo = "bfs";
    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "--sincolor") ANSI = false;
        else if (a == "--todo") modoTodo = true;
        else if (a == "--metodo" && i + 1 < argc) metodo = argv[++i];
        else if (a == "--animar") {
            modoAnimar = true;
            if (i + 1 < argc && isdigit((unsigned char)argv[i + 1][0])) ms = stoi(argv[++i]);
        }
    }

    if (modoAnimar) {
        ejecutarYAnimar(metodo, ms);
        return 0;
    }
    if (modoTodo) {
        imprimirResultado(resolver("bfs"));
        imprimirResultado(resolver("astar", "simple"));
        imprimirResultado(resolver("astar", "mejorada"));
        auto tabla = comparar();
        imprimirTabla(tabla);
        guardarCSV(tabla);
        cout << "\nFilas agregadas a reportes/resultados_misioneros.csv\n";
        return 0;
    }
    menu(ms);
    return 0;
}
