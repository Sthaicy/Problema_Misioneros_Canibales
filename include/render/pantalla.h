#ifndef PANTALLA_H
#define PANTALLA_H
#include <vector>
#include "core/estado.h"

void habilitarANSI();
void alInterrumpir(int);
void imprimirResultado(const Resultado& r);
void imprimirTabla(const std::vector<Resultado>& filas);
void animar(const Resultado& r, int msPausa);

#endif
