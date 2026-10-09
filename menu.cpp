#include "menu.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

struct Biblioteca {
    moduloCatalogo::Catalogo catalogo;
    vector<moduloUsuarios::Usuario> usuarios;
    moduloPrestamos::Libro libros[moduloPrestamos::MAX_LIBROS];
    int cantidadLibros;
    moduloPrestamos::Prestamo prestamos[moduloPrestamos::MAX_PRESTAMOS];
    int cantidadPrestamos;
};

bool leerLinea(const string& mensaje, string& destino) {
    cout << mensaje;
    if (!getline(cin, destino)) {
        return false;
    }
    return true;
}

bool leerEntero(const string& mensaje, int& valor) {
    string linea;
    if (!leerLinea(mensaje, linea)) {
        valor = 0;
        return false;
    }
    try {
        size_t usados = 0;
        valor = stoi(linea, &usados);
        while (usados < linea.size() && linea[usados] == ' ') {
            usados++;
        }
        if (usados != linea.size()) {
            valor = -1;
        }
    } catch (...) {
        valor = -1;
    }
    return true;
}

bool leerFecha(const string& mensaje, moduloPrestamos::Fecha& fecha) {
    string texto;
    if (!leerLinea(mensaje, texto)) {
        return false;
    }
    istringstream entrada(texto);
    int dia, mes, anio;
    char barra1, barra2;
    string resto;
    if (!(entrada >> dia >> barra1 >> mes >> barra2 >> anio) || barra1 != '/' || barra2 != '/' || (entrada >> resto)) {
        cout << "Formato de fecha invalido. Use DD/MM/AAAA.\n";
        return false;
    }
    fecha.dia = dia;
    fecha.mes = mes;
    fecha.anio = anio;
    if (!moduloPrestamos::validarFecha(fecha)) {
        cout << "La fecha no existe.\n";
        return false;
    }
    return true;
}

string fechaATexto(const moduloPrestamos::Fecha& fecha) {
    ostringstream salida;
    salida << setfill('0') << setw(4) << fecha.anio << "-" << setw(2) << fecha.mes << "-" << setw(2) << fecha.dia;
    return salida.str();
}

int indiceUsuario(const Biblioteca& b, const string& identificador) {
    for (int i = 0; i < (int)b.usuarios.size(); i++) {
        if (b.usuarios[i].identificador == identificador) {
            return i;
        }
    }
    return -1;
}

int siguienteIdPrestamo(const Biblioteca& b) {
    int mayor = 0;
    for (int i = 0; i < b.cantidadPrestamos; i++) {
        if (b.prestamos[i].idPrestamo > mayor) {
            mayor = b.prestamos[i].idPrestamo;
        }
    }
    return mayor + 1;
}

bool agregarLibro(Biblioteca& b, string isbn, const string& titulo, const string& autor, const string& signatura, int cantidad) {
    isbn = moduloPrestamos::aMayusculas(isbn);
    if (!moduloPrestamos::validarCodigoLibro(isbn)) {
        cout << "Error: ISBN invalido. Use numeros, guiones o X (4 a 17 caracteres).\n";
        return false;
    }
    if (moduloPrestamos::buscarLibroPorCodigo(b.libros, b.cantidadLibros, isbn) != -1) {
        cout << "Error: el libro con ISBN '" << isbn << "' ya se encuentra registrado.\n";
        return false;
    }
    if (b.cantidadLibros >= moduloPrestamos::MAX_LIBROS) {
        cout << "Error: el catalogo esta lleno.\n";
        return false;
    }
    if (cantidad < 1) {
        cout << "Error: la cantidad de ejemplares debe ser al menos 1.\n";
        return false;
    }
    b.catalogo.registrarLibro(isbn, titulo, autor, signatura);
    b.libros[b.cantidadLibros].codigo = isbn;
    b.libros[b.cantidadLibros].titulo = titulo;
    b.libros[b.cantidadLibros].cantidadTotal = cantidad;
    b.libros[b.cantidadLibros].cantidadDisponible = cantidad;
    b.cantidadLibros++;
    return true;
}

void sincronizarReportes(const Biblioteca& b) {
    int nl = b.cantidadLibros < moduloReportes::MAX ? b.cantidadLibros : moduloReportes::MAX;
    int nu = (int)b.usuarios.size() < moduloReportes::MAX ? (int)b.usuarios.size() : moduloReportes::MAX;
    moduloReportes::nLibros = nl;
    for (int i = 0; i < nl; i++) {
        moduloReportes::libros[i].total = b.libros[i].cantidadTotal;
        moduloReportes::libros[i].disponibles = b.libros[i].cantidadDisponible;
        moduloReportes::libros[i].titulo = b.libros[i].titulo;
        moduloReportes::libros[i].veces = 0;
    }
    for (int i = 0; i < nu; i++) {
        moduloReportes::usuarios[i].nombre = b.usuarios[i].nombre;
    }
    int n = 0;
    for (int j = 0; j < b.cantidadPrestamos && n < moduloReportes::MAX; j++) {
        int pl = moduloPrestamos::buscarLibroPorCodigo(b.libros, b.cantidadLibros, b.prestamos[j].codigoLibro);
        int pu = indiceUsuario(b, b.prestamos[j].idUsuario);
        if (pl < 0 || pl >= nl || pu < 0 || pu >= nu) {
            continue;
        }
        moduloReportes::libros[pl].veces++;
        moduloReportes::prestamos[n].codigoLibro = pl;
        moduloReportes::prestamos[n].idUsuario = pu;
        moduloReportes::prestamos[n].fecha = fechaATexto(b.prestamos[j].fechaPrestamo);
        moduloReportes::prestamos[n].activo = b.prestamos[j].activo;
        n++;
    }
    moduloReportes::nPrestamos = n;
}

void opcionRegistrarLibro(Biblioteca& b) {
    string isbn, titulo, autor, signatura;
    int cantidad;
    leerLinea("ISBN: ", isbn);
    leerLinea("Titulo: ", titulo);
    leerLinea("Autor: ", autor);
    leerLinea("Signatura topografica: ", signatura);
    leerEntero("Cantidad de ejemplares: ", cantidad);
    agregarLibro(b, isbn, titulo, autor, signatura, cantidad);
}

void opcionBuscarLibro(Biblioteca& b) {
    string texto;
    leerLinea("Titulo a buscar: ", texto);
    b.catalogo.buscarPorTitulo(texto);
}

void opcionRegistrarUsuario(Biblioteca& b) {
    string nombre, identificador;
    leerLinea("Nombre: ", nombre);
    leerLinea("Identificador (U y 3 numeros, ejemplo U003): ", identificador);
    identificador = moduloPrestamos::aMayusculas(identificador);
    if (!moduloPrestamos::validarIdUsuario(identificador)) {
        cout << "Error: el identificador debe ser una U seguida de 3 numeros.\n";
        return;
    }
    moduloUsuarios::registrarUsuario(b.usuarios, (int)b.usuarios.size() + 1, nombre, identificador);
}

void opcionBuscarUsuario(Biblioteca& b) {
    string texto;
    leerLinea("Nombre a buscar: ", texto);
    int cantidad = moduloUsuarios::buscarUsuariosPorNombre(b.usuarios, texto);
    cout << "Coincidencias: " << cantidad << "\n";
}

void opcionHistorialUsuario(Biblioteca& b) {
    string identificador;
    leerLinea("Identificador del usuario: ", identificador);
    identificador = moduloPrestamos::aMayusculas(identificador);
    moduloUsuarios::Usuario* usuario = moduloUsuarios::buscarUsuarioPorIdentificador(b.usuarios, identificador);
    if (usuario == nullptr) {
        cout << "Usuario no encontrado.\n";
        return;
    }
    cout << "Historial de " << usuario->nombre << ":\n";
    moduloUsuarios::consultarHistorial(usuario->historialPrestamos);
}

void opcionPrestarLibro(Biblioteca& b) {
    string identificador, codigo;
    moduloPrestamos::Fecha fecha;
    leerLinea("Identificador del usuario: ", identificador);
    identificador = moduloPrestamos::aMayusculas(identificador);
    moduloUsuarios::Usuario* usuario = moduloUsuarios::buscarUsuarioPorIdentificador(b.usuarios, identificador);
    if (usuario == nullptr) {
        cout << "Usuario no encontrado.\n";
        return;
    }
    leerLinea("ISBN del libro: ", codigo);
    codigo = moduloPrestamos::aMayusculas(codigo);
    if (!leerFecha("Fecha del prestamo (DD/MM/AAAA): ", fecha)) {
        return;
    }
    bool creado = moduloPrestamos::crearPrestamo(b.prestamos, b.cantidadPrestamos, b.libros, b.cantidadLibros,
                                                 siguienteIdPrestamo(b), identificador, codigo, fecha);
    if (creado) {
        moduloUsuarios::agregarPrestamoAHistorial(*usuario, codigo);
    }
}

void opcionDevolverLibro(Biblioteca& b) {
    int id;
    moduloPrestamos::Fecha fecha;
    leerEntero("Numero de prestamo: ", id);
    if (!leerFecha("Fecha de devolucion (DD/MM/AAAA): ", fecha)) {
        return;
    }
    moduloPrestamos::registrarDevolucion(b.prestamos, b.cantidadPrestamos, b.libros, b.cantidadLibros, id, fecha);
    moduloPrestamos::guardarPrestamos(b.prestamos, b.cantidadPrestamos);
}

void menuReportes(Biblioteca& b) {
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n--- SUBMENU DE REPORTES ---\n";
        cout << "1. Cantidad total y disponible de libros\n";
        cout << "2. Libros prestados actualmente\n";
        cout << "3. Prestamos de un usuario\n";
        cout << "4. Prestamos de un libro\n";
        cout << "5. Prestamos entre dos fechas\n";
        cout << "6. Libros mas solicitados\n";
        cout << "7. Libro con mayor movimiento\n";
        cout << "8. Prestamos por mes\n";
        cout << "9. Resumen de usuarios\n";
        cout << "0. Volver\n";
        if (!leerEntero("Opcion: ", opcion)) {
            return;
        }
        sincronizarReportes(b);
        switch (opcion) {
            case 1:
                moduloReportes::reporteTotales();
                break;
            case 2:
                moduloPrestamos::mostrarPrestamosActivos(b.prestamos, b.cantidadPrestamos, b.libros, b.cantidadLibros);
                break;
            case 3: {
                string identificador;
                leerLinea("Identificador del usuario: ", identificador);
                int pos = indiceUsuario(b, moduloPrestamos::aMayusculas(identificador));
                if (pos == -1) {
                    cout << "Usuario no encontrado.\n";
                } else {
                    moduloReportes::filtrarRegistros("usuario", to_string(pos));
                }
                break;
            }
            case 4: {
                string codigo;
                leerLinea("ISBN del libro: ", codigo);
                int pos = moduloPrestamos::buscarLibroPorCodigo(b.libros, b.cantidadLibros, moduloPrestamos::aMayusculas(codigo));
                if (pos == -1) {
                    cout << "Libro no encontrado.\n";
                } else {
                    moduloReportes::filtrarRegistros("libro", to_string(pos));
                }
                break;
            }
            case 5: {
                moduloPrestamos::Fecha inicio, fin;
                if (leerFecha("Fecha de inicio (DD/MM/AAAA): ", inicio) && leerFecha("Fecha de fin (DD/MM/AAAA): ", fin)) {
                    moduloReportes::generarReportePrestamos(fechaATexto(inicio), fechaATexto(fin));
                }
                break;
            }
            case 6: {
                int n;
                leerEntero("Cuantos libros quiere ver: ", n);
                moduloReportes::obtenerLibrosMasSolicitados(n);
                break;
            }
            case 7:
                moduloPrestamos::mostrarLibroMasPrestado(b.prestamos, b.cantidadPrestamos, b.libros, b.cantidadLibros);
                break;
            case 8:
                moduloPrestamos::mostrarMatrizPorMes(b.prestamos, b.cantidadPrestamos);
                break;
            case 9:
                moduloUsuarios::mostrarMatrizResumen(moduloUsuarios::generarMatrizResumen(b.usuarios));
                break;
            case 0:
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    }
}

void cargarDatosIniciales(Biblioteca& b) {
    b.cantidadLibros = 0;
    b.cantidadPrestamos = 0;
    agregarLibro(b, "978-0001", "Introduccion a los Algoritmos", "Thomas Cormen", "004.1 COR", 2);
    agregarLibro(b, "978-0002", "C++ para principiantes", "Bjarne Stroustrup", "005.13 STR", 1);
    moduloUsuarios::registrarUsuario(b.usuarios, 1, "Ana Lopez", "U001");
    moduloUsuarios::registrarUsuario(b.usuarios, 2, "Carlos Perez", "U002");
    moduloPrestamos::iniciarPrestamos(b.prestamos, b.cantidadPrestamos, b.libros, b.cantidadLibros);
    for (int i = 0; i < b.cantidadPrestamos; i++) {
        moduloUsuarios::Usuario* usuario = moduloUsuarios::buscarUsuarioPorIdentificador(b.usuarios, b.prestamos[i].idUsuario);
        if (usuario != nullptr) {
            moduloUsuarios::agregarPrestamoAHistorial(*usuario, b.prestamos[i].codigoLibro);
        }
    }
}

void menuPrincipal() {
    Biblioteca b;
    cargarDatosIniciales(b);
    int opcion = -1;
    while (opcion != 0) {
        cout << "\n===== BIBLIOTECA =====\n";
        cout << "1. Registrar libro\n";
        cout << "2. Ver catalogo\n";
        cout << "3. Buscar libro por titulo\n";
        cout << "4. Registrar usuario\n";
        cout << "5. Ver usuarios\n";
        cout << "6. Buscar usuario por nombre\n";
        cout << "7. Historial de un usuario\n";
        cout << "8. Prestar libro\n";
        cout << "9. Devolver libro\n";
        cout << "10. Historial de prestamos\n";
        cout << "11. Reportes\n";
        cout << "0. Salir\n";
        if (!leerEntero("Opcion: ", opcion)) {
            break;
        }
        switch (opcion) {
            case 1: opcionRegistrarLibro(b); break;
            case 2: b.catalogo.listarCatalogo(); break;
            case 3: opcionBuscarLibro(b); break;
            case 4: opcionRegistrarUsuario(b); break;
            case 5: moduloUsuarios::listarUsuarios(b.usuarios); break;
            case 6: opcionBuscarUsuario(b); break;
            case 7: opcionHistorialUsuario(b); break;
            case 8: opcionPrestarLibro(b); break;
            case 9: opcionDevolverLibro(b); break;
            case 10: moduloPrestamos::consultarPrestamos(b.prestamos, b.cantidadPrestamos); break;
            case 11: menuReportes(b); break;
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }
    }
    moduloPrestamos::guardarPrestamos(b.prestamos, b.cantidadPrestamos);
    cout << "Hasta luego.\n";
}
