#ifndef REPORTES_H
#define REPORTES_H

#include <string>
#include <vector>

// --- GENERAR ESTADÍSTICAS ---
void generarReportePrestamos(std::string fechaInicio, std::string fechaFin);
void obtenerLibrosMasSolicitados(int limiteCantidad);

// --- INTEGRAR CONSULTAS ---
void filtrarRegistros(std::string criterio, std::string valor);

#endif