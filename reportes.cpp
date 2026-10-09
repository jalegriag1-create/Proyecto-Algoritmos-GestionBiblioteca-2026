#include "reportes.h"
#include <iostream>

using namespace std;

// --- VARIABLES Y ESTRUCTURAS TEMPORALES ---
int nLibros = 0;
int nPrestamos = 0;
const int MAX = 100;

struct LibroTemp {
    int total;
    int disponibles;
    int veces;
    string titulo;
} libros[100];

struct PrestamoTemp {
    int codigoLibro;
    int idUsuario;
    string fecha;
    bool activo;
} prestamos[100];

struct UsuarioTemp {
    string nombre;
} usuarios[100];

int buscarLibro(int codigo) { return codigo; }
int buscarUsuario(int id) { return id; }

// --- 1. GENERAR ESTADÍSTICAS ---

int sumarDisponibles(int i) {
    if (i == nLibros) {
        return 0;
    }
    return libros[i].disponibles + sumarDisponibles(i + 1);
}

void reporteTotales() {
    int total = 0;
    for (int i = 0; i < nLibros; i++) {
        total = total + libros[i].total;
    }
    cout << "Ejemplares en total: " << total << "\n";
    cout << "Ejemplares disponibles: " << sumarDisponibles(0) << "\n";
}

void mostrarPrestamo(int i) {
    int pl = buscarLibro(prestamos[i].codigoLibro);
    int pu = buscarUsuario(prestamos[i].idUsuario);
    cout << prestamos[i].fecha << " | " << usuarios[pu].nombre
         << " | " << libros[pl].titulo << " | ";
    if (prestamos[i].activo) {
        cout << "Sin devolver\n";
    } else {
        cout << "Devuelto\n";
    }
}

void generarReportePrestamos(string fechaInicio, string fechaFin) {
    int cantidad = 0;
    for (int i = 0; i < nPrestamos; i++) {
        if (prestamos[i].fecha >= fechaInicio && prestamos[i].fecha <= fechaFin) {
            mostrarPrestamo(i);
            cantidad++;
        }
    }
    cout << "Prestamos en ese periodo: " << cantidad << "\n";
}

void obtenerLibrosMasSolicitados(int limiteCantidad) {
    if (nLibros == 0 || limiteCantidad <= 0) {
        cout << "No hay datos para mostrar.\n";
        return;
    }
    int orden[MAX];
    for (int i = 0; i < nLibros; i++) {
        orden[i] = i;
    }
    for (int i = 0; i < nLibros - 1; i++) {
        for (int j = 0; j < nLibros - 1 - i; j++) {
            if (libros[orden[j]].veces < libros[orden[j + 1]].veces) {
                int temp = orden[j];
                orden[j] = orden[j + 1];
                orden[j + 1] = temp;
            }
        }
    }
    if (limiteCantidad > nLibros) {
        limiteCantidad = nLibros;
    }
    for (int i = 0; i < limiteCantidad; i++) {
        cout << i + 1 << ". " << libros[orden[i]].titulo
             << " (" << libros[orden[i]].veces << " prestamos)\n";
    }
}

// --- 3. INTEGRAR CONSULTAS ---

void filtrarRegistros(string criterio, string valor) {
    int encontrados = 0;
    for (int i = 0; i < nPrestamos; i++) {
        bool coincide = false;
        if (criterio == "usuario") {
            coincide = (to_string(prestamos[i].idUsuario) == valor);
        } else if (criterio == "libro") {
            coincide = (to_string(prestamos[i].codigoLibro) == valor);
        } else if (criterio == "estado") {
            if (valor == "activo") {
                coincide = prestamos[i].activo;
            } else if (valor == "devuelto") {
                coincide = !prestamos[i].activo;
            }
        }
        if (coincide) {
            mostrarPrestamo(i);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        cout << "No se encontraron registros.\n";
    }
}





























