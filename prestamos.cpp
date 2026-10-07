#include <iostream>
#include <string>
using namespace std;

// ===== CONSTANTES =====
const int MAX_LIBROS = 100;
const int MAX_USUARIOS = 100;
const int MAX_PRESTAMOS = 200;

// ===== STRUCTS COMPARTIDOS =====
struct Fecha {
    int dia;
    int mes;
    int anio;
};

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
    bool activo;
};

// ===== PROTOTIPOS =====
// Préstamos
int buscarLibro(const Libro libros[], int cantidadLibros, string codigo);
bool existePrestamoActivo(const Prestamo prestamos[], int cantidadPrestamos,
                          string idUsuario, string codigoLibro);
bool crearPrestamo(Prestamo prestamos[], int &cantidadPrestamos,
                   Libro libros[], int cantidadLibros,
                   int idPrestamo, string idUsuario, string codigoLibro, Fecha fecha);
bool registrarDevolucion(Prestamo prestamos[], int cantidadPrestamos,
                         Libro libros[], int cantidadLibros,
                         int idPrestamo, Fecha fechaDevolucion);

// ===== MÓDULO PRÉSTAMOS =====

int buscarLibro(const Libro libros[], int cantidadLibros, string codigo) {
    for (int i = 0; i < cantidadLibros; i++) {
        if (libros[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

bool existePrestamoActivo(const Prestamo prestamos[], int cantidadPrestamos,
                          string idUsuario, string codigoLibro) {
    for (int i = 0; i < cantidadPrestamos; i++) {
        if (prestamos[i].idUsuario == idUsuario &&
            prestamos[i].codigoLibro == codigoLibro &&
            prestamos[i].activo) {
            return true;
        }
    }
    return false;
}

bool crearPrestamo(Prestamo prestamos[], int &cantidadPrestamos,
                   Libro libros[], int cantidadLibros,
                   int idPrestamo, string idUsuario, string codigoLibro, Fecha fecha) {

    if (cantidadPrestamos >= MAX_PRESTAMOS) {
        cout << "[ERROR] No hay espacio para más préstamos." << endl;
        return false;
    }

    int pos = buscarLibro(libros, cantidadLibros, codigoLibro);
    if (pos == -1) {
        cout << "[ERROR] El libro " << codigoLibro << " no existe." << endl;
        return false;
    }

    if (libros[pos].cantidadDisponible <= 0) {
        cout << "[ERROR] No hay ejemplares disponibles de ese libro." << endl;
        return false;
    }

    if (existePrestamoActivo(prestamos, cantidadPrestamos, idUsuario, codigoLibro)) {
        cout << "[ERROR] El usuario ya tiene ese libro prestado." << endl;
        return false;
    }

    prestamos[cantidadPrestamos].idPrestamo = idPrestamo;
    prestamos[cantidadPrestamos].codigoLibro = codigoLibro;
    prestamos[cantidadPrestamos].idUsuario = idUsuario;
    prestamos[cantidadPrestamos].fechaPrestamo = fecha;
    prestamos[cantidadPrestamos].fechaDevolucion.dia = 0;
    prestamos[cantidadPrestamos].fechaDevolucion.mes = 0;
    prestamos[cantidadPrestamos].fechaDevolucion.anio = 0;
    prestamos[cantidadPrestamos].activo = true;

    cantidadPrestamos++;
    libros[pos].cantidadDisponible--;

    cout << "[ÉXITO] Préstamo " << idPrestamo << " registrado." << endl;
    return true;
}

bool registrarDevolucion(Prestamo prestamos[], int cantidadPrestamos,
                         Libro libros[], int cantidadLibros,
                         int idPrestamo, Fecha fechaDevolucion) {

    for (int i = 0; i < cantidadPrestamos; i++) {
        if (prestamos[i].idPrestamo == idPrestamo && prestamos[i].activo) {

            prestamos[i].activo = false;
            prestamos[i].fechaDevolucion = fechaDevolucion;

            int pos = buscarLibro(libros, cantidadLibros, prestamos[i].codigoLibro);
            if (pos != -1) {
                libros[pos].cantidadDisponible++;
            } else {
                cout << "[AVISO] No se encontró el libro del préstamo." << endl;
            }

            cout << "[ÉXITO] Préstamo " << idPrestamo << " devuelto." << endl;
            return true;
        }
    }

    cout << "[ERROR] No existe un préstamo activo con el número " << idPrestamo << "." << endl;
    return false;
}

// ===== MAIN (solo de prueba, se reemplaza al integrar) =====
int main() {
    Libro libros[MAX_LIBROS];
    int cantidadLibros = 2;
    libros[0].codigo = "L001";
    libros[0].titulo = "Algoritmos";
    libros[0].cantidadTotal = 2;
    libros[0].cantidadDisponible = 2;
    libros[1].codigo = "L002";
    libros[1].titulo = "Estructuras de Datos";
    libros[1].cantidadTotal = 1;
    libros[1].cantidadDisponible = 0;

    Prestamo prestamos[MAX_PRESTAMOS];
    int cantidadPrestamos = 0;

    Fecha hoy;
    hoy.dia = 8;
    hoy.mes = 10;
    hoy.anio = 2026;

    crearPrestamo(prestamos, cantidadPrestamos, libros, cantidadLibros, 1, "U01", "L001", hoy);
    crearPrestamo(prestamos, cantidadPrestamos, libros, cantidadLibros, 2, "U01", "L001", hoy);
    crearPrestamo(prestamos, cantidadPrestamos, libros, cantidadLibros, 3, "U02", "L999", hoy);
    crearPrestamo(prestamos, cantidadPrestamos, libros, cantidadLibros, 4, "U02", "L002", hoy);

    cout << "Préstamos: " << cantidadPrestamos << endl;
    cout << "Disponibles de L001: " << libros[0].cantidadDisponible << endl;

    registrarDevolucion(prestamos, cantidadPrestamos, libros, cantidadLibros, 1, hoy);
    registrarDevolucion(prestamos, cantidadPrestamos, libros, cantidadLibros, 1, hoy);
    registrarDevolucion(prestamos, cantidadPrestamos, libros, cantidadLibros, 99, hoy);

    cout << "Disponibles de L001 tras devolver: " << libros[0].cantidadDisponible << endl;
    return 0;
}
