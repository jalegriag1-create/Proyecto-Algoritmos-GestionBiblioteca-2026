# Sistema de Gestión de Biblioteca

Proyecto Final de Algoritmos, UMG. Grupo 1, Sección A.

Repositorio: https://github.com/jalegriag1-create/Proyecto-Algoritmos-GestionBiblioteca-2026

## Descripción

Sistema de consola en C++ para administrar el catálogo de libros, los usuarios y los préstamos y devoluciones de una biblioteca, con persistencia de datos en archivos de texto.

## Módulos

- Catálogo: registrar, editar, consultar y buscar libros por título o ISBN.
- Usuarios: registrar lectores, buscar por nombre, ordenar y consultar historial.
- Préstamos: crear préstamos, validar disponibilidad y registrar devoluciones.
- Reportes y persistencia: estadísticas, guardado y carga de archivos.

## Compilación

g++ -std=c++17 main.cpp -o biblioteca

## Ejecución

Windows: .\biblioteca.exe
Los datos se guardan en libros.txt, usuarios.txt y prestamos.txt, en la misma carpeta del programa. Si no existen, el programa carga datos de ejemplo.

## Datos de prueba

- datos_ejemplo/: archivos listos para copiar junto al ejecutable.
- pruebas/: archivos de entrada para repetir las pruebas.
  Ejemplo: Get-Content pruebas\entrada_basicas.txt | .\biblioteca.exe

## Estado

Versión final de entrega.

## Integrantes

- José Andrés Alegría Gonzalez
- Andy Leonel Bethancourt Contreras
- Eddy Manuel Castillo Álvarez
- Javier Eduardo Melgar Blanco
- Elvin Antonio Peralta Román