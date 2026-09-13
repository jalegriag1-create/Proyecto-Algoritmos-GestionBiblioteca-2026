#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ==========================================
// PARÁMETROS: Estructuras de Datos (Campos)
// ==========================================

class Libro {
public:
    // Parámetros de descripción e identificación
    string isbn;
    string titulo;
    string autor;
    string signaturaTopografica; // Ubicación en estante (Ej: Dewie/LC)
    
    // Parámetros de gestión de ejemplares
    bool disponible;
    string prestadoA; // Guarda el ID del usuario si está prestado

    Libro(string _isbn, string _titulo, string _autor, string _signatura) {
        isbn = _isbn;
        titulo = _titulo;
        autor = _autor;
        signaturaTopografica = _signatura;
        disponible = true;
        prestadoA = "";
    }
};

class Usuario {
public:
    string idUsuario;
    string nombre;

    Usuario(string _id, string _nombre) {
        idUsuario = _id;
        nombre = _nombre;
    }
};

// ==========================================
// MÓDULOS: Lógica del Sistema de Biblioteca
// ==========================================

class SistemaBiblioteca {
private:
    vector<Libro> catalogo;
    vector<Usuario> usuarios;

public:
    // 1. MÓDULO DE CATALOGACIÓN: Agregar libros al inventario
    void registrarLibro(string isbn, string titulo, string autor, string signatura) {
        Libro nuevoLibro(isbn, titulo, autor, signatura);
        catalogo.push_back(nuevoLibro);
        cout << "✔️ Libro '" << titulo << "' catalogado con exito.\n";
    }

    // Registro auxiliar de usuarios
    void registrarUsuario(string id, string nombre) {
        usuarios.push_back(Usuario(id, nombre));
        cout << "✔️ Usuario '" << nombre << "' registrado.\n";
    }

    // 2. MÓDULO DE BÚSQUEDA (OPAC): Buscar por distintos parámetros
    void buscarPorTitulo(string termino) {
        cout << "\n--- RESULTADOS DE BÚSQUEDA PARA: \"" << termino << "\" ---\n";
        bool encontrado = false;
        for (const auto& libro : catalogo) {
            // Conversión rápida a minúsculas simulada para búsqueda flexible
            if (libro.titulo.find(termino) != string::npos) {
                cout << "[" << libro.signaturaTopografica << "] " 
                     << libro.titulo << " - " << libro.autor 
                     << " (ISBN: " << libro.isbn << ") | "
                     << (libro.disponible ? "Disponible" : "Prestado") << "\n";
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron coincidencias.\n";
    }
     // 3. MÓDULO DE CIRCULACIÓN: Control de Préstamos y Devoluciones
    void prestarLibro(string isbn, string idUsuario) {
        // Verificar si el usuario existe
        bool usuarioExiste = false;
        for (const auto& u : usuarios) {
            if (u.idUsuario == idUsuario) { usuarioExiste = true; break; }
        }

        if (!usuarioExiste) {
            cout << "❌ Error: El usuario con ID " << idUsuario << " no esta registrado.\n";
            return;
        }

        // Buscar el libro y cambiar su estado
        for (auto& libro : catalogo) {
            if (libro.isbn == isbn) {
                if (libro.disponible) {
                    libro.disponible = false;
                    libro.prestadoA = idUsuario;
                    cout << "📋 Prestamo exitoso: '" << libro.titulo << "' prestado al usuario " << idUsuario << ".\n";
                    return;
                } else {
                    cout << "❌ Error: El libro ya se encuentra prestado.\n";
                    return;
                }
            }
        }
        cout << "❌ Error: El ISBN " << isbn << " no existe en el catalogo.\n";
    }

    void devolverLibro(string isbn) {
        for (auto& libro : catalogo) {
            if (libro.isbn == isbn) {
                if (!libro.disponible) {
                    libro.disponible = true;
                    libro.prestadoA = "";
                    cout << "🔄 Devolucion exitosa: '" << libro.titulo << "' vuelve a estar disponible.\n";
                    return;
                } else {
                    cout << "⚠️ El libro ya estaba disponible en estanteria.\n";
                    return;
                }
            }
        }
        cout << "❌ Error: El ISBN " << isbn << " no pertenece a la biblioteca.\n";
    }
};

// ==========================================
// FUNCIÓN PRINCIPAL (Demostración)
// ==========================================

int main() {
    SistemaBiblioteca biblioteca;

    cout << "=== SIMULACIÓN DE SISTEMA DE BIBLIOTECA ===\n\n";

    // Probar Módulo de Catalogación
    biblioteca.registrarLibro("9780307474728", "Cien anos de soledad", "Gabriel Garcia Marquez", "863.44 G216c");
    biblioteca.registrarLibro("9780451524935", "1984", "George Orwell", "823.912 O79n");
    biblioteca.registrarUsuario("U001", "Carlos Gomez");

    // Probar Módulo OPAC (Búsqueda)
    biblioteca.buscarPorTitulo("soledad");

    // Probar Módulo de Circulación (Préstamo)
    biblioteca.prestarLibro("9780307474728", "U001");

    // Volver a buscar para ver el cambio de estado (Parámetro dinámico)
    biblioteca.buscarPorTitulo("Cien anos");

    // Probar Módulo de Circulación (Devolución)
    biblioteca.devolverLibro("9780307474728");

    return 0;
}