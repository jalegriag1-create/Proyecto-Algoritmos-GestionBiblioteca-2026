#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Registro anidado para la fecha
struct Fecha {
    int dia;
    int mes;
    int anio;
};

// Estructuras base del sistema
struct Libro {
    string codigo;
    string titulo;
    int cantidadTotal;
    int cantidadDisponible;
};

struct Usuario {
    string idUsuario;
    string nombre;
};

struct Prestamo {
    int idPrestamo;
    string codigoLibro;
    string idUsuario;
    Fecha fechaPrestamo;
    Fecha fechaDevolucion;
    bool activo; // true: Préstamo activo | false: Devuelto
};

// --- FUNCIONES AUXILIARES ---

// Busca un libro por su código y retorna su índice
int buscarLibroPorCodigo(const vector<Libro>& libros, const string& codigo) {
    for (size_t i = 0; i < libros.size(); ++i) {
        if (libros[i].codigo == codigo) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Verifica si el usuario ya tiene un préstamo activo del mismo libro
bool existePrestamoActivo(const vector<Prestamo>& prestamos, const string& idUsuario, const string& codigoLibro) {
    for (const auto& p : prestamos) {
        if (p.idUsuario == idUsuario && p.codigoLibro == codigoLibro && p.activo) {
            return true;
        }
    }
    return false;
}

// --- FUNCIONES DEL MÓDULO DE PRÉSTAMOS ---

// Registra un nuevo préstamo validando existencia, stock y duplicados
bool crearPrestamo(vector<Prestamo>& prestamos, vector<Libro>& libros, int idPrestamo, const string& idUsuario, const string& codigoLibro, Fecha fecha) {
    int idxLibro = buscarLibroPorCodigo(libros, codigoLibro);

    // 1. Validar existencia del libro
    if (idxLibro == -1) {
        cout << "[ERROR] El libro con código '" << codigoLibro << "' no existe." << endl;
        return false;
    }

    // 2. Validar disponibilidad de ejemplares
    if (libros[idxLibro].cantidadDisponible <= 0) {
        cout << "[ERROR] No hay ejemplares disponibles del libro '" << libros[idxLibro].titulo << "'." << endl;
        return false;
    }

    // 3. Validar préstamos duplicados activos del mismo ejemplar
    if (existePrestamoActivo(prestamos, idUsuario, codigoLibro)) {
        cout << "[ERROR] El usuario ya tiene un préstamo activo del libro '" << codigoLibro << "'." << endl;
        return false;
    }

    // Registrar nuevo préstamo
    Prestamo nuevoPrestamo;
    nuevoPrestamo.idPrestamo = idPrestamo;
    nuevoPrestamo.codigoLibro = codigoLibro;
    nuevoPrestamo.idUsuario = idUsuario;
    nuevoPrestamo.fechaPrestamo = fecha;
    nuevoPrestamo.fechaDevolucion = {0, 0, 0};
    nuevoPrestamo.activo = true;

    prestamos.push_back(nuevoPrestamo);

    // Actualizar inmediatamente la disponibilidad del libro
    libros[idxLibro].cantidadDisponible--;

    cout << "[ÉXITO] Préstamo #" << idPrestamo << " registrado correctamente." << endl;
    return true;
}

// Registra la devolución de un libro y actualiza el inventario
bool registrarDevolucion(vector<Prestamo>& prestamos, vector<Libro>& libros, int idPrestamo, Fecha fechaDevolucion) {
    for (auto& p : prestamos) {
        if (p.idPrestamo == idPrestamo && p.activo) {
            p.activo = false;
            p.fechaDevolucion = fechaDevolucion;

            // Restablecer la disponibilidad en el inventario
            int idxLibro = buscarLibroPorCodigo(libros, p.codigoLibro);
            if (idxLibro != -1) {
                libros[idxLibro].cantidadDisponible++;
            }

            cout << "[ÉXITO] Devolución del préstamo #" << idPrestamo << " registrada exitosamente." << endl;
            return true;
        }
    }

    cout << "[ERROR] No se encontró un préstamo activo con el ID " << idPrestamo << "." << endl;
    return false;
}

// Muestra en consola los préstamos registrados
void consultarPrestamos(const vector<Prestamo>& prestamos) {
    cout << "\n--- HISTORIAL DE PRÉSTAMOS ---" << endl;
    for (const auto& p : prestamos) {
        cout << "ID Préstamo: " << p.idPrestamo 
             << " | Usuario: " << p.idUsuario 
             << " | Libro: " << p.codigoLibro 
             << " | Estado: " << (p.activo ? "ACTIVO" : "DEVUELTO") << endl;
    }
}