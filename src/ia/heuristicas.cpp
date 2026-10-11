// Heuristicas (ambas admisibles)
#include "ia/heuristicas.h"

int hSimple(const Estado& e) {
    return (e.m + e.c + 1) / 2;  // ceil(n / 2)
}

int hMejorada(const Estado& e) {
    int n = e.m + e.c;
    if (n == 0) return 0;
    if (e.b == 1) return n <= 2 ? 1 : 2 * n - 3;
    int n1 = n + 1;  // alguien debe volver
    return 1 + (n1 <= 2 ? 1 : 2 * n1 - 3);
}
