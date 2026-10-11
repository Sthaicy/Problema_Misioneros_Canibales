#ifndef HEURISTICAS_H
#define HEURISTICAS_H
#include "core/estado.h"

int hSimple(const Estado& e);    // ceil((M+C)/2)
int hMejorada(const Estado& e);  // considera el regreso de la barca

#endif
