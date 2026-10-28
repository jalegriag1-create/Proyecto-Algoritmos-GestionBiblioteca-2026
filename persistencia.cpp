#include "persistencia.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool guardarDatos(std::string nombreArchivo, std::string datos) {
    std::ofstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        return false;
    }
    archivo << datos;
    return archivo.good();
}

std::string cargarDatos(std::string nombreArchivo) {
    std::ifstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        return "";
    }
    std::stringstream contenido;
    contenido << archivo.rdbuf();
    return contenido.str();
}
