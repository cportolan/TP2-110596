#include "adversario.h"
#include "juego.h"
#include "funcionamiento.h"
#include "ataque.h"
#include "pokemon.h"
#include "lista.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct adversario_pokedata {
	char nombre_pokemon[20];
	int pokemon_usado;
} adversario_pokedata_t;

struct adversario {
	lista_t* pokemones_originales;
	lista_t *ataques_adversario;
	lista_t *pokemones_adversario;
	adversario_pokedata_t pokedatas[3];
	struct ataque ataques_usados[9];
	int cant_ataques_usados;
};

const int POKEMONES_POR_JUGADOR = 3;
const int MAX_POKE_USADO = 3;
const int VALOR_INICIO = 0;
const int UNO_SOLO = 1;
const size_t PRIMERA_POS = 0;
const size_t SEGUNDA_POS = 1;
const size_t TERCER_POS = 2;

adversario_t *adversario_crear(lista_t *pokemon)
{
	if (!pokemon)
		return NULL;
	
	adversario_t *adversario = calloc(1, sizeof(adversario_t));

	if (!adversario)
		return NULL;

	adversario->pokemones_originales = pokemon;

	adversario->pokemones_adversario = lista_crear();

	if (!adversario->pokemones_adversario)
		return NULL;

	adversario->ataques_adversario = lista_crear();

	if (!adversario->ataques_adversario) {
		lista_destruir(adversario->pokemones_adversario);
		return NULL;
	}

	adversario->cant_ataques_usados = VALOR_INICIO;

	return adversario;
}

int comparador_nombres(void *e1, void *e2)
{
	return strcmp(pokemon_nombre((pokemon_t *)e1), (char *)e2);
}

void insertar_ataques_en_lista(const struct ataque *ataque, void *_lista)
{
	lista_t *lista = (lista_t *)_lista;
	lista_insertar(lista, (struct ataque *)ataque);
}

bool printear_todos_ataques(void *a, void* aux)
{
	if (!a)
		return false;
	
	struct ataque *ataque = a;
	printf("-> Nombre ataque adversario: %s\n", ataque->nombre);
	return true;
}

pokemon_t *pokemon_aleatorio(lista_t *pokemones)
{
	if (!pokemones)
		return NULL;

	bool pokemon_encontrado = false;
	pokemon_t *aux;

	while (!pokemon_encontrado) {
		size_t pos;

		if (lista_tamanio(pokemones) > UNO_SOLO) 
			pos = (size_t)rand() % (lista_tamanio(pokemones));
		else 
			pos = (size_t)VALOR_INICIO;

		aux = lista_elemento_en_posicion(pokemones, pos);

		if (aux != NULL) 
			pokemon_encontrado = true;
	}

	return aux;
}

int comparar_ataques(void *_ataque, void *_nombre)
{
	if (!_ataque || !_nombre)
		return false;

	struct ataque *ataque = (struct ataque *)_ataque;
	const char *nombre = (const char *)_nombre;
	return strcmp(ataque->nombre, nombre);
}

struct ataque* ataque_aleatorio(pokemon_t *pokemon, adversario_t *adversario)
{
	if (!pokemon || !adversario)
		return NULL;

	bool ataque_encontrado = false;
	struct ataque *aux;

	while(!ataque_encontrado) {
		size_t pos;
		pos = (size_t)rand() % (lista_tamanio(adversario->ataques_adversario));

		aux = lista_elemento_en_posicion(adversario->ataques_adversario, pos);

		if (aux != NULL && pokemon_buscar_ataque(pokemon, aux->nombre)) {
			ataque_encontrado = true;
		}
	}
	
	return aux;
}

void quitar_elementos_lista_adversario(lista_t *lista, int cantidad_elementos)
{
	for(int i = VALOR_INICIO; i < cantidad_elementos+1; i++) 
		lista_quitar(lista);
}

bool insertar_tres_pokemones_aleatorios(adversario_t *adversario)
{
	if (!adversario)
		return false;

	for(int i = VALOR_INICIO; i < POKEMONES_POR_JUGADOR; i++) {
		pokemon_t *p = pokemon_aleatorio(adversario->pokemones_originales);

		if (!p)
			return false;

		strcpy(adversario->pokedatas[i].nombre_pokemon, pokemon_nombre(p));
		adversario->pokedatas[i].pokemon_usado = VALOR_INICIO;
		if(!lista_insertar(adversario->pokemones_adversario, p))
			return false;
		con_cada_ataque(p, insertar_ataques_en_lista, adversario->ataques_adversario);
	}

	return true;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	if (!adversario || !nombre1 || !nombre2 || !nombre3)
		return false;

	bool no_son_repetidos = false;

	while(!no_son_repetidos) {
		insertar_tres_pokemones_aleatorios(adversario);

		*nombre1 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, PRIMERA_POS));
		*nombre2 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, SEGUNDA_POS));
		*nombre3 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, TERCER_POS));

		if (!nombre1 || !nombre2 || !nombre3)
			return false;

		if (pokemon_repetido(*nombre1, *nombre2, *nombre3)) {
			quitar_elementos_lista_adversario(adversario->pokemones_adversario, POKEMONES_POR_JUGADOR);
			quitar_elementos_lista_adversario(adversario->ataques_adversario, (int)lista_tamanio(adversario->ataques_adversario));
		} else {
			no_son_repetidos = true;
		}
	}

	printf("\n-> LOS POKEMONES SELECCIONADOS POR EL ADVERSARIO SON:\n");
	printf("-> Nombre Pokemon 1: %s\n", *nombre1);
	printf("-> Nombre Pokemon 2: %s\n", *nombre2);
	printf("-> Nombre Pokemon 3: %s\n", *nombre3);

	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	return true;
}

bool pokemon_con_ataques(pokemon_t *pokemon, adversario_t *adversario)
{
	for (int i = 0; i < 3; i++) {
		if (strcmp(pokemon_nombre(pokemon), adversario->pokedatas[i].nombre_pokemon) == 0) {
			if (adversario->pokedatas[i].pokemon_usado < MAX_POKE_USADO) {
				adversario->pokedatas[i].pokemon_usado++;
				return true;
			}
		}
	}

	return false;
}

bool ataque_usado(adversario_t *adversario, struct ataque* ataque)
{
	for (int i = 0; i < 9; i++) {
		if (strcmp(ataque->nombre, adversario->ataques_usados[i].nombre) == 0) {
			return true;
		}
	}

	return false;
}

jugada_t adversario_calcular_jugada(adversario_t *adversario)
{
	jugada_t jugada;

	pokemon_t *pokemon;
	struct ataque *ataque;

	bool poke_encontrado = false;
	while(!poke_encontrado) {
		pokemon = pokemon_aleatorio(adversario->pokemones_adversario);

		if (pokemon != NULL && pokemon_con_ataques(pokemon, adversario)) 
			poke_encontrado = true;
	}

	bool ataque_encontrado = false;
	while(!ataque_encontrado) {
		ataque = ataque_aleatorio(pokemon, adversario);

		if (ataque != NULL && !ataque_usado(adversario, ataque))
			ataque_encontrado = true;
	}
	
	adversario->ataques_usados[adversario->cant_ataques_usados] = *ataque;
	printf("Se guardo %s como un ataque usado\n", adversario->ataques_usados[adversario->cant_ataques_usados].nombre);
	adversario->cant_ataques_usados++;
	strcpy(jugada.pokemon, pokemon_nombre(pokemon));
	strcpy(jugada.ataque, ataque->nombre);
	return jugada;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };

	if (!adversario) {
		return j;
	}
	
	j = adversario_calcular_jugada(adversario);

	printf("\n-> JUGADA ADVERSARIO: El adversario ha jugado al pokemon: %s con el ataque: %s\n", j.pokemon, j.ataque);

	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	return;
}

void adversario_destruir(adversario_t *adversario)
{
	if (adversario) {
		lista_destruir(adversario->pokemones_adversario);
		lista_destruir(adversario->ataques_adversario);
		free(adversario);
	}
}
