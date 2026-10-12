# 🛶 Problema de los Misioneros y Caníbales — Búsqueda en Inteligencia Artificial

Implementación en **C++** del problema clásico de los **Misioneros y Caníbales** resuelto mediante dos metodologías fundamentales de búsqueda en Inteligencia Artificial:
1. **Búsqueda No Informada (Ciega):** BFS (*Breadth-First Search* / Primero en Anchura).
2. **Búsqueda Informada (Heurística):** Algoritmo A\* (*A-Star*) evaluado con dos funciones heurísticas (Simple y Mejorada).

Incluye una **interfaz gráfica en consola (ASCII con colores ANSI)** con animación fluida del cruce del río paso a paso, un panel de métricas en tiempo real, un modo comparativo tipo benchmark y exportación automática de resultados a formato CSV.

---

## 📁 1. Estructura del Proyecto

El código está organizado siguiendo buenas prácticas de separación modular en C++:

```text
Problema_Misioneros_Canibales/
├── Makefile                # Script de compilación para Linux / macOS / MSYS2
├── compilar.bat            # Script de compilación rápida para Windows (g++)
├── README.md               # Esta guía
├── reportes/               # Carpeta donde se exporta resultados_misioneros.csv
├── docs/                   # Documentación adicional del proyecto
├── include/                # Archivos de cabecera (.h)
│   ├── colores.h           # Códigos ANSI para colores de consola
│   ├── core/
│   │   └── estado.h        # Modelo de estado (M,C,B), acciones, validación y transiciones
│   ├── ia/
│   │   ├── bfs.h           # Declaración del algoritmo BFS
│   │   ├── astar.h         # Declaración del algoritmo A*
│   │   └── heuristicas.h   # Funciones heurísticas simple y mejorada
│   ├── render/
│   │   └── pantalla.h      # Renderizador ASCII, animación y tablas
│   └── utils/
│       └── metricas.h      # Medición de tiempo, cronómetro y exportación a CSV
└── src/                    # Implementación de los módulos (.cpp)
    ├── main.cpp            # Menú interactivo y procesamiento de argumentos CLI
    ├── core/
    │   └── estado.cpp      # Lógica de validación, sucesores y reconstrucción de ruta
    ├── ia/
    │   ├── bfs.cpp         # Algoritmo BFS con arreglo de visitados en O(1)
    │   ├── astar.cpp       # Algoritmo A* con cola de prioridad y actualización de costos
    │   ├── heuristicas.cpp # Cálculo de hSimple y hMejorada
    │   ├── render/
    │   └── pantalla.cpp    # Animación fluida de la barca y tablero de métricas
    └── utils/
        └── metricas.cpp    # Banco de pruebas comparativo y generación de CSV
```

---

## 🛠️ 2. Requisitos y Compilación

### Requisitos:
* Compilador de C++ con soporte para **C++17** (ej. `g++` 8+ en MinGW-w64, Clang o MSVC).
* Terminal compatible con secuencias de escape ANSI (Windows Terminal, PowerShell, CMD en Windows 10/11, o terminales Linux/macOS).

### Compilar en Windows:
Desde la raíz del proyecto (`Problema_Misioneros_Canibales`), ejecuta:
```cmd
compilar.bat
```
*Esto creará la carpeta `build/` y generará el ejecutable `build\misioneros.exe`.*

### Compilar en Linux / macOS:
```bash
make
```

### Compilar manualmente con `g++`:
```bash
mkdir build
g++ -O2 -std=c++17 -Wall -Iinclude src/main.cpp src/core/estado.cpp src/ia/bfs.cpp src/ia/astar.cpp src/ia/heuristicas.cpp src/render/pantalla.cpp src/utils/metricas.cpp -o build/misioneros.exe
```

---

## 🚀 3. Guía de Ejecución: Cómo ver y comparar las 2 Metodologías

### Método A: Menú Interactivo (Recomendado)
Ejecuta el programa sin argumentos:
```cmd
build\misioneros.exe
```

Aparecerá el menú interactivo con las siguientes opciones:

```text
  ===== MISIONEROS Y CANIBALES - Busqueda en IA =====

   1. Resolver con BFS (primero en anchura)
   2. Resolver con A* (heuristica simple)
   3. Resolver con A* (heuristica mejorada)
   4. Comparar los 3 casos y exportar CSV
   0. Salir

  Opcion: 
```

* **Opción 1:** Ejecuta **BFS**, animando gráficamente el cruce paso a paso y mostrando al final la ruta de 11 pasos y las métricas.
* **Opción 2:** Ejecuta **A\* con heurística simple**, visualizando la animación y el desempeño del algoritmo informado básico.
* **Opción 3:** Ejecuta **A\* con heurística mejorada**, permitiendo observar cómo la heurística más informada guía la búsqueda de forma óptima.
* **Opción 4:** Corre una prueba comparativa automática (*benchmark* de 1000 iteraciones) que despliega una tabla resumen con todos los métodos frente a frente y actualiza `reportes/resultados_misioneros.csv`.

---

### Método B: Ejecución por Línea de Comandos (CLI)

El ejecutable cuenta con banderas para automatización, pruebas y visualización rápida:

| Comando | Descripción |
| :--- | :--- |
| `build\misioneros.exe --animar` | Ejecuta la animación directa (por defecto con BFS a 600 ms por paso). |
| `build\misioneros.exe --animar 300` | Ejecuta la animación acelerada a 300 milisegundos por paso. |
| `build\misioneros.exe --animar --metodo astar` | Ejecuta la animación resolviendo con **A\* (heurística simple)**. |
| `build\misioneros.exe --animar --metodo mejorada` | Ejecuta la animación resolviendo con **A\* (heurística mejorada)**. |
| `build\misioneros.exe --animar --metodo bfs` | Ejecuta la animación resolviendo con **BFS**. |
| `build\misioneros.exe --todo` | **Modo de evaluación completa:** Muestra la ruta de cada método, la tabla comparativa y guarda el archivo CSV sin necesidad de interactuar. |
| `build\misioneros.exe --sincolor` | Desactiva los colores y códigos ANSI (ideal para terminales antiguas o redirección a archivo de texto). |

---


### 📈 Interpretación de las Métricas:
* **Costo (11) y Profundidad (11):** Todos los métodos encuentran la solución óptima de 11 cruces, demostrando la admisibilidad de las heurísticas y la propiedad de optimalidad de BFS y A\*.
* **Nodos Expandidos y Generados:**
  * **BFS y A\* (h simple)** exploran una cantidad similar en este espacio pequeño.
  * **A\* (h mejorada)** reduce significativamente los nodos generados (de 31 a 23) y expandidos (de 13 a 9), demostrando el poder de poda de una heurística informada y más ajustada a la realidad del problema.
* **Máximo tamaño de la frontera:** Mide el consumo de memoria durante la búsqueda. A\* mejorada mantiene una frontera más reducida (5 frente a 8).
* **Exportación CSV:** Cada ejecución exitosa almacena sus datos en [resultados_misioneros.csv](reportes/resultados_misioneros.csv) para graficar o generar reportes posteriores en herramientas como Python, Excel o R.

---

----
## 🎨 4. Leyenda de la Animación en Consola

Durante la animación verás una representación en tiempo real del río:
* 🔵 **M** (Azul): Misionero
* 🔴 **C** (Rojo): Caníbal
* 🟡 **[ M C ]** (Amarillo): Barca con los tripulantes que van a bordo
* 🌊 **~~~~~** (Cian): Río entre ambas orillas
* 📊 **Panel Lateral:** Muestra en cada fotograma el método activo, número de cruce actual, costo acumulado y estado de la búsqueda.
