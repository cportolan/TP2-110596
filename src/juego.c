#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct jugador {
	lista_t *pokemones_elegidos;
	float puntaje;
} jugador_t;

struct juego {
	lista_t *pokemones;
	jugador_t *jugador1;
	jugador_t *jugador2;
	bool terminado;
};

const int POKEMONES_MINIMOS_PARA_JUGAR = 4;

void pasar_pokemones(pokemon_t *actual, void *_lista)
{
	lista_t *l = (lista_t *)_lista;
	lista_insertar(l, actual);
}

jugador_t *jugador_crear()
{
	jugador_t *jugador = calloc(1, sizeof(jugador_t));

	if (!jugador)
		return NULL;
	
	jugador->pokemones_elegidos = NULL;
	jugador->puntaje = 0;
}

juego_t *juego_crear()
{
	juego_t *juego = calloc(1, sizeof(juego_t));

	if (!juego)
		return NULL;

	juego->pokemones = NULL;
	juego->jugador1 = jugador_crear();
	juego->jugador2 = jugador_crear();
	juego->terminado = false;

	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if (!juego || !archivo)
		return ERROR_GENERAL;

	juego->pokemones = lista_crear();

	if (!juego->pokemones)
		return ERROR_GENERAL;

	informacion_pokemon_t *info = pokemon_cargar_archivo(archivo);
	con_cada_pokemon(info, pasar_pokemones, juego->pokemones);
	
	if (lista_tamanio(juego->pokemones) < POKEMONES_MINIMOS_PARA_JUGAR) {
		for(size_t i = 0; i <= lista_tamanio(juego_listar_pokemon(juego)) + 1; i++)
			lista_quitar(juego->pokemones);

		return POKEMON_INSUFICIENTES;
	}

	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if (!juego)
		return NULL;

	return juego->pokemones;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if (!juego)
		return ERROR_GENERAL;

	
	
	return TODO_OK;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if (!juego)
		return 0;
	
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	return juego->terminado;
}

void juego_destruir(juego_t *juego)
{
	if (juego) {
		lista_destruir(juego->pokemones);
		free(juego);
	}
}
