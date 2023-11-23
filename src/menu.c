#include "menu.h"
#include "lista.h"
#include "pokemon.h"
#include "juego.h"
#include "ataque.h"

struct info {
	bool juego_finalizado;
};

struct comando {
    char *instruccion;
    char *descripcion;
    void (*funcion)(juego_t *);
};

struct menu {
    lista_t* comandos;
};

#define MAX_STRING 256

#define LETRA_ELECTRICO 'E'
#define LETRA_NORMAL 'N'
#define LETRA_PLANTA 'P'
#define LETRA_FUEGO 'F'
#define LETRA_AGUA 'A'
#define LETRA_ROCA 'R'

const int CANT_POKEMONES_SELECCIONADOS = 3;

char cambiar_tipo_letra(enum TIPO tipo)
{
	switch (tipo) {
	case ELECTRICO:
		return LETRA_ELECTRICO;
	case FUEGO:
		return LETRA_FUEGO;
	case AGUA:
		return LETRA_AGUA;
	case PLANTA:
		return LETRA_PLANTA;
	case ROCA:
		return LETRA_ROCA;
	case NORMAL:
		return LETRA_NORMAL;
	}
	return NORMAL;
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

bool existe_pokemon(char nombre[MAX_STRING], juego_t *juego)
{
	pokemon_t *p = lista_buscar_elemento(juego_listar_pokemon(juego), comparar_pokemones, nombre);
	if (!p) 
		return false;
	return true;
}

void mostrar_tipo(pokemon_t *p)
{
	char tipo = cambiar_tipo_letra(pokemon_tipo(p));
	printf("Tipo de pokemon: %c\n", tipo);
}

void mostrar_ataque(const struct ataque *ataque, void *aux)
{
	if (!ataque) 
		return;
		
	printf("-> Nombre Ataque: %s\n", ataque->nombre);
}

bool mostrar_poke(void *p, void* aux)
{
	if (!p)
		return false;
	
	pokemon_t *pokemon = p;
	printf("Nombre: %s\n", pokemon_nombre(pokemon));
	mostrar_tipo(pokemon);
	con_cada_ataque(pokemon, mostrar_ataque, NULL);
	printf("\n");
	return true;
}

void listar_pokemones(juego_t *juego)
{
	printf("\n\n");
	printf("LOS POKEMONES DISPONIBLES SON: \n\n");
	lista_con_cada_elemento(juego_listar_pokemon(juego), mostrar_poke, NULL);
	printf("\n\n");
}

void quit(juego_t *juego)
{
	printf("\n\n          ╔══════════════════════════════════════════════════════════════╗\n"
	    "	  ║                        NOS VEMOS PRONTO                      ║\n"
	    "	  ╠══════════════════════════════════════════════════════════════╣\n"
	    "	  ║   Solicistaste el comando de salida. Espero volverte a ver.  ║\n"
	    "	  ║                           Saludos!                           ║\n"
	    "	  ╚══════════════════════════════════════════════════════════════╝\n\n\n");

	return;
}

void jugador_seleccionar_pokemon(juego_t *juego)
{
	printf("\n\n-> SELECCIONE TRES POKEMONES PARA JUGAR ESCRIBIENDO SU NOMBRE <-\n\n");

	char nombre1[MAX_STRING];
	char nombre2[MAX_STRING];
	char nombre3[MAX_STRING];

	bool poke1_seleccionado = false;
	bool poke2_seleccionado = false;
	bool poke3_seleccionado = false;

	printf("Ingresa el nombre del primer Pokémon (existente): ");
	while(!poke1_seleccionado) {
		
    	scanf("%s", nombre1);
		if (existe_pokemon(nombre1, juego)) 
			poke1_seleccionado = true;
		else 
			printf("\nEl pokemon ingresado no existe. Vuelva a escribir el pokemon: ");
	}

	printf("\nIngresa el nombre del segundo Pokémon: ");
	while(!poke2_seleccionado) {
    	
    	scanf("%s", nombre2);
		if (existe_pokemon(nombre2, juego) && !pokemon_repetido(nombre1, nombre2, " ")) 
			poke2_seleccionado = true;
		else 
			printf("\nEl pokemon ingresado no existe o es repetido. Vuelva a escribir el pokemon: ");
	}

	printf("\nIngresa el nombre del tercer Pokémon: ");
	while(!poke3_seleccionado) {
    	scanf("%s", nombre3);
		if (existe_pokemon(nombre3, juego) && !pokemon_repetido(nombre1, nombre2, nombre3)) 
			poke3_seleccionado = true;
		else 
			printf("\nEl pokemon ingresado no existe o es repetido. Vuelva a escribir el pokemon: ");
	}

	juego_seleccionar_pokemon(juego, JUGADOR1, nombre1, nombre2, nombre3);
}

void jugador_realizar_jugada(juego_t *juego)
{	
	printf("\n\n-> ELEGI UN POKEMON Y UN ATAQUE PARA REALIZAR TU JUGADA.\n\n");
	jugada_t jugada_jugador1;
	jugada_t jugada_jugador2;

	char nombre_pokemon_j1[MAX_STRING];
	char nombre_ataque_j1[MAX_STRING];

	printf("Ingresa el nombre del Pokemon a elegir: ");
	scanf("%s", nombre_pokemon_j1);

	printf("Ingresa el nombre del ataque: ");
	scanf("%s", nombre_ataque_j1);

	strcpy(jugada_jugador1.pokemon, nombre_pokemon_j1);
	strcpy(jugada_jugador1.ataque, nombre_ataque_j1);

	juego_jugar_turno(juego, jugada_jugador1, jugada_jugador2);
}

void mostrar_ayuda(juego_t *juego)
{
	printf("\n\n          ╔══════════════════════════════════════════════════════════════╗\n"
	    "	  ║                       LISTADO DE COMANDOS                    ║\n"
	    "	  ╠══════════════════════════════════════════════════════════════╣\n"
	    "	  ║ Solicistaste el comando de ayuda. Permitime mostrarme todos  ║\n"
	    "	  ║ los comandos existentes:                                     ║\n"
	    "	  ║                     - h: Ayuda                               ║\n"
		"	  ║                     - l: Listar pokemones del archivo        ║\n"
		"	  ║                     - 1: Seleccionar pokes para jugador 1.   ║\n"
		"	  ║                     - 2: Seleccionar pokes para jugador 2.   ║\n"
		"	  ║                     - q: Quit (salir del juego).             ║\n"
	    "	  ╚══════════════════════════════════════════════════════════════╝\n\n\n");
}

void menu_pedir_archivo(juego_t *juego) {
	char *a = malloc(MAX_STRING);

	if (!a)
		return;

	printf("\n\n          ╔══════════════════════════════════════════════════════════════╗\n"
	       "	  ║                  CARGAR ARCHIVO DE POKEMONES                 ║\n"
	       "	  ╠══════════════════════════════════════════════════════════════╣\n"
	       "	  ║ Por favor, ingrese el nombre del archivo en el que se hallan ║\n"
	       "	  ║ todos los pokemones, según muestra el siguiente ejemplo:     ║\n"
	       "	  ║                       ./ruta/nombre.txt                      ║\n"
	       "	  ╚══════════════════════════════════════════════════════════════╝\n\n\n"
	       "Archivo: ");
	scanf(" %255s", a);

	if (!a) {
		free(a);
		return;
	} 

	juego_cargar_pokemon(juego, a);
}

menu_t *menu_crear() {
	menu_t *menu = calloc(1, sizeof(menu_t));

	if (!menu)
		return NULL;
	
	menu->comandos = lista_crear();

	if (!menu->comandos) {
		free(menu);
		return NULL;
	}

	return menu;
}

comando_t *crear_comando(char *instruccion, char *descripcion, void (*funcion)(juego_t *))
{
	if (!instruccion || !descripcion || !funcion)
		return NULL;

	comando_t *com = malloc(sizeof(comando_t));

	if (!com)
		return NULL;

	com->instruccion = instruccion;
	com->descripcion = descripcion;
	com->funcion = funcion;

	return com;
}

int buscar_comando(void *_comando, void *_instruccion)
{
	if (!_comando || !_instruccion)
		return false;

	comando_t *comando = (comando_t *)_comando;
	char *instruccion = (char *)_instruccion;
	return strcmp(comando->instruccion, instruccion);
}

menu_t *menu_agregar_comando(menu_t *menu, char* instruccion, char *descripcion, void (*funcion)(juego_t *)) 
{
    if (!menu || !instruccion || !descripcion || !funcion)
        return NULL;

    comando_t *comando = crear_comando(instruccion, descripcion, funcion);

    if (!comando)
        return NULL;

    lista_insertar(menu->comandos, comando);

    return menu;
}

bool menu_contiene_comando(menu_t *menu, char *instruccion)
{
	if (!menu || !instruccion || !menu_obtener_comando(menu, instruccion))
		return false;

	return true;
}

comando_t *menu_obtener_comando(menu_t *menu, char *instruccion)
{
	if (!menu || !instruccion)
		return NULL;

	return lista_buscar_elemento(menu->comandos, buscar_comando, (void *)instruccion);
}

bool menu_ejecutar_comando(menu_t *menu, char *instruccion, juego_t *contexto)
{
	if (!menu || !instruccion)
		return false;

	comando_t *comando = lista_buscar_elemento(menu->comandos, buscar_comando, (void *)instruccion);

	if (comando) {
		comando->funcion(contexto);
		return true;
	}
		
	return false;
}

bool menu_vacio(menu_t *menu)
{
	if (!menu || lista_vacia(menu->comandos) == true)
		return true;

	return false;
}

size_t menu_cantidad_comandos(menu_t *menu)
{
	if (!menu)
		return 0;

	return lista_tamanio(menu->comandos);
}

menu_t *menu_eliminar_comando(menu_t *menu, char *instruccion)
{
	if (!menu || !instruccion)
		return NULL;

	size_t pos = 0;

	lista_iterador_t *lista_iterador = lista_iterador_crear(menu->comandos);

	bool encontrado = false;

	while (lista_iterador_tiene_siguiente(lista_iterador) && !encontrado) {
		pos++;
		comando_t *comando = lista_iterador_elemento_actual(lista_iterador);

		if (strcmp(comando->instruccion, instruccion) == 0) {
			pos--;
			encontrado = true;
		}
	}

	if (!encontrado)
		return NULL;

	comando_t *eliminado = lista_quitar_de_posicion(menu->comandos, pos);
	free(eliminado);
	lista_iterador_destruir(lista_iterador);
	return menu;
}

void agregar_todos_los_comandos(menu_t *menu)
{
	menu_agregar_comando(menu, "c", "Cargar un archivo", menu_pedir_archivo);
	menu_agregar_comando(menu, "l", "Listar pokemones", listar_pokemones);
	menu_agregar_comando(menu, "h", "Ayuda", mostrar_ayuda);
	menu_agregar_comando(menu, "s", "Seleccionar pokemones", jugador_seleccionar_pokemon);
	menu_agregar_comando(menu, "j", "Realizar jugada", jugador_realizar_jugada);
	menu_agregar_comando(menu, "q", "Salir del juego", quit);
}

void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;

	lista_destruir_todo(menu->comandos, free);
	free(menu);
}