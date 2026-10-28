#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Libro {
    string isbn;
    string titulo;
    string autor;
    string signaturaTopografica;
    bool disponible;

    Libro(string _isbn, string _titulo, string _autor, string _signatura) {
        isbn = _isbn;
        titulo = _titulo;
        autor = _autor;
        signaturaTopografica = _signatura;
        disponible = true;
    }
};

class Catalogo {
private:
    vector<Libro> catalogo;

    bool buscarPorIsbn(const string& isbn) const {
        for (const auto& libro : catalogo) {
            if (libro.isbn == isbn) {
                return true;
            }
        }
        return false;
    }

public:
    void registrarLibro(string isbn, string titulo, string autor, string signatura) {
        if (isbn.empty() || isbn.find_first_not_of(' ') == string::npos) {
            cout << "❌ Error: El ISBN no puede estar vacio.\n";
            return;
        }

        if (buscarPorIsbn(isbn)) {
            cout << "❌ Error: El libro con ISBN '" << isbn << "' ya se encuentra registrado.\n";
            return;
        }

        Libro nuevoLibro(isbn, titulo, autor, signatura);
        catalogo.push_back(nuevoLibro);
        cout << "✔️ Libro '" << titulo << "' catalogado con exito.\n";
    }

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

    void actualizarDisponibilidad(const string& isbn, bool disponible) {
        for (auto& libro : catalogo) {
            if (libro.isbn == isbn) {
                libro.disponible = disponible;
                return;
            }
        }
    }

    bool editarLibro(const std::string& isbn, const std::string& titulo, const std::string& autor, const std::string& signatura) {
        if (titulo.find_first_not_of(' ') == string::npos) {
            cout << "Error: el titulo no puede estar vacio.\n";
            return false;
        }
        for (auto& libro : catalogo) {
            if (libro.isbn == isbn) {
                libro.titulo = titulo;
                libro.autor = autor;
                libro.signaturaTopografica = signatura;
                return true;
            }
        }
        return false;
    }

    void consultarPorIsbn(const std::string& isbn) {
        for (const auto& libro : catalogo) {
            if (libro.isbn == isbn) {
                cout << "\n--- DATOS DEL LIBRO ---\n";
                cout << "ISBN: " << libro.isbn << "\n";
                cout << "Titulo: " << libro.titulo << "\n";
                cout << "Autor: " << libro.autor << "\n";
                cout << "Signatura: " << libro.signaturaTopografica << "\n";
                cout << "Estado: " << (libro.disponible ? "Disponible" : "Prestado") << "\n";
                return;
            }
        }
        cout << "No se encontro ningun libro con ISBN '" << isbn << "'.\n";
    }
};

int main() {
    Catalogo catalogo;

    cout << "=== MÓDULO DE CATÁLOGO ===\n\n";

    catalogo.registrarLibro("9780307474728", "Cien anos de soledad", "Gabriel Garcia Marquez", "863.44 G216c");
    catalogo.registrarLibro("9780451524935", "1984", "George Orwell", "823.912 O79n");

    cout << "\n--- Probando Validaciones de ISBN ---\n";
    catalogo.registrarLibro("", "Libro Invalido 1", "Autor Anonimo", "000.00 X000x");
    catalogo.registrarLibro("9780307474728", "Cien anos de soledad (Copia)", "Gabriel Garcia Marquez", "863.44 G216c");

    string terminoBusqueda = "soledad";
    catalogo.buscarPorTitulo(terminoBusqueda);

    catalogo.listarCatalogo();

    cout << "\n--- Probando editarLibro y consultarPorIsbn ---\n";
    cout << (catalogo.editarLibro("9780307474728", "Cien anos de soledad (Edicion 2)", "Gabriel Garcia Marquez", "863.44 G216c") ? "true" : "false") << endl;
    cout << (catalogo.editarLibro("0000000000", "No existe", "Nadie", "000.00") ? "true" : "false") << endl;
    cout << (catalogo.editarLibro("9780451524935", "   ", "George Orwell", "823.912 O79n") ? "true" : "false") << endl;
    catalogo.consultarPorIsbn("9780307474728");
    catalogo.consultarPorIsbn("0000000000");

    return 0;
}