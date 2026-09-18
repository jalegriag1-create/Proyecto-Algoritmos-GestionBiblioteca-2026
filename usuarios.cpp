// Modulo Usuarios - Sistema de Gestion de Biblioteca
// Struct, prototipos y funciones iniciales (registro, validacion, historial)

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Estructura que representa a un usuario/lector de la biblioteca
struct Usuario {
    int id;
    string nombre;
    string identificador;              // carnet, DPI o codigo unico
    vector<string> historialPrestamos; // codigos de libros que ha pedido
};

// --- Prototipos de funciones (contratos de entrada/salida) ---

// Registra un nuevo usuario en la coleccion.
// Entrada: vector de usuarios (por referencia), id, nombre, identificador.
// Salida: no retorna valor; agrega el usuario al vector si el identificador no existe.
void registrarUsuario(vector<Usuario>& usuarios, int id, const string& nombre, const string& identificador);

// Valida si un identificador ya existe entre los usuarios registrados.
// Entrada: vector de usuarios, identificador a validar.
// Salida: true si el identificador existe, false si no.
bool validarIdentificador(const vector<Usuario>& usuarios, const string& identificador);

// Busca un usuario por su identificador.
// Entrada: vector de usuarios, identificador buscado.
// Salida: puntero al Usuario encontrado, o nullptr si no existe.
Usuario* buscarUsuarioPorIdentificador(vector<Usuario>& usuarios, const string& identificador);

// Consulta el historial basico de prestamos de un usuario de forma recursiva.
// Entrada: historial (vector de codigos), indice desde donde se imprime (default 0).
// Salida: no retorna valor; imprime cada elemento del historial en consola.
void consultarHistorial(const vector<string>& historial, int indice = 0);

// Agrega un codigo de libro al historial de prestamos de un usuario.
// Entrada: referencia al usuario, codigo del libro.
// Salida: no retorna valor; modifica el historial del usuario.
void agregarPrestamoAHistorial(Usuario& usuario, const string& codigoLibro);

// Lista todos los usuarios registrados con su informacion basica.
// Entrada: vector de usuarios.
// Salida: no retorna valor; imprime el listado en consola.
void listarUsuarios(const vector<Usuario>& usuarios);

// --- Implementacion ---

void registrarUsuario(vector<Usuario>& usuarios, int id, const string& nombre, const string& identificador) {
    if (validarIdentificador(usuarios, identificador)) {
        cout << "Error: ya existe un usuario con el identificador " << identificador << ".\n";
        return;
    }
    Usuario nuevo;
    nuevo.id = id;
    nuevo.nombre = nombre;
    nuevo.identificador = identificador;
    usuarios.push_back(nuevo);
    cout << "Usuario registrado correctamente: " << nombre << " (" << identificador << ")\n";
}

bool validarIdentificador(const vector<Usuario>& usuarios, const string& identificador) {
    for (const Usuario& u : usuarios) {
        if (u.identificador == identificador) {
            return true;
        }
    }
    return false;
}

Usuario* buscarUsuarioPorIdentificador(vector<Usuario>& usuarios, const string& identificador) {
    for (Usuario& u : usuarios) {
        if (u.identificador == identificador) {
            return &u;
        }
    }
    return nullptr;
}

// Recorrido recursivo del historial de prestamos (cumple el requisito de recursividad)
void consultarHistorial(const vector<string>& historial, int indice) {
    if (indice >= (int)historial.size()) {
        if (indice == 0) {
            cout << "  (sin prestamos registrados)\n";
        }
        return; // caso base
    }
    cout << "  - " << historial[indice] << "\n";
    consultarHistorial(historial, indice + 1); // llamada recursiva
}

void agregarPrestamoAHistorial(Usuario& usuario, const string& codigoLibro) {
    usuario.historialPrestamos.push_back(codigoLibro);
}

void listarUsuarios(const vector<Usuario>& usuarios) {
    if (usuarios.empty()) {
        cout << "No hay usuarios registrados.\n";
        return;
    }
    for (const Usuario& u : usuarios) {
        cout << "ID: " << u.id << " | Nombre: " << u.nombre
             << " | Identificador: " << u.identificador << "\n";
        cout << "Historial:\n";
        consultarHistorial(u.historialPrestamos);
    }
}

// --- Prueba del modulo (se puede quitar cuando se integre con el main del equipo) ---
#ifdef USUARIOS_TEST
int main() {
    vector<Usuario> usuarios;

    registrarUsuario(usuarios, 1, "Ana Lopez", "U001");
    registrarUsuario(usuarios, 2, "Carlos Perez", "U002");
    registrarUsuario(usuarios, 1, "Duplicado", "U001"); // debe rechazar

    Usuario* u = buscarUsuarioPorIdentificador(usuarios, "U001");
    if (u != nullptr) {
        agregarPrestamoAHistorial(*u, "LIB-001");
        agregarPrestamoAHistorial(*u, "LIB-015");
    }

    cout << "\n--- Listado de usuarios ---\n";
    listarUsuarios(usuarios);

    cout << "\n¿Existe U099? " << (validarIdentificador(usuarios, "U099") ? "Si" : "No") << "\n";

    return 0;
}
#endif
