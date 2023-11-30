#include "lista.h"
#include "pokemon.h"
#include "juego.h"
#include "ataque.h"
#include "adversario.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct jugador_pokedata {
	char nombre_pokemon[20];
	int usado;
} jugador_pokedata_t;

struct partida {
	lista_t *pokemones_jugador;
};

#define MAX_STRING 256
#define LETRA_ELECTRICO 'E'
#define LETRA_NORMAL 'N'
#define LETRA_PLANTA 'P'
#define LETRA_FUEGO 'F'
#define LETRA_AGUA 'A'
#define LETRA_ROCA 'R'
#define LETRA_INICIAR 'i'
#define LETRA_SALIR 'q'
const int CANT_POKEMONES_SELECCIONADOS = 3;

struct partida* crear_struct_partida(juego_t *juego)
{
	struct partida* p = malloc(sizeof(struct partida));

	if (!p)
		return NULL;

	p->pokemones_jugador = lista_crear();

	if (!p->pokemones_jugador)
		return NULL;

	return p;
}

void destruir_struct_partida(struct partida* partida)
{
	if (partida) {
		lista_destruir(partida->pokemones_jugador);
		free(partida);
	}
}

char* cambiar_tipo_letra(enum TIPO tipo)
{
	switch (tipo) {
	case ELECTRICO:
		return "Electrico";
	case FUEGO:
		return "Fuego";
	case AGUA:
		return "Agua";
	case PLANTA:
		return "Planta";
	case ROCA:
		return "Roca";
	case NORMAL:
		return "Normal";
	}
	return "Normal";
}

int comparar_pokemones(void *_pokemon, void *_nombre)
{
	if (!_pokemon || !_nombre)
		return false;

	pokemon_t *pokemon = (pokemon_t *)_pokemon;
	const char *nombre = (const char *)_nombre;
	return strcmp(pokemon_nombre(pokemon), nombre);
}

bool pokemon_repetido(const char *nombre1, const char *nombre2, const char *nombre3) 
{
	return (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3) == 0);
}

bool existe_pokemon(char nombre[MAX_STRING], lista_t *lista)
{
	pokemon_t *p = lista_buscar_elemento(lista, comparar_pokemones, nombre);
	if (!p) 
		return false;
	return true;
}

void mostrar_tipo(pokemon_t *p)
{
	char* tipo = cambiar_tipo_letra(pokemon_tipo(p));
	printf("Tipo de pokemon: %s\n", tipo);
}

void mostrar_ataque(const struct ataque *ataque, void *aux)
{
	if (!ataque) 
		return;
		
	printf("-> Ataque: %s\n", ataque->nombre);
}

bool mostrar_ataques_adversario(void *a, void *aux)
{
	if (!a)
		return false;

	struct ataque *ataque = a;
	printf("-> ATAQUE ADVERSARIO: %s\n", ataque->nombre);
	return true;
}

bool mostrar_poke(void *p, void* aux)
{
	if (!p)
		return false;
	
	pokemon_t *pokemon = p;
	printf("-> Nombre: %s\n", pokemon_nombre(pokemon));
	mostrar_tipo(pokemon);
	con_cada_ataque(pokemon, mostrar_ataque, NULL);
	printf("\n");
	return true;
}

char ingresar_comando() 
{		
	char letra;
	bool comando_correcto = false;

	while(!comando_correcto) {
		printf("Ingrese un comando valido (iniciar/salir del juego -> i / q): ");
		scanf("%c", &letra);

		while(getchar() != '\n');

		if (letra == LETRA_SALIR || letra == LETRA_INICIAR)
			comando_correcto = true;
	}

	return letra;
}

void mostrar_bienvenida(juego_t *juego)
{
		printf("\n\n          ╔══════════════════════════════════════════════════════════════╗\n"
	       "	  ║             BIENVENIDO AL GRAN JUEGO DE POKEMONES!!          ║\n"
	       "	  ╠══════════════════════════════════════════════════════════════╣\n"
	       "	  ║ Le doy la bienvenida a este divertido juego de cartas. Este  ║\n"
	       "	  ║ se dividira por turnos, y se debe ir jugando un pokemon con  ║\n"
	       "	  ║ un ataque a eleccion. Para iniciar el juego, le voy a pedir  ║\n"
		   "	  ║ a continuacion que ingrese un archivo con pokemones, luego   ║\n"
		   "	  ║ se mostrara una lista de todos los pokemones disponibles y   ║\n"
		   "	  ║ debe elegir tres pokemones. Una vez iniciado el juego, se    ║\n"
   		   "	  ║ tendra que ir eligiendo un pokemon y un ataque para luchar y ║\n"
		   "	  ║ la maquina jugara contra usted. Le deseo mucha suerte! :)    ║\n"
		   "	  ║                                                              ║\n"
		   "	  ║ Los comandos existentes:                                     ║\n"
		   "	  ║                                                              ║\n"
	       "	  ║                     - i: Iniciar el juego!                   ║\n"
		   "	  ║                     - q: Salir del juego.                    ║\n"
		   "	  ║                                                              ║\n"
		   "	  ║                                                              ║\n"
		   "	  ║                                     Atentamente, Cristian.   ║\n"
	       "	  ╚══════════════════════════════════════════════════════════════╝\n\n\n");
}

void listar_pokemones(juego_t *juego)
{
	printf("\n");
	printf("LOS POKEMONES DISPONIBLES SON: \n\n");
	lista_con_cada_elemento(juego_listar_pokemon(juego), mostrar_poke, NULL);
}

void quit()
{
	printf("\n\n          ╔══════════════════════════════════════════════════════════════╗\n"
	    "	  ║                        NOS VEMOS PRONTO                      ║\n"
	    "	  ╠══════════════════════════════════════════════════════════════╣\n"
	    "	  ║   Solicistaste el comando de salida. Espero volverte a ver.  ║\n"
	    "	  ║                           Saludos!                           ║\n"
	    "	  ╚══════════════════════════════════════════════════════════════╝\n\n\n");

	return;
}

void adversario_selecciona_pokemon(adversario_t *adversario)
{
	char *nombre1;
	char *nombre2;
	char *nombre3;
	
	adversario_seleccionar_pokemon(adversario, &nombre1, &nombre2, &nombre3);
}

bool llenar_partida(struct partida *partida, char *poke1, char *poke2, char *poke3, juego_t *juego)
{
	if (!poke1 || !poke2 || !poke3)
		return false;

	pokemon_t *p1 = lista_buscar_elemento(juego_listar_pokemon(juego), comparar_pokemones, poke1);
	pokemon_t *p2 = lista_buscar_elemento(juego_listar_pokemon(juego), comparar_pokemones, poke2);
	pokemon_t *p3 = lista_buscar_elemento(juego_listar_pokemon(juego), comparar_pokemones, poke3);

	if (!p1 || !p2 || !p3)
		return false;

	lista_insertar(partida->pokemones_jugador, p1);
	lista_insertar(partida->pokemones_jugador, p2);
	lista_insertar(partida->pokemones_jugador, p3);

	return true;
}

void seleccionar_pokemones(juego_t *juego, adversario_t *adversario, struct partida *partida)
{
	printf("-> SELECCIONE TRES POKEMONES PARA JUGAR ESCRIBIENDO SU NOMBRE <-\n\n");

	char nombre1[MAX_STRING];
	char nombre2[MAX_STRING];
	char nombre3[MAX_STRING];

	bool poke1_seleccionado = false;
	bool poke2_seleccionado = false;
	bool poke3_seleccionado = false;

	printf("-> Ingresa el nombre del primer Pokémon: ");
	while(!poke1_seleccionado) {
		
    	scanf("%s", nombre1);
		if (existe_pokemon(nombre1, juego_listar_pokemon(juego))) 
			poke1_seleccionado = true;
		else 
			printf("\n-> ERROR: El pokemon ingresado no existe. Vuelva a escribir el pokemon: ");
	}

	printf("\n-> Ingresa el nombre del segundo Pokémon: ");
	while(!poke2_seleccionado) {
    	
    	scanf("%s", nombre2);
		if (existe_pokemon(nombre2, juego_listar_pokemon(juego)) && !pokemon_repetido(nombre1, nombre2, " ")) 
			poke2_seleccionado = true;
		else 
			printf("\n-> ERROR: El pokemon ingresado no existe o es repetido. Vuelva a escribir el pokemon: ");
	}

	printf("\n-> Ingresa el nombre del tercer Pokémon: ");
	while(!poke3_seleccionado) {
    	scanf("%s", nombre3);
		if (existe_pokemon(nombre3, juego_listar_pokemon(juego)) && !pokemon_repetido(nombre1, nombre2, nombre3)) 
			poke3_seleccionado = true;
		else 
			printf("\n-> ERROR: El pokemon ingresado no existe o es repetido. Vuelva a escribir el pokemon: ");
	}

	juego_seleccionar_pokemon(juego, JUGADOR1, nombre1, nombre2, nombre3);
	llenar_partida(partida, nombre1, nombre2, nombre3, juego);
	adversario_selecciona_pokemon(adversario);
}

void menu_pedir_archivo(juego_t *juego) {
	JUEGO_ESTADO cargado = ERROR_GENERAL;

	while (cargado != TODO_OK) {
		char *a = malloc(MAX_STRING);

		if (!a) {
			printf("-> ERROR: No se pudo reservar la memoria para el archivo correctamente\n");
			return;
		}

		printf("\n\n          ╔══════════════════════════════════════════════════════════════╗\n"
	       "	  ║                  CARGAR ARCHIVO DE POKEMONES                 ║\n"
	       "	  ╠══════════════════════════════════════════════════════════════╣\n"
	       "	  ║ Por favor, ingrese el nombre del archivo en el que se hallan ║\n"
	       "	  ║ todos los pokemones, según muestra el siguiente ejemplo:     ║\n"
	       "	  ║                       ./ruta/nombre.txt                      ║\n"
	       "	  ╚══════════════════════════════════════════════════════════════╝\n\n\n"
	       "Archivo: ");
		scanf(" %255s", a);

		cargado = juego_cargar_pokemon(juego, a);

		if (cargado == POKEMON_INSUFICIENTES)
			printf("\n-> ERROR: En el archivo ingresado, no hay la cantidad minima de pokemones para poder jugar (4). Por favor, ingresa un nuevo archivo.\n");
		else if (cargado == ERROR_GENERAL)
			printf("\n-> ERROR: Ocurrio un error cargando el archivo, puede que sea un archivo invalido, asegurese de cumplir el formato pedido. Por favor, ingresa un nuevo archivo.\n");
		else if (cargado == TODO_OK)
			printf("\n\n 	  ╔══════════════════════════════════════════════════════════════╗\n"
	       	"	  ║  ARCHIVO CARGADO CORRECTAMENTE! LA LISTA TIENE %i POKEMONES!  ║\n"
	       "	  ╚══════════════════════════════════════════════════════════════╝\n\n", (int)lista_tamanio(juego_listar_pokemon(juego)));	
	}

	return;
}

jugada_t elegir_jugada()
{
	jugada_t jugada;

	printf("\n\n-> JUGADA: DEBE ESCOGER UN NOMBRE DE POKEMON Y LUEGO UN NOMBRE DE ATAQUE <-\n\n");

	printf("- Ingresa el nombre del pokemon: ");
	scanf("%s", jugada.pokemon);
	printf("- Ingresa el nombre del ataque: ");
	scanf("%s", jugada.ataque);

	return jugada;
}

bool jugada_valida(juego_t *juego, jugada_t jugada, struct partida *partida)
{
	pokemon_t *p = lista_buscar_elemento(partida->pokemones_jugador, comparar_pokemones, jugada.pokemon);
	if ((p != NULL) && (pokemon_buscar_ataque(p, jugada.ataque) != NULL)) {
		return true;
	} else {
		printf("\n-> ERROR: Hubo un error al recibir tu jugada. Por favor, escriba nuevamente pokemon y ataque tal como se lo pide y verifique que sea correcto\n");
		return false;
	}
}

void elegir_ganador(juego_t *juego) 
{
	int puntaje_j1 = juego_obtener_puntaje(juego, JUGADOR1);
	int puntaje_j2 = juego_obtener_puntaje(juego, JUGADOR2);

	if (puntaje_j1 > puntaje_j2)
		printf("\n       -> FELICIDADES, GANASTE LA BATALLA DE POKEMONES. <-\n");
	else if (puntaje_j1 < puntaje_j2)
		printf("\n       -> LO SENTIMOS, HAS PERDIDO LA BATALLA DE POKEMONES <-\n");
	else if (puntaje_j1 == puntaje_j2)
		printf("\n       -> ESTA BATALLA HA RESULTADO EN UN EMPATE, VUELVA A JUGAR PARA GANAR!\n");
}

bool inicializar_juego(juego_t *juego, adversario_t *adversario, struct partida *partida) {

	if (!juego || !adversario)
		return false;

	menu_pedir_archivo(juego);
	listar_pokemones(juego);
	seleccionar_pokemones(juego, adversario, partida);

	return true;
}

