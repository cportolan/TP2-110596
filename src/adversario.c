#include "lista.h"
#include "juego.h"
#include "adversario.h"
#include "pokemon.h"
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
	adversario->pokemones_jugador = NULL;
	adversario->adversario_puntaje = 0;

	return adversario;
}

pokemon_t *pokemon_aleatorio(lista_t *pokemones, size_t i)
{
	size_t pos = (size_t)rand() % (lista_tamanio(pokemones) - i);
	pokemon_t *aux = lista_elemento_en_posicion(pokemones, pos);
	lista_quitar_de_posicion(pokemones, pos);
	lista_insertar(pokemones, aux);
	return aux;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1,
				    char **nombre2, char **nombre3)
{
	if (!adversario || !nombre1 || !nombre2 || !nombre3)
		return false;

	for(size_t i = 0; i < 2; i++)
		lista_insertar(adversario->pokemones_adversario, pokemon_aleatorio(adversario->pokemones_originales, i));

	*nombre1 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 0));
	*nombre2 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 1));
	*nombre3 = (char *)pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 2));

	printf("NOMBRE: %s\n", *nombre1);
	printf("NOMBRE: %s\n", *nombre2);
	printf("NOMBRE: %s\n", *nombre3);

	// strcpy(*nombre1, pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 0)));
	// strcpy(*nombre2, pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 1)));
	// strcpy(*nombre3, pokemon_nombre(lista_elemento_en_posicion(adversario->pokemones_adversario, 2)));

	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1,
				     char *nombre2, char *nombre3)
{
	return false;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	jugada_t j = { .ataque = "", .pokemon = "" };
	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
}

void adversario_destruir(adversario_t *adversario)
{
	if (adversario) {
		lista_destruir(adversario->pokemones_adversario);
		free(adversario);
	}
}
