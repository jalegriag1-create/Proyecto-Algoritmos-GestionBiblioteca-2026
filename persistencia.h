#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>

// --- GUARDAR Y CARGAR ARCHIVOS ---
bool guardarDatos(std::string nombreArchivo, std::string datos);
std::string cargarDatos(std::string nombreArchivo);

#endif