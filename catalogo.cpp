#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// PARÁMETROS: Estructura de Datos (Campos)
// ==========================================

class Libro {
public:
    // Parámetros de descripción e identificación
    string isbn;                 // Identificador único del libro
    string titulo;                // Título de la obra
    string autor;                 // Autor de la obra
    string signaturaTopografica;  // Ubicación en estante (Ej: Dewey/LC)
    bool disponible;              // Estado del ejemplar (true = en estantería)

    // Parámetros del constructor: valores necesarios para crear un Libro
    Libro(string _isbn, string _titulo, string _autor, string _signatura) {
        isbn = _isbn;
        titulo = _titulo;
        autor = _autor;
        signaturaTopografica = _signatura;
        disponible = true;
    }
};

// ==========================================
// MÓDULO: Catálogo
// ==========================================
// Funciones:
//   1. registrarLibro(isbn, titulo, autor, signatura)
//      -> Agrega un nuevo libro al catálogo validando que el ISBN no esté vacío ni repetido.
//   2. buscarPorTitulo(termino)
//      -> Busca libros cuyo título contenga el término dado.
//   3. listarCatalogo()
//      -> Muestra todos los libros registrados.

class Catalogo {
private:
    vector<Libro> catalogo; // Parámetro: colección interna de libros

    // Función auxiliar para verificar si un ISBN ya existe
    bool buscarPorIsbn(const string& isbn) const {
        for (const auto& libro : catalogo) {
            if (libro.isbn == isbn) {
                return true;
            }
        }
        return false;
    }

public:
    // Función: registrarLibro con validaciones
    void registrarLibro(string isbn, string titulo, string autor, string signatura) {
        // Validación 1: ISBN vacío
        if (isbn.empty() || isbn.find_first_not_of(' ') == string::npos) {
            cout << "❌ Error: El ISBN no puede estar vacio.\n";
            return;
        }

        // Validación 2: ISBN repetido
        if (buscarPorIsbn(isbn)) {
            cout << "❌ Error: El libro con ISBN '" << isbn << "' ya se encuentra registrado.\n";
            return;
        }

        Libro nuevoLibro(isbn, titulo, autor, signatura);
        catalogo.push_back(nuevoLibro);
        cout << "✔️ Libro '" << titulo << "' catalogado con exito.\n";
    }

    // Función: buscarPorTitulo
    void buscarPorTitulo(string termino) {
        cout << "\n--- RESULTADOS DE BÚSQUEDA PARA: \"" << termino << "\" ---\n";
        bool encontrado = false;
        for (const auto& libro : catalogo) {
            if (libro.titulo.find(termino) != string::npos) {
                cout << "[" << libro.signaturaTopografica << "] "
                     << libro.titulo << " - " << libro.autor
                     << " (ISBN: " << libro.isbn << ") | "
                     << (libro.disponible ? "Disponible" : "Prestado") << "\n";
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron coincidencia.\n";
    }

    // Función: listarCatalogo
    void listarCatalogo() {
        cout << "\n--- CATÁLOGO COMPLETO ---\n";
        if (catalogo.empty()) {
            cout << "El catálogo está vacío.\n";
            return;
        }
        for (const auto& libro : catalogo) {
            cout << "[" << libro.signaturaTopografica << "] "
                 << libro.titulo << " - " << libro.autor
                 << " (ISBN: " << libro.isbn << ") | "
                 << (libro.disponible ? "Disponible" : "Prestado") << "\n";
        }
    }
};

// ==========================================
// FUNCIÓN PRINCIPAL (Demostración)
// ==========================================

int main() {
    Catalogo catalogo;

    cout << "=== MÓDULO DE CATÁLOGO ===\n\n";

    // 1. Registros correctos
    catalogo.registrarLibro("9780307474728", "Cien anos de soledad", "Gabriel Garcia Marquez", "863.44 G216c");
    catalogo.registrarLibro("9780451524935", "1984", "George Orwell", "823.912 O79n");

    // 2. Demostración de las nuevas validaciones de seguridad
    cout << "\n--- Probando Validaciones de ISBN ---\n";
    catalogo.registrarLibro("", "Libro Invalido 1", "Autor Anonimo", "000.00 X000x"); // Intento con ISBN vacío
    catalogo.registrarLibro("9780307474728", "Cien anos de soledad (Copia)", "Gabriel Garcia Marquez", "863.44 G216c"); // Intento con ISBN duplicado

    // 3. Llamar buscarPorTitulo desde el flujo principal
    string terminoBusqueda = "soledad";
    catalogo.buscarPorTitulo(terminoBusqueda);

    // 4. Mostrar estado final del catálogo
    catalogo.listarCatalogo();

    return 0;
}

