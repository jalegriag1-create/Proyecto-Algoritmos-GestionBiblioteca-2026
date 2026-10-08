#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

// ===== CONSTANTES =====
const int MAX_LIBROS = 100;
const int MAX_PRESTAMOS = 200;

// ===== STRUCTS =====
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
int buscarLibroPorCodigo(const Libro libros[], int cantidadLibros, string codigo);
bool existePrestamoActivo(const Prestamo prestamos[], int cantidadPrestamos,
                          string idUsuario, string codigoLibro);
bool existeIdPrestamo(const Prestamo prestamos[], int cantidadPrestamos, int idPrestamo);
string aMayusculas(string texto);
bool validarCodigoLibro(string codigo);
bool validarIdUsuario(string id);
void buscarPrestamosPorTitulo(const Prestamo prestamos[], int cantidadPrestamos,
                              const Libro libros[], int cantidadLibros, string texto);
bool validarFecha(Fecha f);
bool fechaEsAnterior(Fecha a, Fecha b);
bool crearPrestamo(Prestamo prestamos[], int &cantidadPrestamos,
                   Libro libros[], int cantidadLibros,
                   int idPrestamo, string idUsuario, string codigoLibro, Fecha fecha);
bool registrarDevolucion(Prestamo prestamos[], int cantidadPrestamos,
                         Libro libros[], int cantidadLibros,
                         int idPrestamo, Fecha fechaDevolucion);
void consultarPrestamos(const Prestamo prestamos[], int cantidadPrestamos);
void mostrarPrestamosActivos(const Prestamo prestamos[], int cantidadPrestamos,
                             const Libro libros[], int cantidadLibros);
void mostrarPrestamosPorUsuario(const Prestamo prestamos[], int cantidadPrestamos, string idUsuario);
void mostrarLibroMasPrestado(const Prestamo prestamos[], int cantidadPrestamos,
                             const Libro libros[], int cantidadLibros);
void mostrarCantidadLibros(const Libro libros[], int cantidadLibros);
int contarPrestamosActivos(const Prestamo prestamos[], int cantidadPrestamos);
void mostrarMatrizPorMes(const Prestamo prestamos[], int cantidadPrestamos);
void guardarPrestamos(const Prestamo prestamos[], int cantidadPrestamos);
int cargarPrestamos(Prestamo prestamos[]);
void recalcularDisponibilidad(Libro libros[], int cantidadLibros,
                              const Prestamo prestamos[], int cantidadPrestamos);
int iniciarPrestamos(Prestamo prestamos[], int &cantidadPrestamos,
                     Libro libros[], int cantidadLibros);

// ===== BÚSQUEDAS BÁSICAS =====

// Busca un libro por su código y devuelve su posición (o -1 si no está)
int buscarLibroPorCodigo(const Libro libros[], int cantidadLibros, string codigo) {
    for (int i = 0; i < cantidadLibros; i++) {
        if (libros[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// Revisa si el usuario ya tiene ese libro prestado y sin devolver
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

// Revisa si ya existe un préstamo con ese número
bool existeIdPrestamo(const Prestamo prestamos[], int cantidadPrestamos, int idPrestamo) {
    for (int i = 0; i < cantidadPrestamos; i++) {
        if (prestamos[i].idPrestamo == idPrestamo) {
            return true;
        }
    }
    return false;
}

// ===== CADENAS =====

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
void buscarPrestamosPorTitulo(const Prestamo prestamos[], int cantidadPrestamos,
                              const Libro libros[], int cantidadLibros, string texto) {
    texto = aMayusculas(texto);
    cout << "\n--- PRÉSTAMOS CUYO LIBRO CONTIENE \"" << texto << "\" ---" << endl;

    int encontrados = 0;

    for (int i = 0; i < cantidadPrestamos; i++) {
        int pos = buscarLibroPorCodigo(libros, cantidadLibros, prestamos[i].codigoLibro);

        if (pos != -1) {
            string titulo = aMayusculas(libros[pos].titulo);

            // find devuelve string::npos cuando el texto NO está dentro del título
            if (titulo.find(texto) != string::npos) {
                string estado = "DEVUELTO";
                if (prestamos[i].activo) {
                    estado = "ACTIVO";
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

// ===== FECHAS =====

// Fecha válida: mes entre 1 y 12, y día dentro de los días de ese mes
bool validarFecha(Fecha f) {
    if (f.anio < 2000 || f.anio > 2100) {
        return false;
    }
    if (f.mes < 1 || f.mes > 12) {
        return false;
    }

    int diasPorMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // En año bisiesto, febrero tiene 29 días
    if ((f.anio % 4 == 0 && f.anio % 100 != 0) || f.anio % 400 == 0) {
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

// ===== OPERACIONES =====

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
    if (!validarFecha(fecha)) {
        cout << "[ERROR] Fecha de préstamo inválida." << endl;
        return false;
    }

    int pos = buscarLibroPorCodigo(libros, cantidadLibros, codigoLibro);
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

    if (existeIdPrestamo(prestamos, cantidadPrestamos, idPrestamo)) {
        cout << "[ERROR] Ya existe un préstamo con el número " << idPrestamo << "." << endl;
        return false;
    }

    // Se guarda el nuevo préstamo en la siguiente posición libre del arreglo
    prestamos[cantidadPrestamos].idPrestamo = idPrestamo;
    prestamos[cantidadPrestamos].codigoLibro = codigoLibro;
    prestamos[cantidadPrestamos].idUsuario = idUsuario;
    prestamos[cantidadPrestamos].fechaPrestamo = fecha;
    prestamos[cantidadPrestamos].fechaDevolucion.dia = 0;
    prestamos[cantidadPrestamos].fechaDevolucion.mes = 0;
    prestamos[cantidadPrestamos].fechaDevolucion.anio = 0;
    prestamos[cantidadPrestamos].activo = true;
    cantidadPrestamos++;

    // El préstamo afecta de inmediato la disponibilidad
    libros[pos].cantidadDisponible--;
    guardarPrestamos(prestamos, cantidadPrestamos);

    cout << "[ÉXITO] Préstamo " << idPrestamo << " registrado." << endl;
    return true;
}

bool registrarDevolucion(Prestamo prestamos[], int cantidadPrestamos,
                         Libro libros[], int cantidadLibros,
                         int idPrestamo, Fecha fechaDevolucion) {

    if (!validarFecha(fechaDevolucion)) {
        cout << "[ERROR] Fecha de devolución inválida." << endl;
        return false;
    }

    for (int i = 0; i < cantidadPrestamos; i++) {
        if (prestamos[i].idPrestamo == idPrestamo && prestamos[i].activo) {

            if (fechaEsAnterior(fechaDevolucion, prestamos[i].fechaPrestamo)) {
                cout << "[ERROR] La fecha de devolución no puede ser anterior a la del préstamo." << endl;
                return false;
            }

            // Cambia el estado del préstamo a devuelto
            prestamos[i].activo = false;
            prestamos[i].fechaDevolucion = fechaDevolucion;

            // Devuelve el ejemplar al libro
            int pos = buscarLibroPorCodigo(libros, cantidadLibros, prestamos[i].codigoLibro);
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

// Muestra en consola el historial de préstamos
void consultarPrestamos(const Prestamo prestamos[], int cantidadPrestamos) {
    cout << "\n--- HISTORIAL DE PRÉSTAMOS ---" << endl;

    for (int i = 0; i < cantidadPrestamos; i++) {
        string estado = "DEVUELTO";
        if (prestamos[i].activo) {
            estado = "ACTIVO";
        }

        cout << "ID Préstamo: " << prestamos[i].idPrestamo
             << " | Usuario: " << prestamos[i].idUsuario
             << " | Libro: " << prestamos[i].codigoLibro
             << " | Estado: " << estado << endl;
    }
}

// ===== REPORTES Y CONSULTAS =====

// Muestra solo los préstamos que siguen activos (libros prestados actualmente)
void mostrarPrestamosActivos(const Prestamo prestamos[], int cantidadPrestamos,
                             const Libro libros[], int cantidadLibros) {
    cout << "\n--- LIBROS PRESTADOS ACTUALMENTE ---" << endl;

    int encontrados = 0;

    for (int i = 0; i < cantidadPrestamos; i++) {
        if (prestamos[i].activo) {
            // Busco el título del libro para mostrarlo
            string titulo = prestamos[i].codigoLibro;
            int pos = buscarLibroPorCodigo(libros, cantidadLibros, prestamos[i].codigoLibro);
            if (pos != -1) {
                titulo = libros[pos].titulo;
            }

            cout << "ID Préstamo: " << prestamos[i].idPrestamo
                 << " | Libro: " << titulo
                 << " | Usuario: " << prestamos[i].idUsuario
                 << " | Fecha: " << prestamos[i].fechaPrestamo.dia << "/"
                 << prestamos[i].fechaPrestamo.mes << "/"
                 << prestamos[i].fechaPrestamo.anio << endl;
            encontrados++;
        }
    }

    if (encontrados == 0) {
        cout << "No hay libros prestados actualmente." << endl;
    }
}

// Muestra el historial de préstamos de un solo usuario
void mostrarPrestamosPorUsuario(const Prestamo prestamos[], int cantidadPrestamos, string idUsuario) {
    idUsuario = aMayusculas(idUsuario);
    cout << "\n--- PRÉSTAMOS DEL USUARIO " << idUsuario << " ---" << endl;

    int encontrados = 0;

    for (int i = 0; i < cantidadPrestamos; i++) {
        if (prestamos[i].idUsuario == idUsuario) {
            string estado = "DEVUELTO";
            if (prestamos[i].activo) {
                estado = "ACTIVO";
            }

            cout << "ID Préstamo: " << prestamos[i].idPrestamo
                 << " | Libro: " << prestamos[i].codigoLibro
                 << " | Estado: " << estado << endl;
            encontrados++;
        }
    }

    if (encontrados == 0) {
        cout << "Este usuario no tiene préstamos registrados." << endl;
    }
}

// Muestra el libro con más préstamos (el de mayor movimiento)
void mostrarLibroMasPrestado(const Prestamo prestamos[], int cantidadPrestamos,
                             const Libro libros[], int cantidadLibros) {
    cout << "\n--- LIBRO CON MAYOR MOVIMIENTO ---" << endl;

    if (cantidadPrestamos == 0) {
        cout << "Todavía no hay préstamos registrados." << endl;
        return;
    }

    string codigoMayor = "";
    int mayor = 0;

    // Para cada préstamo cuento cuántos préstamos tienen el mismo libro
    for (int i = 0; i < cantidadPrestamos; i++) {
        int veces = 0;
        for (int j = 0; j < cantidadPrestamos; j++) {
            if (prestamos[j].codigoLibro == prestamos[i].codigoLibro) {
                veces++;
            }
        }
        if (veces > mayor) {
            mayor = veces;
            codigoMayor = prestamos[i].codigoLibro;
        }
    }

    string titulo = codigoMayor;
    int pos = buscarLibroPorCodigo(libros, cantidadLibros, codigoMayor);
    if (pos != -1) {
        titulo = libros[pos].titulo;
    }

    cout << "Libro: " << titulo << " (" << codigoMayor << ")" << endl;
    cout << "Veces prestado: " << mayor << endl;
}

// Muestra la cantidad total y disponible de cada libro, y los totales generales
void mostrarCantidadLibros(const Libro libros[], int cantidadLibros) {
    cout << "\n--- CANTIDAD TOTAL Y DISPONIBLE DE LIBROS ---" << endl;

    if (cantidadLibros == 0) {
        cout << "No hay libros registrados." << endl;
        return;
    }

    int sumaTotal = 0;
    int sumaDisponible = 0;

    for (int i = 0; i < cantidadLibros; i++) {
        cout << "Libro: " << libros[i].titulo
             << " (" << libros[i].codigo << ")"
             << " | Total: " << libros[i].cantidadTotal
             << " | Disponibles: " << libros[i].cantidadDisponible << endl;

        sumaTotal = sumaTotal + libros[i].cantidadTotal;
        sumaDisponible = sumaDisponible + libros[i].cantidadDisponible;
    }

    cout << "TOTAL de ejemplares: " << sumaTotal
         << " | Disponibles: " << sumaDisponible << endl;
}

// ===== RECURSIVIDAD =====

// Cuenta recursivamente los préstamos activos entre los primeros "cantidadPrestamos"
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

// ===== MATRIZ =====

// Muestra cuántos préstamos hubo en cada mes, separados en activos y devueltos
void mostrarMatrizPorMes(const Prestamo prestamos[], int cantidadPrestamos) {
    // 12 filas (una por mes) y 2 columnas (0 = activos, 1 = devueltos)
    int matriz[12][2] = {0};

    // Recorro los préstamos y sumo 1 en la celda que corresponda
    for (int i = 0; i < cantidadPrestamos; i++) {
        int mes = prestamos[i].fechaPrestamo.mes - 1;   // el mes 1 va en la fila 0

        if (mes >= 0 && mes < 12) {
            if (prestamos[i].activo) {
                matriz[mes][0]++;
            } else {
                matriz[mes][1]++;
            }
        }
    }

    // Imprimo la matriz
    cout << "\n=== PRÉSTAMOS POR MES ===" << endl;
    cout << "Mes\tActivos\tDevueltos" << endl;
    for (int i = 0; i < 12; i++) {
        cout << (i + 1) << "\t" << matriz[i][0] << "\t" << matriz[i][1] << endl;
    }
}

// ===== ARCHIVOS =====

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

    int cantidad = 0;
    archivo >> cantidad;

    if (cantidad > MAX_PRESTAMOS) {
        cantidad = MAX_PRESTAMOS;
    }

    int leidos = 0;

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

        // Si una línea está incompleta, dejamos de leer
        if (archivo.fail()) {
            break;
        }
        leidos++;
    }

    archivo.close();
    return leidos;
}

// Después de cargar los archivos, recalcula cuántos ejemplares hay disponibles:
// parte del total y resta uno por cada préstamo activo de ese libro
void recalcularDisponibilidad(Libro libros[], int cantidadLibros,
                              const Prestamo prestamos[], int cantidadPrestamos) {
    for (int i = 0; i < cantidadLibros; i++) {
        libros[i].cantidadDisponible = libros[i].cantidadTotal;
    }

    for (int j = 0; j < cantidadPrestamos; j++) {
        if (prestamos[j].activo) {
            int pos = buscarLibroPorCodigo(libros, cantidadLibros, prestamos[j].codigoLibro);
            if (pos != -1) {
                libros[pos].cantidadDisponible--;
            }
        }
    }
}

// Se llama una sola vez al iniciar el programa:
// carga prestamos.txt y deja la disponibilidad de los libros al día
int iniciarPrestamos(Prestamo prestamos[], int &cantidadPrestamos,
                     Libro libros[], int cantidadLibros) {
    cantidadPrestamos = cargarPrestamos(prestamos);
    recalcularDisponibilidad(libros, cantidadLibros, prestamos, cantidadPrestamos);
    return cantidadPrestamos;
}       