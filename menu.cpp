#include "menu.h"
#include "reportes.h"
#include <iostream>

using namespace std;

// Funciones temporales del equipo para evitar errores
void registrarLibro() { cout << "\n[Modulo Libro] Registrar libro\n"; }
void modificarLibro() { cout << "\n[Modulo Libro] Modificar libro\n"; }
void mostrarLibros() { cout << "\n[Modulo Libro] Ver libros\n"; }
void buscarPorCodigo() { cout << "\n[Modulo Libro] Buscar por codigo\n"; }
void buscarPorTitulo() { cout << "\n[Modulo Libro] Buscar por titulo\n"; }
void registrarUsuario() { cout << "\n[Modulo Usuario] Registrar usuario\n"; }
void mostrarUsuarios() { cout << "\n[Modulo Usuario] Ver usuarios\n"; }
void prestarLibro() { cout << "\n[Modulo Prestamo] Prestar libro\n"; }
void devolverLibro() { cout << "\n[Modulo Prestamo] Devolver libro\n"; }

// Submenú de Reportes (Interacción de consola)
void menuReportes() {
    int opcion;
    do {
        cout << "\n--- SUBMENU DE REPORTES ---\n";
        cout << "1. Cantidad total y disponible de libros\n";
        cout << "2. Libros prestados actualmente\n";
        cout << "3. Prestamos de un usuario\n";
        cout << "4. Prestamos de un libro\n";
        cout << "5. Prestamos entre dos fechas\n";
        cout << "6. Libros mas solicitados\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                reporteTotales();
                break;
            case 2:
                filtrarRegistros("estado", "activo");
                break;
            case 3: {
                string id;
                cout << "ID del usuario: ";
                cin >> id;
                filtrarRegistros("usuario", id);
                break;
            }
            case 4: {
                string codigo;
                cout << "Codigo del libro: ";
                cin >> codigo;
                filtrarRegistros("libro", codigo);
                break;
            }
            case 5: {
                string inicio, fin;
                cout << "Fecha de inicio (AAAA-MM-DD): ";
                cin >> inicio;
                cout << "Fecha de fin (AAAA-MM-DD): ";
                cin >> fin;
                generarReportePrestamos(inicio, fin);
                break;
            }
            case 6: {
                int n;
                cout << "Cuantos libros quiere ver: ";
                cin >> n;
                obtenerLibrosMasSolicitados(n);
                break;
            }
            case 0:
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}

// Menú Principal
void menuPrincipal() {
    int opcion;
    do {
        cout << "\n===== BIBLIOTECA =====\n";
        cout << "1. Registrar libro\n";
        cout << "2. Modificar libro\n";
        cout << "3. Ver libros\n";
        cout << "4. Buscar libro por codigo\n";
        cout << "5. Buscar libro por titulo\n";
        cout << "6. Registrar usuario\n";
        cout << "7. Ver usuarios\n";
        cout << "8. Prestar libro\n";
        cout << "9. Devolver libro\n";
        cout << "10. Reportes\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: registrarLibro(); break;
            case 2: modificarLibro(); break;
            case 3: mostrarLibros(); break;
            case 4: buscarPorCodigo(); break;
            case 5: buscarPorTitulo(); break;
            case 6: registrarUsuario(); break;
            case 7: mostrarUsuarios(); break;
            case 8: prestarLibro(); break;
            case 9: devolverLibro(); break;
            case 10: menuReportes(); break;
            case 0: cout << "Hasta luego.\n"; break;
            default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);
}