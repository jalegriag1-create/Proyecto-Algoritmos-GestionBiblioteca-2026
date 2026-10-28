#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
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
#include "reportes.cpp"
}

namespace moduloPersistencia {
#include "persistencia.cpp"
}

#include "menu.cpp"

int main() {
    std::cout << "=== Sistema de Gestion de Biblioteca ===\n";
    menuPrincipal();
    return 0;
}
