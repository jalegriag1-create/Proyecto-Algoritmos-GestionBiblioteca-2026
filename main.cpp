#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

namespace moduloCatalogo {
#include "catalogo.cpp"
}

namespace moduloPrestamos {
#include "prestamos.cpp"
}

namespace moduloUsuarios {
#include "usuarios.cpp"
}

namespace moduloReportes {
#include "reportes_persistencia.cpp"
}

int main() {
    std::cout << "=== Sistema de Gestion de Biblioteca ===\n\n";

    moduloCatalogo::Catalogo catalogo;
    catalogo.registrarLibro("978-0001", "Introduccion a los Algoritmos", "Thomas Cormen", "004.1 COR");
    catalogo.registrarLibro("978-0002", "C++ para principiantes", "Bjarne Stroustrup", "005.13 STR");
    catalogo.listarCatalogo();

    std::vector<moduloUsuarios::Usuario> usuarios;
    moduloUsuarios::registrarUsuario(usuarios, 1, "Ana Lopez", "U001");
    moduloUsuarios::registrarUsuario(usuarios, 2, "Carlos Perez", "U002");

    moduloUsuarios::Usuario* usuario = moduloUsuarios::buscarUsuarioPorIdentificador(usuarios, "U001");
    if (usuario != nullptr) {
        moduloUsuarios::agregarPrestamoAHistorial(*usuario, "978-0001");
    }

    moduloUsuarios::listarUsuarios(usuarios);

    moduloPrestamos::Libro libros[moduloPrestamos::MAX_LIBROS];
    libros[0].codigo = "978-0001";
    libros[0].titulo = "Introduccion a los Algoritmos";
    libros[0].cantidadTotal = 1;
    libros[0].cantidadDisponible = 1;

    moduloPrestamos::Prestamo prestamos[moduloPrestamos::MAX_PRESTAMOS];
    int cantidadPrestamos = 0;
    int cargados = moduloPrestamos::iniciarPrestamos(prestamos, cantidadPrestamos, libros, 1);
    std::cout << "Prestamos cargados del archivo: " << cargados << '\n';
    moduloPrestamos::Fecha fechaPrestamo{18, 9, 2026};
    moduloPrestamos::crearPrestamo(prestamos, cantidadPrestamos, libros, 1,
                                   1, "U001", "978-0001", fechaPrestamo);
    std::cout << "Prestamos activos: "
              << moduloPrestamos::contarPrestamosActivos(prestamos, cantidadPrestamos) << '\n';
    moduloPrestamos::mostrarPrestamosActivos(prestamos, cantidadPrestamos, libros, 1);
    moduloPrestamos::mostrarCantidadLibros(libros, 1);
    moduloPrestamos::Fecha fechaDevolucion{25, 9, 2026};
    moduloPrestamos::registrarDevolucion(prestamos, cantidadPrestamos, libros, 1,
                                         1, fechaDevolucion);
    std::cout << "Prestamos activos: "
              << moduloPrestamos::contarPrestamosActivos(prestamos, cantidadPrestamos) << '\n';
        moduloPrestamos::mostrarPrestamosPorUsuario(prestamos, cantidadPrestamos, "U001");
    moduloPrestamos::mostrarLibroMasPrestado(prestamos, cantidadPrestamos, libros, 1);
    moduloPrestamos::mostrarMatrizPorMes(prestamos, cantidadPrestamos);
        
    std::cout << "\n--- REPORTES Y PERSISTENCIA ---\n";
    moduloReportes::generarReportePrestamos("18/09/2026", "25/09/2026");
    moduloReportes::obtenerLibrosMasSolicitados(5);

    std::cout << "\n=== Ejecucion finalizada ===\n";
    return 0;
}
