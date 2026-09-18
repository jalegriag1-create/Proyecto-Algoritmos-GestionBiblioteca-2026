#include <iostream>
#include <string>
#include <vector>

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

    std::vector<moduloPrestamos::Libro> libros;
    libros.push_back({"978-0001", "Introduccion a los Algoritmos", 1, 1});

    std::vector<moduloPrestamos::Prestamo> prestamos;
    moduloPrestamos::Fecha fechaPrestamo{18, 9, 2026};
    moduloPrestamos::crearPrestamo(prestamos, libros, 1, "U001", "978-0001", fechaPrestamo);
    moduloPrestamos::consultarPrestamos(prestamos);

    moduloPrestamos::Fecha fechaDevolucion{25, 9, 2026};
    moduloPrestamos::registrarDevolucion(prestamos, libros, 1, fechaDevolucion);
    moduloPrestamos::consultarPrestamos(prestamos);

    std::cout << "\n--- REPORTES Y PERSISTENCIA ---\n";
    moduloReportes::generarReportePrestamos("18/09/2026", "25/09/2026");
    moduloReportes::obtenerLibrosMasSolicitados(5);

    std::cout << "\n=== Ejecucion finalizada ===\n";
    return 0;
}
