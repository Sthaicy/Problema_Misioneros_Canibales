// Salida por consola: tablas, animacion ASCII con colores ANSI y panel de metricas
#include "render/pantalla.h"

#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "colores.h"
#include "utils/metricas.h"

using namespace std;

void habilitarANSI() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &modo))
        SetConsoleMode(h, modo | 0x0004);  // ENABLE_VIRTUAL_TERMINAL_PROCESSING
#endif
}

void alInterrumpir(int) {  // Ctrl+C: devolver el cursor antes de salir
    cout << "\033[?25h\033[0m\n" << flush;
    _Exit(0);
}

// ---------------------------------------------------------------------------
// Tablas
// ---------------------------------------------------------------------------
void imprimirResultado(const Resultado& r) {
    string linea(64, '=');
    cout << linea << "\nMetodo: " << nombreMetodo(r) << "\n" << linea << "\n";
    if (!r.exito) {
        cout << "No se encontro solucion.\n";
        return;
    }
    for (size_t i = 0; i < r.ruta.size(); ++i) {
        const Estado& e = r.ruta[i].estado;
        cout << setw(2) << i << ". (" << e.m << ", " << e.c << ", " << e.b << ")  "
             << r.ruta[i].descripcion << "\n";
    }
    cout << string(64, '-') << "\n"
         << "Costo de la ruta (cruces) : " << r.costo << "\n"
         << "Profundidad               : " << r.profundidad << "\n"
         << "Nodos generados           : " << r.generados << "\n"
         << "Nodos expandidos          : " << r.expandidos << "\n"
         << "Tamano max. de frontera   : " << r.maxFrontera << "\n"
         << fixed << setprecision(4)
         << "Tiempo                    : " << r.tiempoMs << " ms\n\n";
}

void imprimirTabla(const vector<Resultado>& filas) {
    cout << string(92, '=') << "\nEVALUACION COMPARATIVA (C++)\n" << string(92, '=') << "\n";
    cout << left << setw(20) << "Metodo" << right << setw(7) << "Costo" << setw(7) << "Prof."
         << setw(11) << "Generados" << setw(12) << "Expandidos" << setw(12) << "Max.front."
         << setw(12) << "Tiempo(ms)" << "\n";
    for (const auto& r : filas) {
        cout << left << setw(20) << nombreMetodo(r) << right << setw(7) << r.costo << setw(7)
             << r.profundidad << setw(11) << r.generados << setw(12) << r.expandidos << setw(12)
             << r.maxFrontera << setw(12) << fixed << setprecision(5) << r.tiempoMs << "\n";
    }
}

// ---------------------------------------------------------------------------
// Animacion (sin parpadeo + panel de metricas)
// ---------------------------------------------------------------------------
namespace {

constexpr int RIO = 30;        // ancho del rio en columnas
constexpr int COL_DER = 48;    // columna donde empieza la orilla derecha
constexpr int COL_PANEL = 66;  // columna donde empieza el panel de metricas

// cantidad de caracteres visibles (ignora secuencias \033[...m)
size_t anchoVisible(const string& s) {
    size_t n = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\033') {
            while (i < s.size() && s[i] != 'm') ++i;
        } else {
            ++n;
        }
    }
    return n;
}

string personas(int m, int c) {  // ocupa siempre 14 columnas visibles
    string s;
    for (int i = 0; i < m; ++i) s += azul("M") + " ";
    for (int i = 0; i < c; ++i) s += rojo("C") + " ";
    s += string(14 - 2 * (m + c), ' ');
    return s;
}

// desp = desplazamiento de la barca (columnas) dentro del rio
// pm, pc = pasajeros que van en la barca;  paso = numero de paso mostrado
string construirFrame(const Resultado& r, int mi, int ci, int md, int cd, int desp, int pm, int pc,
                      const string& titulo, int paso, const Estado& e) {
    string barca = amarillo("[");
    if (pm + pc == 0) barca += amarillo("__");
    for (int i = 0; i < pm; ++i) barca += azul("M") + " ";
    for (int i = 0; i < pc; ++i) barca += rojo("C") + " ";
    barca += amarillo("]");

    string cuentaIzq = "M=" + to_string(mi) + " C=" + to_string(ci);
    string cuentaDer = "M=" + to_string(md) + " C=" + to_string(cd);

    vector<string> L;
    L.push_back("  MISIONEROS Y CANIBALES - Prototipo IA (BFS y A*)");
    L.push_back("");
    L.push_back("  " + verde("ORILLA IZQUIERDA") + string(COL_DER - 18, ' ') + verde("ORILLA DERECHA"));
    L.push_back("  " + personas(mi, ci) + cian(string(RIO, '~')) + "  " + personas(md, cd));
    L.push_back("  " + string(14, ' ') + string(desp, ' ') + barca);
    L.push_back("  " + string(14, ' ') + cian(string(RIO, '~')));
    L.push_back("  " + cuentaIzq + string(COL_DER - 2 - cuentaIzq.size(), ' ') + cuentaDer);
    while (L.size() < 11) L.push_back("");

    ostringstream t;
    t << fixed << setprecision(4) << r.tiempoMs << " ms";
    int total = (int)r.ruta.size() - 1;
    vector<string> panel = {
        amarillo("RESULTADOS DE LA BUSQUEDA"),
        "Metodo        : " + nombreMetodo(r),
        "Costo (cruces): " + to_string(r.costo),
        "Profundidad   : " + to_string(r.profundidad),
        "Generados     : " + to_string(r.generados),
        "Expandidos    : " + to_string(r.expandidos),
        "Frontera max. : " + to_string(r.maxFrontera),
        "Tiempo        : " + t.str(),
        "Paso          : " + to_string(paso) + " / " + to_string(total)};
    for (size_t k = 0; k < panel.size(); ++k) {
        string& linea = L[2 + k];
        linea += string(COL_PANEL - min<size_t>(COL_PANEL, anchoVisible(linea)), ' ') + panel[k];
    }

    L.push_back("");
    L.push_back("  " + titulo);
    L.push_back("  Estado (M,C,B) = (" + to_string(e.m) + "," + to_string(e.c) + "," + to_string(e.b) + ")");
    L.push_back("  Leyenda: " + azul("M") + "=Misionero  " + rojo("C") + "=Canibal  " + cian("~") +
                "=Rio  " + amarillo("[ ]") + "=Barca");

    string sal;
    if (ANSI) sal += "\033[H";                       // cursor a (0,0): sin parpadeo
    for (const auto& linea : L) sal += linea + (ANSI ? "\033[K" : "") + "\n";  // borra resto de la linea
    if (ANSI) sal += "\033[J";                       // borra lo que sobre debajo
    return sal;
}

void mostrarEstado(const Resultado& r, int i) {
    const Estado& e = r.ruta[i].estado;
    int desp = (e.b == 1) ? 0 : RIO - 4;  // barca vacia mide 4 columnas
    string titulo = (i == 0) ? "Estado inicial" : "Paso " + to_string(i) + "/" + to_string((int)r.ruta.size() - 1) +
                                                   ": " + r.ruta[i].descripcion;
    cout << construirFrame(r, e.m, e.c, TOTAL_M - e.m, TOTAL_C - e.c, desp, 0, 0, titulo, i, e) << flush;
}

}  // namespace

void animar(const Resultado& r, int msPausa) {
    if (!r.exito) {
        cout << "No hay solucion que mostrar.\n";
        return;
    }
    auto dormir = [](int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); };
    const int CUADROS = 8, MS_CUADRO = 50;
    int total = (int)r.ruta.size() - 1;

    if (ANSI) cout << "\033[2J\033[?25l";  // limpiar UNA vez y ocultar el cursor
    mostrarEstado(r, 0);
    for (int i = 1; i <= total; ++i) {
        dormir(msPausa);
        const Estado& a = r.ruta[i - 1].estado;
        const Estado& b = r.ruta[i].estado;
        int pm = abs(a.m - b.m), pc = abs(a.c - b.c);
        string titulo = "Paso " + to_string(i) + "/" + to_string(total) + ": " + r.ruta[i].descripcion;

        // orillas sin los pasajeros que ya subieron a la barca
        int mi = a.b == 1 ? a.m - pm : a.m, ci = a.b == 1 ? a.c - pc : a.c;
        int md = TOTAL_M - a.m - (a.b == 0 ? pm : 0), cd = TOTAL_C - a.c - (a.b == 0 ? pc : 0);

        int maxDesp = RIO - (2 + 2 * (pm + pc));
        int desde = a.b == 1 ? 0 : maxDesp, hasta = a.b == 1 ? maxDesp : 0;
        for (int k = 0; k <= CUADROS; ++k) {
            int desp = desde + (hasta - desde) * k / CUADROS;
            cout << construirFrame(r, mi, ci, md, cd, desp, pm, pc, titulo, i - 1, a) << flush;
            dormir(MS_CUADRO);
        }
        mostrarEstado(r, i);
    }
    if (ANSI) cout << "\033[?25h";  // devolver el cursor
    cout << "\n  " << verde("Meta alcanzada en " + to_string(total) + " cruces.") << "\n\n";
}
