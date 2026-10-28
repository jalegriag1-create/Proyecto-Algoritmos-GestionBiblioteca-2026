// Modulo Usuarios - Sistema de Gestion de Biblioteca
// Struct, prototipos y funciones (registro, validacion, historial, busquedas y procesamiento)

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Estructura que representa a un usuario/lector de la biblioteca
struct Usuario {
    int id;
    string nombre;
    string identificador;              // carnet, DPI o codigo unico
    vector<string> historialPrestamos; // codigos de libros que ha pedido
};

// --- Prototipos de funciones (contratos de entrada/salida) ---

// Convierte un texto a minusculas (normalizacion para comparar sin distinguir mayusculas).
// Entrada: texto original.
// Salida: copia del texto en minusculas; el original no cambia.
string aMinusculas(string texto);

// Registra un nuevo usuario en la coleccion.
// Entrada: vector de usuarios (por referencia), id, nombre, identificador.
// Salida: no retorna valor; agrega el usuario si el identificador no esta vacio ni repetido.
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

// Busca usuarios cuyo nombre contenga un texto (sin distinguir mayusculas) y los muestra.
// Entrada: vector de usuarios, texto a buscar.
// Salida: cantidad de usuarios encontrados (0 si el texto esta vacio o solo tiene espacios).
int buscarUsuariosPorNombre(const vector<Usuario>& usuarios, const string& texto);

// Cuenta cuantos usuarios tienen al menos un prestamo en su historial.
// Entrada: vector de usuarios.
// Salida: cantidad de usuarios con historial no vacio.
int contarUsuariosConPrestamos(const vector<Usuario>& usuarios);

// Ordena el vector de usuarios alfabeticamente por nombre, sin distinguir mayusculas (burbuja).
// Entrada: vector de usuarios (se modifica).
// Salida: no retorna valor.
void ordenarUsuariosPorNombre(vector<Usuario>& usuarios);

// Arma una matriz resumen: cada fila = [nombre, identificador, total de prestamos].
// Entrada: vector de usuarios.
// Salida: matriz de strings (vector de vectores).
vector<vector<string>> generarMatrizResumen(const vector<Usuario>& usuarios);

// Muestra la matriz resumen en consola con dos for anidados.
// Entrada: matriz de strings.
// Salida: no retorna valor.
void mostrarMatrizResumen(const vector<vector<string>>& matriz);

// --- Implementacion ---

// Recorre el texto y convierte cada letra a minuscula
string aMinusculas(string texto) {
    for (size_t i = 0; i < texto.size(); i++) {
        texto[i] = tolower((unsigned char)texto[i]);
    }
    return texto;
}

void registrarUsuario(vector<Usuario>& usuarios, int id, const string& nombre, const string& identificador) {
    // Caso invalido: identificador vacio
    if (identificador == "") {
        cout << "Error: el identificador no puede estar vacio.\n";
        return;
    }
    if (validarIdentificador(usuarios, identificador)) {
        cout << "Error: ya existe un usuario con el identificador " << identificador << ".\n";
        return;
    }
    for (const Usuario& u : usuarios) {
        if (u.id == id) {
            cout << "Error: ya existe un usuario con el id " << id << ".\n";
            return;
        }
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

// Recorre el vector y muestra los usuarios cuyo nombre contiene el texto, sin distinguir mayusculas.
// Se compara en minusculas, pero se imprime el nombre original.
int buscarUsuariosPorNombre(const vector<Usuario>& usuarios, const string& texto) {
    // Caso invalido: texto vacio o solo espacios
    if (texto.find_first_not_of(' ') == string::npos) {
        cout << "Error: la busqueda no puede estar vacia.\n";
        return 0;
    }

    string textoMin = aMinusculas(texto);
    int encontrados = 0;
    for (const Usuario& u : usuarios) {
        if (aMinusculas(u.nombre).find(textoMin) != string::npos) {
            cout << "  - " << u.nombre << " (" << u.identificador << ")\n";
            encontrados++;
        }
    }
    return encontrados;
}

// Recorre el vector y cuenta los usuarios cuyo historial no esta vacio
int contarUsuariosConPrestamos(const vector<Usuario>& usuarios) {
    int contador = 0;
    for (const Usuario& u : usuarios) {
        if (!u.historialPrestamos.empty()) {
            contador++;
        }
    }
    return contador;
}

// Ordenamiento burbuja: compara cada nombre (en minusculas) con el siguiente y los intercambia si estan al reves
void ordenarUsuariosPorNombre(vector<Usuario>& usuarios) {
    int n = usuarios.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (aMinusculas(usuarios[j].nombre) > aMinusculas(usuarios[j + 1].nombre)) {
                Usuario temporal = usuarios[j];
                usuarios[j] = usuarios[j + 1];
                usuarios[j + 1] = temporal;
            }
        }
    }
}

// Cada usuario se convierte en una fila de la matriz
vector<vector<string>> generarMatrizResumen(const vector<Usuario>& usuarios) {
    vector<vector<string>> matriz;
    for (const Usuario& u : usuarios) {
        vector<string> fila;
        fila.push_back(u.nombre);
        fila.push_back(u.identificador);
        fila.push_back(to_string(u.historialPrestamos.size()));
        matriz.push_back(fila);
    }
    return matriz;
}

// Recorre la matriz fila por fila y columna por columna
void mostrarMatrizResumen(const vector<vector<string>>& matriz) {
    cout << "Nombre | Identificador | Prestamos\n";
    for (int i = 0; i < (int)matriz.size(); i++) {
        for (int j = 0; j < (int)matriz[i].size(); j++) {
            cout << matriz[i][j] << " | ";
        }
        cout << "\n";
    }
}

// --- Prueba del modulo (se puede quitar cuando se integre con el main del equipo) ---
#ifdef USUARIOS_TEST
int main() {
    vector<Usuario> usuarios;

    registrarUsuario(usuarios, 1, "Ana Lopez", "U001");
    registrarUsuario(usuarios, 2, "Carlos Perez", "U002");
    registrarUsuario(usuarios, 1, "Duplicado", "U001");      // debe rechazar (identificador repetido)
    registrarUsuario(usuarios, 2, "Otro Distinto", "U777");  // debe rechazar (id repetido)
    registrarUsuario(usuarios, 3, "Sin Identificador", "");  // caso invalido: debe rechazar
    registrarUsuario(usuarios, 4, "Ana Martinez", "U003");
    registrarUsuario(usuarios, 5, "zoe Ramirez", "U004");    // nombre en minuscula (para probar el orden)
    registrarUsuario(usuarios, 6, "beto Diaz", "U005");      // nombre en minuscula (para probar el orden)

    Usuario* u = buscarUsuarioPorIdentificador(usuarios, "U001");
    if (u != nullptr) {
        agregarPrestamoAHistorial(*u, "LIB-001");
        agregarPrestamoAHistorial(*u, "LIB-015");
    }

    cout << "\n--- Listado de usuarios ---\n";
    listarUsuarios(usuarios);

    cout << "\n¿Existe U099? " << (validarIdentificador(usuarios, "U099") ? "Si" : "No") << "\n";

    cout << "\n--- Busqueda por nombre 'Ana' ---\n";
    int encontrados = buscarUsuariosPorNombre(usuarios, "Ana");
    cout << "Encontrados: " << encontrados << "\n";

    cout << "\n--- Busqueda por nombre 'ana' (minusculas) ---\n";
    encontrados = buscarUsuariosPorNombre(usuarios, "ana");
    cout << "Encontrados: " << encontrados << "\n";

    cout << "\n--- Busqueda por nombre 'ANA' (mayusculas) ---\n";
    encontrados = buscarUsuariosPorNombre(usuarios, "ANA");
    cout << "Encontrados: " << encontrados << "\n";

    cout << "\n--- Busqueda con texto vacio ---\n";
    encontrados = buscarUsuariosPorNombre(usuarios, "");
    cout << "Encontrados: " << encontrados << "\n";

    cout << "\nUsuarios con prestamos: " << contarUsuariosConPrestamos(usuarios) << "\n";

    ordenarUsuariosPorNombre(usuarios);
    cout << "\n--- Matriz resumen (ordenada por nombre, sin distinguir mayusculas) ---\n";
    mostrarMatrizResumen(generarMatrizResumen(usuarios));

    return 0;
}
#endif

