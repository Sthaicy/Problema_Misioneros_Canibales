# 🛶 Problema de los Misioneros y Caníbales — Búsqueda en Inteligencia Artificial

Implementación en **C++** del problema clásico de los **Misioneros y Caníbales** resuelto mediante dos metodologías fundamentales de búsqueda en Inteligencia Artificial:
1. **Búsqueda No Informada (Ciega):** BFS (*Breadth-First Search* / Primero en Anchura).
2. **Búsqueda Informada (Heurística):** Algoritmo A\* (*A-Star*) evaluado con dos funciones heurísticas (Simple y Mejorada).

Incluye una **interfaz gráfica en consola (ASCII con colores ANSI)** con animación fluida del cruce del río paso a paso, un panel de métricas en tiempo real, un modo comparativo tipo benchmark y exportación automática de resultados a formato CSV.

---

## 📋 1. Descripción del Problema

Tres misioneros (**M**) y tres caníbales (**C**) están en la orilla izquierda de un río y desean cruzar a la orilla derecha utilizando una barca (**B**) que puede transportar como máximo a **dos personas** a la vez. La barca no puede cruzar vacía; debe haber al menos una persona a bordo para remar.

### ⚠️ Restricción Crítica:
En ninguna orilla el número de caníbales puede superar al de misioneros si hay al menos un misionero presente, de lo contrario los caníbales se comerían a los misioneros:
* **Orilla Izquierda:** Si $M_{izq} > 0 \implies C_{izq} \le M_{izq}$
* **Orilla Derecha:** Si $M_{der} > 0 \implies C_{der} \le M_{der}$

### 🎯 Definición Formal del Espacio de Estados:
* **Estado:** Tríada $(M, C, B)$, donde:
  * $M \in \{0, 1, 2, 3\}$: Misioneros en la orilla izquierda.
  * $C \in \{0, 1, 2, 3\}$: Caníbales en la orilla izquierda.
  * $B \in \{1, 0\}$: Posición de la barca ($1 =$ izquierda, $0 =$ derecha).
  *(Los valores en la orilla derecha son implícitos: $3 - M$ y $3 - C$)*.
* **Estado Inicial:** $(3, 3, 1)$ — Todos en la orilla izquierda.
* **Estado Meta:** $(0, 0, 0)$ — Todos en la orilla derecha.
* **Acciones Posibles:** Cruzar con 1 o 2 personas: $(1, 0)$, $(0, 1)$, $(2, 0)$, $(0, 2)$, $(1, 1)$.
* **Costo de la Solución Óptima:** **11 cruces** (profundidad 11).

---

## 🧠 2. Las Dos Metodologías de Búsqueda

| Metodología | Tipo | Algoritmo | Función de Evaluación / Estrategia | Garantías |
| :--- | :--- | :--- | :--- | :--- |
| **Metodología 1** | No Informada (Ciega) | **BFS** | Explora por niveles con cola FIFO. Test de meta al generar. | Completo y óptimo en número de pasos para costos uniformes. |
| **Metodología 2** | Informada (Heurística) | **A\*** | Minimiza $f(n) = g(n) + h(n)$ usando min-heap. Test de meta al expandir. | Óptimo y completo si $h(n)$ es admisible y consistente. |

### 🔍 Heurísticas implementadas para A\*:
1. **Heurística Simple ($h_{simple}$):**
   $$h(e) = \left\lceil \frac{M + C}{2} \right\rceil$$
   Calcula la cantidad mínima de viajes considerando que la barca lleva a lo sumo 2 personas. Es admisible pero optimista, ya que no contempla que alguien debe regresar en la barca ni las restricciones de seguridad.
2. **Heurística Mejorada ($h_{mejorada}$):**
   Toma en cuenta los viajes de ida y vuelta requeridos para trasladar a $n = M + C$ personas con capacidad 2.
   * Si la barca está a la izquierda ($B=1$): si $n \le 2$ requiere 1 viaje; si $n > 2$ requiere al menos $2n - 3$ cruces.
   * Si la barca está a la derecha ($B=0$): alguien debe regresar primero con la barca.
   * Es una heurística más informada ($h_{mejorada}(n) \ge h_{simple}(n)$) que poda significativamente el espacio de búsqueda expandiendo menos nodos.

---

## 📁 3. Estructura del Proyecto

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

## 🛠️ 4. Requisitos y Compilación

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

## 🚀 5. Guía de Ejecución: Cómo ver y comparar las 2 Metodologías

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

## 📊 6. Comparativa de Resultados y Métricas

Al seleccionar la **Opción 4** del menú o ejecutar con la bandera `--todo`, obtendrás una tabla comparativa como la siguiente:

```text
============================================================================================
EVALUACION COMPARATIVA (C++)
============================================================================================
Metodo               Costo  Prof.  Generados  Expandidos  Max.front.   Tiempo(ms)
BFS                     11     11         31          13           8      0.01500
A* (h simple)           11     11         31          13           8      0.02200
A* (h mejorada)         11     11         23           9           5      0.01600
```

### 📈 Interpretación Pedagógica de las Métricas:
* **Costo (11) y Profundidad (11):** Todos los métodos encuentran la solución óptima de 11 cruces, demostrando la admisibilidad de las heurísticas y la propiedad de optimalidad de BFS y A\*.
* **Nodos Expandidos y Generados:**
  * **BFS y A\* (h simple)** exploran una cantidad similar en este espacio pequeño.
  * **A\* (h mejorada)** reduce significativamente los nodos generados (de 31 a 23) y expandidos (de 13 a 9), demostrando el poder de poda de una heurística informada y más ajustada a la realidad del problema.
* **Máximo tamaño de la frontera:** Mide el consumo de memoria durante la búsqueda. A\* mejorada mantiene una frontera más reducida (5 frente a 8).
* **Exportación CSV:** Cada ejecución exitosa almacena sus datos en [resultados_misioneros.csv](reportes/resultados_misioneros.csv) para graficar o generar reportes posteriores en herramientas como Python, Excel o R.

---

## 🎨 7. Leyenda de la Animación en Consola

Durante la animación verás una representación en tiempo real del río:
* 🔵 **M** (Azul): Misionero
* 🔴 **C** (Rojo): Caníbal
* 🟡 **[ M C ]** (Amarillo): Barca con los tripulantes que van a bordo
* 🌊 **~~~~~** (Cian): Río entre ambas orillas
* 📊 **Panel Lateral:** Muestra en cada fotograma el método activo, número de cruce actual, costo acumulado y estado de la búsqueda.
