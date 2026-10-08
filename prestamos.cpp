#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
using namespace std;

// ===== CONSTANTES =====
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
int buscarLibroPorCodigo(const vector<Libro>& libros, string codigo);
bool existePrestamoActivo(const vector<Prestamo>& prestamos,
                          string idUsuario, string codigoLibro);
bool existeIdPrestamo(const vector<Prestamo>& prestamos, int idPrestamo);
string aMayusculas(string texto);
bool validarCodigoLibro(string codigo);
bool validarIdUsuario(string id);
void buscarPrestamosPorTitulo(const vector<Prestamo>& prestamos, const vector<Libro>& libros, string texto);
bool validarFecha(Fecha f);
bool fechaEsAnterior(Fecha a, Fecha b);
bool crearPrestamo(vector<Prestamo>& prestamos, vector<Libro>& libros,
                   int idPrestamo, string idUsuario, string codigoLibro, Fecha fecha);
bool registrarDevolucion(vector<Prestamo>& prestamos, vector<Libro>& libros,
                         int idPrestamo, Fecha fechaDevolucion);
void consultarPrestamos(const vector<Prestamo>& prestamos);
int contarPrestamosActivos(const vector<Prestamo>& prestamos, int cantidad);
void mostrarMatrizPorMes(const vector<Prestamo>& prestamos);
void guardarPrestamos(const vector<Prestamo>& prestamos);
int cargarPrestamos(vector<Prestamo>& prestamos);
void recalcularDisponibilidad(vector<Libro>& libros, const vector<Prestamo>& prestamos);
int iniciarPrestamos(vector<Prestamo>& prestamos, vector<Libro>& libros);

// ===== MÓDULO PRÉSTAMOS =====

// Busca un libro por su código y devuelve su posición (o -1 si no está)
int buscarLibroPorCodigo(const vector<Libro>& libros, string codigo) {
    int cantidad = libros.size();
    for (int i = 0; i < cantidad; i++) {
        if (libros[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// Revisa si el usuario ya tiene ese libro prestado y sin devolver
bool existePrestamoActivo(const vector<Prestamo>& prestamos,
                          string idUsuario, string codigoLibro) {
    int cantidad = prestamos.size();
    for (int i = 0; i < cantidad; i++) {
        if (prestamos[i].idUsuario == idUsuario &&
            prestamos[i].codigoLibro == codigoLibro &&
            prestamos[i].activo) {
            return true;
        }
    }
    return false;
}

// Revisa si ya existe un préstamo con ese número
bool existeIdPrestamo(const vector<Prestamo>& prestamos, int idPrestamo) {
    int cantidad = prestamos.size();
    for (int i = 0; i < cantidad; i++) {
        if (prestamos[i].idPrestamo == idPrestamo) {
            return true;
        }
    }
    return false;
}

// ----- CADENAS -----

// Convierte un texto a mayúsculas: "u001" pasa a "U001"
string aMayusculas(string texto) {
    int largo = texto.length();
    for (int i = 0; i < largo; i++) {
        texto[i] = toupper(texto[i]);
    }
    return texto;
}

// Código de libro válido (ISBN): solo números, guiones o X, entre 4 y 17 caracteres
bool validarCodigoLibro(string codigo) {
    int largo = codigo.length();
    if (largo < 4 || largo > 17) {
        return false;
    }
    for (int i = 0; i < largo; i++) {
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

// Busca los préstamos cuyo libro tiene el texto dado dentro de su título
// (sin importar si se escribe en mayúsculas o minúsculas)
void buscarPrestamosPorTitulo(const vector<Prestamo>& prestamos, const vector<Libro>& libros, string texto) {
    texto = aMayusculas(texto);
    cout << "\n--- PRÉSTAMOS CUYO LIBRO CONTIENE \"" << texto << "\" ---" << endl;

    int cantidad = prestamos.size();
    int encontrados = 0;

    for (int i = 0; i < cantidad; i++) {
        int pos = buscarLibroPorCodigo(libros, prestamos[i].codigoLibro);

        if (pos != -1) {
            string titulo = aMayusculas(libros[pos].titulo);

            // find devuelve string::npos cuando el texto no está dentro del título
            if (titulo.find(texto) != string::npos) {
                string estado;
                if (prestamos[i].activo) {
                    estado = "ACTIVO";
                } else {
                    estado = "DEVUELTO";
                }

                cout << "ID Préstamo: " << prestamos[i].idPrestamo
                     << " | Libro: " << libros[pos].titulo
                     << " | Usuario: " << prestamos[i].idUsuario
                     << " | Estado: " << estado << endl;
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {
        cout << "No se encontraron préstamos con ese título." << endl;
    }
}

// ----- FECHAS -----

// Fecha válida: mes entre 1 y 12, y día dentro de los días de ese mes
bool validarFecha(Fecha f) {
    if (f.anio < 2000 || f.anio > 2100) {
        return false;
    }
    if (f.mes < 1 || f.mes > 12) {
        return false;
    }

    // Días de cada mes (febrero se ajusta abajo si el año es bisiesto)
    int diasPorMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool esBisiesto = (f.anio % 4 == 0 && f.anio % 100 != 0) || (f.anio % 400 == 0);
    if (esBisiesto) {
        diasPorMes[1] = 29;
    }

    if (f.dia < 1 || f.dia > diasPorMes[f.mes - 1]) {
        return false;
    }
    return true;
}

// Devuelve true si la fecha a es anterior a la fecha b
bool fechaEsAnterior(Fecha a, Fecha b) {
    if (a.anio != b.anio) {
        return a.anio < b.anio;
    }
    if (a.mes != b.mes) {
        return a.mes < b.mes;
    }
    return a.dia < b.dia;
}

// ----- OPERACIONES -----

bool crearPrestamo(vector<Prestamo>& prestamos, vector<Libro>& libros,
                   int idPrestamo, string idUsuario, string codigoLibro, Fecha fecha) {

    if (prestamos.size() >= MAX_PRESTAMOS) {
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
    if (!validarFecha(fecha)) {
        cout << "[ERROR] Fecha de préstamo inválida." << endl;
        return false;
    }

    int pos = buscarLibroPorCodigo(libros, codigoLibro);
    if (pos == -1) {
        cout << "[ERROR] El libro " << codigoLibro << " no existe." << endl;
        return false;
    }

    if (libros[pos].cantidadDisponible <= 0) {
        cout << "[ERROR] No hay ejemplares disponibles de ese libro." << endl;
        return false;
    }

    if (existePrestamoActivo(prestamos, idUsuario, codigoLibro)) {
        cout << "[ERROR] El usuario ya tiene ese libro prestado." << endl;
        return false;
    }

    if (existeIdPrestamo(prestamos, idPrestamo)) {
        cout << "[ERROR] Ya existe un préstamo con el número " << idPrestamo << "." << endl;
        return false;
    }

    // Se arma el nuevo préstamo y se agrega al vector
    Prestamo nuevo;
    nuevo.idPrestamo = idPrestamo;
    nuevo.codigoLibro = codigoLibro;
    nuevo.idUsuario = idUsuario;
    nuevo.fechaPrestamo = fecha;
    nuevo.fechaDevolucion.dia = 0;
    nuevo.fechaDevolucion.mes = 0;
    nuevo.fechaDevolucion.anio = 0;
    nuevo.activo = true;
    prestamos.push_back(nuevo);

    // El préstamo afecta de inmediato la disponibilidad
    libros[pos].cantidadDisponible--;
    guardarPrestamos(prestamos);

    cout << "[ÉXITO] Préstamo " << idPrestamo << " registrado." << endl;
    return true;
}

bool registrarDevolucion(vector<Prestamo>& prestamos, vector<Libro>& libros,
                         int idPrestamo, Fecha fechaDevolucion) {

    if (!validarFecha(fechaDevolucion)) {
        cout << "[ERROR] Fecha de devolución inválida." << endl;
        return false;
    }

    int cantidad = prestamos.size();
    for (int i = 0; i < cantidad; i++) {
        if (prestamos[i].idPrestamo == idPrestamo && prestamos[i].activo) {

            if (fechaEsAnterior(fechaDevolucion, prestamos[i].fechaPrestamo)) {
                cout << "[ERROR] La fecha de devolución no puede ser anterior a la del préstamo." << endl;
                return false;
            }

            prestamos[i].activo = false;
            prestamos[i].fechaDevolucion = fechaDevolucion;

            int pos = buscarLibroPorCodigo(libros, prestamos[i].codigoLibro);
            if (pos != -1) {
                libros[pos].cantidadDisponible++;
            } else {
                cout << "[AVISO] No se encontró el libro del préstamo." << endl;
            }
            guardarPrestamos(prestamos);
            cout << "[ÉXITO] Préstamo " << idPrestamo << " devuelto." << endl;
            return true;
        }
    }

    cout << "[ERROR] No existe un préstamo activo con el número " << idPrestamo << "." << endl;
    return false;
}

// Muestra en consola el historial de préstamos
void consultarPrestamos(const vector<Prestamo>& prestamos) {
    cout << "\n--- HISTORIAL DE PRÉSTAMOS ---" << endl;

    int cantidad = prestamos.size();
    for (int i = 0; i < cantidad; i++) {
        string estado;
        if (prestamos[i].activo) {
            estado = "ACTIVO";
        } else {
            estado = "DEVUELTO";
        }

        cout << "ID Préstamo: " << prestamos[i].idPrestamo
             << " | Usuario: " << prestamos[i].idUsuario
             << " | Libro: " << prestamos[i].codigoLibro
             << " | Estado: " << estado << endl;
    }
}

// ----- RECURSIVIDAD -----

// Cuenta recursivamente los préstamos activos entre los primeros "cantidad" préstamos
int contarPrestamosActivos(const vector<Prestamo>& prestamos, int cantidad) {
    // CASO BASE: si no quedan préstamos por revisar, hay 0 activos
    if (cantidad == 0) {
        return 0;
    }
    // CASO RECURSIVO: reviso el último préstamo y sumo el resultado del resto
    int actual = 0;
    if (prestamos[cantidad - 1].activo) {
        actual = 1;
    }
    return actual + contarPrestamosActivos(prestamos, cantidad - 1);
}

// ----- MATRIZ -----

// Muestra cuántos préstamos hubo en cada mes, separados en activos y devueltos
void mostrarMatrizPorMes(const vector<Prestamo>& prestamos) {
    // 12 filas (una por mes) y 2 columnas (0 = activos, 1 = devueltos)
    int matriz[12][2] = {0};

    // Recorro los préstamos y sumo 1 en la celda que corresponda
    int cantidad = prestamos.size();
    for (int i = 0; i < cantidad; i++) {
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
void guardarPrestamos(const vector<Prestamo>& prestamos) {
    ofstream archivo("prestamos.txt");

    if (!archivo) {
        cout << "[ERROR] No se pudo abrir prestamos.txt para guardar." << endl;
        return;
    }

    int cantidad = prestamos.size();

    // Primera línea: cuántos préstamos hay
    archivo << cantidad << endl;

    // Una línea por préstamo, con los datos separados por espacios
    for (int i = 0; i < cantidad; i++) {
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

// Lee prestamos.txt y llena el vector. Devuelve cuántos préstamos cargó
int cargarPrestamos(vector<Prestamo>& prestamos) {
    ifstream archivo("prestamos.txt");

    // Si el archivo no existe todavía (primera vez), empezamos en 0
    if (!archivo) {
        return 0;
    }

    int cantidad;
    archivo >> cantidad;

    // Si la primera lectura falla (archivo vacío o dañado), empezamos en 0
    if (archivo.fail()) {
        return 0;
    }

    if (cantidad > MAX_PRESTAMOS) {
        cantidad = MAX_PRESTAMOS;
    }

    prestamos.clear();

    for (int i = 0; i < cantidad; i++) {
        Prestamo p;
        archivo >> p.idPrestamo
                >> p.codigoLibro
                >> p.idUsuario
                >> p.fechaPrestamo.dia
                >> p.fechaPrestamo.mes
                >> p.fechaPrestamo.anio
                >> p.fechaDevolucion.dia
                >> p.fechaDevolucion.mes
                >> p.fechaDevolucion.anio
                >> p.activo;

        // Si una línea está incompleta, dejamos de leer
        if (archivo.fail()) {
            break;
        }
        prestamos.push_back(p);
    }

    archivo.close();
    return prestamos.size();
}

// Después de cargar los archivos, recalcula cuántos ejemplares hay disponibles:
// parte del total y resta uno por cada préstamo activo de ese libro
void recalcularDisponibilidad(vector<Libro>& libros, const vector<Prestamo>& prestamos) {
    int cantidadLibros = libros.size();
    int cantidadPrestamos = prestamos.size();

    for (int i = 0; i < cantidadLibros; i++) {
        libros[i].cantidadDisponible = libros[i].cantidadTotal;
    }

    for (int j = 0; j < cantidadPrestamos; j++) {
        if (prestamos[j].activo) {
            int pos = buscarLibroPorCodigo(libros, prestamos[j].codigoLibro);
            if (pos != -1) {
                libros[pos].cantidadDisponible--;
            }
        }
    }
}

// Se llama una sola vez al iniciar el programa:
// carga prestamos.txt y deja la disponibilidad de los libros al día
int iniciarPrestamos(vector<Prestamo>& prestamos, vector<Libro>& libros) {
    int cargados = cargarPrestamos(prestamos);
    recalcularDisponibilidad(libros, prestamos);
    return cargados;
}