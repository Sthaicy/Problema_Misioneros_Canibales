#ifndef COLORES_H
#define COLORES_H
#include <string>

// true = usa secuencias ANSI (colores, cursor); false con --sincolor
inline bool ANSI = true;

inline std::string pintar(const std::string& codigo, const std::string& t) {
    return ANSI ? "\033[" + codigo + "m" + t + "\033[0m" : t;
}
inline std::string azul(const std::string& t)     { return pintar("1;34", t); }
inline std::string rojo(const std::string& t)     { return pintar("1;31", t); }
inline std::string cian(const std::string& t)     { return pintar("36", t); }
inline std::string amarillo(const std::string& t) { return pintar("1;33", t); }
inline std::string verde(const std::string& t)    { return pintar("32", t); }

#endif
