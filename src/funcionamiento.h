#ifndef FUNCIONAMIENTO_H_
#define FUNCIONAMIENTO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#include "juego.h"
#include "adversario.h"

typedef struct partida partida_t;

/*
	* Creo el struct partida.
*/
partida_t *crear_struct_partida(juego_t *juego);

/*
	* Destruyo el menu partida.
*/
void destruir_struct_partida(partida_t *partida);

/*
	* Llena el struct de partida. (pokemones del jugador)
*/
bool llenar_partida(partida_t *partida, char *poke1, char *poke2, char *poke3, juego_t *juego);

/*
	* Muestra un mensaje de bienvenida al usuario.
*/
void mostrar_bienvenida();

/*
	* Pide al usuario el archivo para poder comenzar a jugar.
*/
void menu_pedir_archivo(juego_t *juego);

/*
	* Al inicio del juego, le pide al usuario que ingrese para iniciar o salir del juego.
*/
char ingresar_comando();

/*
	* Muestra todos los pokemones en una lista.
*/
bool mostrar_poke(void *elemento1, void *elemento2);

/*
	* Muestra los ataques del adversario.
*/
bool mostrar_ataques_adversario(void *a, void *aux);

/*
	* Printea una lista de todos los pokemones.
*/
void listar_pokemones(juego_t *juego);

/*
	* Sale del juego mostrando un mensaje.
*/
void quit();

/*
	* Los jugadores deben elegir tres pokemones.
*/
void jugador_seleccionar_pokemon(juego_t *juego, adversario_t *adversario);

/*
	* El adversario elige los pokemones.
*/
void adversario_selecciona_pokemon(adversario_t *adversario);

/*
	* Devuelve true si hay pokemones repetidos.
*/
bool pokemon_repetido(const char* n1, const char *n2, const char *n3);

/*
	* Los jugadores realizan la jugada eligiendo pokemon y ataque.
*/
void jugador_realizar_jugada(juego_t *juego);

/*
	* Inicializa el juego, dejandolo listo para poder jugar. Pide informacion y realiza las validaciones.
*/
bool *inicializar_juego(juego_t *juego, adversario_t *adversario, partida_t *partida);

/*
	* El jugador elige una jugada (pokemon y ataque).
*/
jugada_t elegir_jugada(partida_t *partida);

/*
	* Valida la jugada.
*/
bool jugada_valida(juego_t *juego, jugada_t jugada, partida_t *partida);

/*
	* Determina el ganador de la partida.
*/
void elegir_ganador(juego_t *juego);

#endif // __MENU_H__