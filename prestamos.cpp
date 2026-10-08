#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
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
int buscarLibro(const Libro libros[], int cantidadLibros, string codigo);
bool existePrestamoActivo(const Prestamo prestamos[], int cantidadPrestamos,
                          string idUsuario, string codigoLibro);
string aMayusculas(string texto);
bool validarCodigoLibro(string codigo);
bool validarIdUsuario(string id);
bool crearPrestamo(Prestamo prestamos[], int &cantidadPrestamos,
                   Libro libros[], int cantidadLibros,
                   int idPrestamo, string idUsuario, string codigoLibro, Fecha fecha);
bool registrarDevolucion(Prestamo prestamos[], int cantidadPrestamos,
                         Libro libros[], int cantidadLibros,
                         int idPrestamo, Fecha fechaDevolucion);
int contarPrestamosActivos(const Prestamo prestamos[], int cantidadPrestamos);
void mostrarMatrizPorMes(const Prestamo prestamos[], int cantidadPrestamos);
void guardarPrestamos(const Prestamo prestamos[], int cantidadPrestamos);
int cargarPrestamos(Prestamo prestamos[]);

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

// ----- CADENAS -----

// Convierte un texto a mayúsculas: "l001" pasa a "L001"
string aMayusculas(string texto) {
    for (int i = 0; i < texto.length(); i++) {
        texto[i] = toupper(texto[i]);
    }
    return texto;
}

// Código de libro válido (ISBN): solo números, guiones o X, entre 4 y 17 caracteres
bool validarCodigoLibro(string codigo) {
    if (codigo.length() < 4 || codigo.length() > 17) {
        return false;
    }
    for (int i = 0; i < codigo.length(); i++) {
        char c = codigo[i];
        bool esNumero = (c >= '0' && c <= '9');
        if (!esNumero && c != '-' && c != 'X') {
            return false;
        }
    }
    return true;
}

// ID de usuario válido: una U y 3 números (ejemplo: U001)
bool validarIdUsuario(string id) {
    if (id.length() != 4) {
        return false;
    }
    if (id[0] != 'U') {
        return false;
    }
    for (int i = 1; i < 4; i++) {
        if (id[i] < '0' || id[i] > '9') {
            return false;
        }
    }
    return true;
}

// ----- OPERACIONES -----

bool crearPrestamo(Prestamo prestamos[], int &cantidadPrestamos,
                   Libro libros[], int cantidadLibros,
                   int idPrestamo, string idUsuario, string codigoLibro, Fecha fecha) {

    if (cantidadPrestamos >= MAX_PRESTAMOS) {
        cout << "[ERROR] No hay espacio para más préstamos." << endl;
        return false;
    }

    // Normalizar y validar los identificadores
    codigoLibro = aMayusculas(codigoLibro);
    idUsuario = aMayusculas(idUsuario);

    if (!validarCodigoLibro(codigoLibro)) {
        cout << "[ERROR] Código de libro inválido: " << codigoLibro << endl;
        return false;
    }
    if (!validarIdUsuario(idUsuario)) {
        cout << "[ERROR] ID de usuario inválido: " << idUsuario << endl;
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
    guardarPrestamos(prestamos, cantidadPrestamos);

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
            guardarPrestamos(prestamos, cantidadPrestamos);
            cout << "[ÉXITO] Préstamo " << idPrestamo << " devuelto." << endl;
            return true;
        }
    }

    cout << "[ERROR] No existe un préstamo activo con el número " << idPrestamo << "." << endl;
    return false;
}

// ----- RECURSIVIDAD -----

// Cuenta recursivamente los préstamos activos
int contarPrestamosActivos(const Prestamo prestamos[], int cantidadPrestamos) {
    // CASO BASE: si no quedan préstamos por revisar, hay 0 activos
    if (cantidadPrestamos == 0) {
        return 0;
    }
    // CASO RECURSIVO: reviso el último préstamo y sumo el resultado del resto
    int actual = 0;
    if (prestamos[cantidadPrestamos - 1].activo) {
        actual = 1;
    }
    return actual + contarPrestamosActivos(prestamos, cantidadPrestamos - 1);
}

// ----- MATRIZ -----

// Muestra cuántos préstamos hubo en cada mes, separados en activos y devueltos
void mostrarMatrizPorMes(const Prestamo prestamos[], int cantidadPrestamos) {
    // 12 filas (una por mes) y 2 columnas (0 = activos, 1 = devueltos)
    int matriz[12][2] = {0};

    // Recorro los préstamos y sumo 1 en la celda que corresponda
    for (int i = 0; i < cantidadPrestamos; i++) {
        int mes = prestamos[i].fechaPrestamo.mes - 1;   // mes 1 va en la fila 0

        if (mes >= 0 && mes < 12) {
            if (prestamos[i].activo) {
                matriz[mes][0]++;
            } else {
                matriz[mes][1]++;
            }
        }
    }

    // Imprimo la matriz
    cout << "\n=== PRESTAMOS POR MES ===" << endl;
    cout << "Mes\tActivos\tDevueltos" << endl;
    for (int i = 0; i < 12; i++) {
        cout << (i + 1) << "\t" << matriz[i][0] << "\t" << matriz[i][1] << endl;
    }
}

// ----- ARCHIVOS -----

// Guarda todos los préstamos en prestamos.txt
void guardarPrestamos(const Prestamo prestamos[], int cantidadPrestamos) {
    ofstream archivo("prestamos.txt");

    if (!archivo) {
        cout << "[ERROR] No se pudo abrir prestamos.txt para guardar." << endl;
        return;
    }

    // Primera línea: cuántos préstamos hay
    archivo << cantidadPrestamos << endl;

    // Una línea por préstamo, con los datos separados por espacios
    for (int i = 0; i < cantidadPrestamos; i++) {
        archivo << prestamos[i].idPrestamo << " "
                << prestamos[i].codigoLibro << " "
                << prestamos[i].idUsuario << " "
                << prestamos[i].fechaPrestamo.dia << " "
                << prestamos[i].fechaPrestamo.mes << " "
                << prestamos[i].fechaPrestamo.anio << " "
                << prestamos[i].fechaDevolucion.dia << " "
                << prestamos[i].fechaDevolucion.mes << " "
                << prestamos[i].fechaDevolucion.anio << " "
                << prestamos[i].activo << endl;
    }

    archivo.close();
}

// Lee prestamos.txt y llena el arreglo. Devuelve cuántos préstamos cargó
int cargarPrestamos(Prestamo prestamos[]) {
    ifstream archivo("prestamos.txt");

    // Si el archivo no existe todavía (primera vez), empezamos en 0
    if (!archivo) {
        return 0;
    }

    int cantidad;
    archivo >> cantidad;

    if (cantidad > MAX_PRESTAMOS) {
        cantidad = MAX_PRESTAMOS;
    }

    for (int i = 0; i < cantidad; i++) {
        archivo >> prestamos[i].idPrestamo
                >> prestamos[i].codigoLibro
                >> prestamos[i].idUsuario
                >> prestamos[i].fechaPrestamo.dia
                >> prestamos[i].fechaPrestamo.mes
                >> prestamos[i].fechaPrestamo.anio
                >> prestamos[i].fechaDevolucion.dia
                >> prestamos[i].fechaDevolucion.mes
                >> prestamos[i].fechaDevolucion.anio
                >> prestamos[i].activo;
    }

    archivo.close();
    return cantidad;
}