#include "adversario.h"
#include "juego.h"
#include "funcionamiento.h"
#include "ataque.h"
#include "pokemon.h"
#include "lista.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct adversario {
	lista_t* pokemones_originales;
	lista_t *ataques_disponibles;
	lista_t *pokemones_adversario;
	lista_t *pokemones_jugador;
	int adversario_puntaje;
};

const int POKEMONES_POR_JUGADOR = 3;

adversario_t *adversario_crear(lista_t *pokemon)
{
	if (!pokemon)
		return NULL;
	
	adversario_t *adversario = calloc(1, sizeof(adversario_t));

	if (!adversario)
		return NULL;

	adversario->pokemones_originales = pokemon;
	adversario->pokemones_adversario = lista_crear();
	adversario->ataques_disponibles = lista_crear();
	adversario->pokemones_jugador = lista_crear();
	adversario->adversario_puntaje = 0;

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

void mostrar_ataques(const struct ataque *ataque, void *aux)
{
	if (!ataque) 
		return;
		
	printf("-> Nombre Ataque: %s\n", ataque->nombre);
}

bool printear_todos_ataques(void *p, void* aux)
{
	if (!p)
		return false;
	
	pokemon_t *pokemon = p;
	con_cada_ataque(pokemon, mostrar_ataques, NULL);
	printf("\n");
	return true;
}

void contar_ataques(const struct ataque *ataque, void *_contador)
{
	if (_contador) {
		int *aux = _contador;
		(*aux)++;
	}
}

pokemon_t *pokemon_aleatorio(lista_t *pokemones)
{
	bool pokemon_encontrado = false;
	pokemon_t *aux;
	int contador = 0;

	while (!pokemon_encontrado) {
		size_t pos;

		if (lista_tamanio(pokemones) > 1) 
			pos = (size_t)rand() % (lista_tamanio(pokemones));
		else 
			pos = 0;

		aux = lista_elemento_en_posicion(pokemones, pos);

		if (aux != NULL && con_cada_ataque(aux, contar_ataques, &contador) > 0) 
			pokemon_encontrado = true;
	}

	return aux;
}

struct ataque* ataque_aleatorio(pokemon_t *pokemon, lista_t *ataques)
{
	bool ataque_encontrado = false;
	struct ataque *aux;

	while(!ataque_encontrado) {
		size_t pos;
		
		if (lista_tamanio(ataques) > 1) 
			pos = (size_t)rand() % (lista_tamanio(ataques));
		else 
			pos = 0;
		
		aux = lista_elemento_en_posicion(ataques, pos);

		if (pokemon_buscar_ataque(pokemon, aux->nombre) != NULL && aux != NULL) {
			lista_quitar_de_posicion(ataques, pos);
			ataque_encontrado = true;
		} else {
			ataque_encontrado = false;
		}
	}	
	
	return aux;
}

void quitar_elementos_lista_adversario(lista_t *lista, int cantidad_elementos)
{
	for(int i = 0; i < cantidad_elementos+1; i++) 
		lista_quitar(lista);
}

bool insertar_tres_pokemones_aleatorios(adversario_t *adversario)
{
	if (!adversario)
		return false;

	for(int i = 0; i < POKEMONES_POR_JUGADOR; i++) {
		pokemon_t *p = pokemon_aleatorio(adversario->pokemones_originales);

		if (!p)
			return false;

		lista_insertar(adversario->pokemones_adversario, p);
		con_cada_ataque(p, insertar_ataques_en_lista, adversario->ataques_disponibles);
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

		*nombre1 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 0));
		*nombre2 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 1));
		*nombre3 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 2));

		if (!nombre1 || !nombre2 || !nombre3)
			return false;

		if (pokemon_repetido(*nombre1, *nombre2, *nombre3)) {
			quitar_elementos_lista_adversario(adversario->pokemones_adversario, POKEMONES_POR_JUGADOR);
			quitar_elementos_lista_adversario(adversario->ataques_disponibles, (int)lista_tamanio(adversario->ataques_disponibles));
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
	// if (!adversario || !nombre1 || !nombre2 || !nombre3)
	// 	return false;

	// if (!lista_insertar(adversario->pokemones_jugador, lista_buscar_elemento(adversario->pokemones_originales, comparador_nombres, nombre1)))
	// 	return false;
	
	// if (!lista_insertar(adversario->pokemones_jugador, lista_buscar_elemento(adversario->pokemones_originales, comparador_nombres, nombre2)))
	// 	return false;
	
	// if (!lista_insertar(adversario->pokemones_adversario, lista_buscar_elemento(adversario->pokemones_originales, comparador_nombres, nombre3)))
	// 	return false;

	return true;
}

jugada_t adversario_calcular_jugada(lista_t *pokemones, lista_t *ataques)
{
	jugada_t jugada;

	pokemon_t *pokemon;
	struct ataque *ataque;

	bool poke_encontrado = false;
	while(!poke_encontrado) {
		pokemon = pokemon_aleatorio(pokemones);

		if (pokemon != NULL)
			poke_encontrado = true;
	}

	bool ataque_encontrado = false;
	while(!ataque_encontrado) {
		ataque = ataque_aleatorio(pokemon, ataques);

		if (ataque != NULL)
			ataque_encontrado = true;
	}
	
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
	
	j = adversario_calcular_jugada(adversario->pokemones_adversario, adversario->ataques_disponibles);

	printf("-> JUGADA ADVERSARIO: El adversario ha jugado al pokemon: %s con el ataque: %s\n", j.pokemon, j.ataque);
	printf("El adversario tiene %i ataques\n", (int)lista_tamanio(adversario->ataques_disponibles));
	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	return;
}

void adversario_destruir(adversario_t *adversario)
{
	if (adversario) {

		if (adversario->pokemones_originales)
			lista_destruir(adversario->pokemones_originales);
		else if (adversario->pokemones_jugador)
			lista_destruir(adversario->pokemones_jugador);
		else if (adversario->pokemones_adversario)
			lista_destruir(adversario->pokemones_adversario);
		else if (adversario->ataques_disponibles)
			lista_destruir(adversario->ataques_disponibles);
		
		free(adversario);
	}
}
