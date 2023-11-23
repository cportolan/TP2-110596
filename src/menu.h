#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#include "juego.h"

typedef struct comando comando_t;

typedef struct menu menu_t;

typedef struct info info_t;

/*
	* Pide al usuario el archivo para poder comenzar a jugar.
*/
void menu_pedir_archivo();

/*
	* Muestra los comandos de ayuda con una interfaz grafica amigable.
*/
void mostrar_ayuda();

/*
	* Printea una lista de todos los pokemones.
*/
void listar_pokemones();

/*
	* Sale del juego.
*/
void quit();

/*
	* Los jugadores deben elegir tres pokemones.
*/
void jugador_seleccionar_pokemon();


/*
	* Los jugadores realizan la jugada eligiendo pokemon y ataque.
*/
void jugador_realizar_jugada();

/*
    * Crea el menu reservando la memoria necesaria e inicializando los campos del mismo. 
    * Devuelve NULL en caso de no poder, o el menu en caso contrario.
*/
menu_t *menu_crear();

/*
Agrega el comando a nuestro menu y lo hace accesible para poder usarlo.
Devuelve NULL en caso de no poder o el menu en caso contrario.
*/
menu_t *menu_agregar_comando(menu_t *menu, char *instruccion, char *descripcion,
			     void (*funcion)(juego_t *));

/* 
	* Devuelve true si el menú contiene el comando dado, o 
	* false en caso contrario.
 */
bool menu_contiene_comando(menu_t *menu, char *instruccion);

/* 
	* Ejecuta el comando dado del menú.
	* Devuelve NULL en caso de error o de no poder eliminarlo, 
	* o el menú en caso contrario.
*/
bool menu_ejecutar_comando(menu_t *menu, char *instruccion, juego_t *contexto);

/* 
	* Busca el comando dado del menú.
	* Devuelve NULL en caso de error o de no encontrarlo,
	* o el comando en caso contrario.
*/
comando_t *menu_obtener_comando(menu_t *menu, char *instruccion);

/* 
	* Devuelve la cantidad de comandos del menú.
	* Devuelve 0 si el menú no existe.
*/
size_t menu_cantidad_comandos(menu_t *menu);

/* 
	* Elimina el comando dado del menú y libera la memoria 
	* reservada para el mismo.
	* Devuelve NULL en caso de error o de no poder eliminarlo, 
	* o el menú en caso contrario.
*/
menu_t *menu_eliminar_comando(menu_t *menu, char *instruccion);

/*
	* Agrega todos los comandos para los pokemones.
*/
void agregar_todos_los_comandos(menu_t *menu);

/* 
	* Libera la memoria reservada para el menu
*/
void menu_destruir(menu_t *menu);


#endif // __MENU_H__