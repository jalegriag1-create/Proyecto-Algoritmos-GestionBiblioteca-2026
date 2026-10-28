#ifndef REPORTES_H
#define REPORTES_H

#include <string>

void reporteTotales();
void generarReportePrestamos(std::string fechaInicio, std::string fechaFin);
void obtenerLibrosMasSolicitados(int limiteCantidad);
void filtrarRegistros(std::string criterio, std::string valor);

#endif