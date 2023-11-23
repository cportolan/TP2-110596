#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "adversario.h"

typedef struct jugador {
	lista_t *pokemones_elegidos;
	lista_t *ataques_disponibles;
	int puntaje;
} jugador_t;

struct juego {
	lista_t *pokemones;
	informacion_pokemon_t *info;
	jugador_t *jugador1;
	int puntaje_adversario;
	bool terminado;
	int rondas;
};

const int MAX_RONDAS_JUEGO = 9;
const int POKEMONES_MINIMOS_PARA_JUGAR = 4;

int comparador_ataques(void *_ataque, void *_nombre)
{
	if (!_ataque || !_nombre)
		return false;

	struct ataque *ataque = (struct ataque *)_ataque;
	const char *nombre = (const char *)_nombre;
	return strcmp(ataque->nombre, nombre);
}

int comparador_pokemones(void *_pokemon, void *_nombre)
{
	if (!_pokemon || !_nombre)
		return false;

	pokemon_t *pokemon = (pokemon_t *)_pokemon;
	const char *nombre = (const char *)_nombre;
	return strcmp(pokemon_nombre(pokemon), nombre);
}

void pasar_pokemones(pokemon_t *actual, void *_lista)
{
	if (!actual || !_lista)
		return;

	lista_t *l = (lista_t *)_lista;
	
	if (!lista_insertar(l, actual))
		return;
}

void pasar_ataques(const struct ataque* actual, void *_lista)
{
	if (!actual || !_lista)
		return;

	lista_t *l = (lista_t *)_lista;

	if (!lista_insertar(l, (struct ataque *)actual))
		return;
}

jugador_t *jugador_crear()
{
	jugador_t *jugador = calloc(1, sizeof(jugador_t));

	if (!jugador)
		return NULL;
	
	jugador->pokemones_elegidos = lista_crear();
	jugador->ataques_disponibles = lista_crear();
	jugador->puntaje = 0;

	return jugador;
}

juego_t *juego_crear()
{
	juego_t *juego = calloc(1, sizeof(juego_t));

	if (!juego)
		return NULL;

	juego->pokemones = lista_crear();
	juego->info = NULL;
	juego->jugador1 = jugador_crear();
	juego->puntaje_adversario = 0;
	juego->terminado = false;
	juego->rondas = 0;

	return juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if (!juego || !archivo || !juego->pokemones)
		return ERROR_GENERAL;

	juego->info = pokemon_cargar_archivo(archivo);

	if (!juego->info) 
		return ERROR_GENERAL;

	con_cada_pokemon(juego->info, pasar_pokemones, juego->pokemones);

	if (lista_tamanio(juego->pokemones) == 0)
		return ERROR_GENERAL;
		
	
	if (lista_tamanio(juego->pokemones) < POKEMONES_MINIMOS_PARA_JUGAR) {
		for(size_t i = 0; i <= lista_tamanio(juego_listar_pokemon(juego)) + 1; i++)
			lista_quitar(juego->pokemones);

		pokemon_destruir_todo(juego->info);
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

bool pokemones_repetidos(const char *nombre1, const char *nombre2, const char *nombre3) 
{
	return (strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3) == 0);
}

void insertar_pokemones_jugador(juego_t *juego, JUGADOR jugador, pokemon_t *p1, pokemon_t *p2, pokemon_t *p3)
{
	if (jugador == JUGADOR1) {
		lista_insertar(juego->jugador1->pokemones_elegidos, p1);
		lista_insertar(juego->jugador1->pokemones_elegidos, p2);
		con_cada_ataque(p1, pasar_ataques, juego->jugador1->ataques_disponibles);
		con_cada_ataque(p2, pasar_ataques, juego->jugador1->ataques_disponibles);

	} else if (jugador == JUGADOR2) {
		lista_insertar(juego->jugador1->pokemones_elegidos, p3);
		con_cada_ataque(p3, pasar_ataques, juego->jugador1->ataques_disponibles);
	}
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if (!juego)
		return ERROR_GENERAL;
	if (pokemones_repetidos(nombre1, nombre2, nombre3))
		return POKEMON_REPETIDO;

	pokemon_t *p1 = lista_buscar_elemento(juego->pokemones, comparador_pokemones, (char *)nombre1);
	pokemon_t *p2 = lista_buscar_elemento(juego->pokemones, comparador_pokemones, (char *)nombre2);
	pokemon_t *p3 = lista_buscar_elemento(juego->pokemones, comparador_pokemones, (char *)nombre3);

	if (!p1 || !p2 || !p3)
		return POKEMON_INEXISTENTE;
		
	insertar_pokemones_jugador(juego, jugador, p1, p2, p3);
	return TODO_OK;
}

bool jugada_valida(juego_t *juego, jugada_t jugada) 
{
	if (!juego)
		return false;

	pokemon_t *pokemon = lista_buscar_elemento(juego->jugador1->pokemones_elegidos, comparador_pokemones, jugada.pokemon);
	struct ataque* ataque = lista_buscar_elemento(juego->jugador1->ataques_disponibles, comparador_ataques, jugada.ataque);

	if (!pokemon || !pokemon_buscar_ataque(pokemon, jugada.ataque) || !ataque)
		return false;

	return true;
}

RESULTADO_ATAQUE calcular_resultado(enum TIPO ataque, enum TIPO pokemon)
{
	if (ataque == NORMAL || pokemon == NORMAL)
		return ATAQUE_REGULAR;

	switch(ataque) {
		case FUEGO:
			if (pokemon == AGUA) return ATAQUE_INEFECTIVO;
			else if (pokemon == PLANTA) return ATAQUE_EFECTIVO;
			break;
		case PLANTA:
			if (pokemon == FUEGO) return ATAQUE_INEFECTIVO;
			else if (pokemon == ROCA) return ATAQUE_EFECTIVO;
			break;		
		case ROCA:
			if (pokemon == PLANTA) return ATAQUE_INEFECTIVO;
			else if (pokemon == ELECTRICO) return ATAQUE_EFECTIVO;
			break;
		case ELECTRICO:
			if (pokemon == ROCA) return ATAQUE_INEFECTIVO;
			else if (pokemon == AGUA) return ATAQUE_EFECTIVO;
			break;
		case AGUA:
			if (pokemon == ELECTRICO) return ATAQUE_INEFECTIVO;
			else if (pokemon == FUEGO) return ATAQUE_EFECTIVO;
			break;
		default:
			return ATAQUE_REGULAR;
			break;
	}

	return ATAQUE_ERROR;
}

int calcular_puntaje(struct ataque *ataque, RESULTADO_ATAQUE resultado, int *puntos)
{
	if (resultado == ATAQUE_EFECTIVO)
		*puntos += ((int)(ataque->poder * 3));
	else if (resultado == ATAQUE_INEFECTIVO) {
		*puntos += (int)(ataque->poder / 2);
		if ((int)ataque->poder % 2 != 0)
			(*puntos)++;
	}
	else
		*puntos = (int)ataque->poder;

	return *puntos;
}

size_t obtener_pos_ataque_usado(lista_t *l, struct ataque* ataque)
{
	for (size_t i = 0; i < lista_tamanio(l); i++)
		if (ataque == lista_elemento_en_posicion(l, i)) 
			return i;
		
	return 0;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	
	resultado_jugada_t resultado = {ATAQUE_ERROR, ATAQUE_ERROR};

	pokemon_t *p1 = lista_buscar_elemento(juego->jugador1->pokemones_elegidos, comparador_pokemones, jugada_jugador1.pokemon);
	pokemon_t *p2 = lista_buscar_elemento(juego->pokemones, comparador_pokemones, jugada_jugador2.pokemon);

	if (!juego || !p1 || !p2 || (!jugada_valida(juego, jugada_jugador1) && !jugada_valida(juego, jugada_jugador2)))
		return resultado;
	else if (!jugada_valida(juego, jugada_jugador1))
		resultado.jugador1 = ATAQUE_ERROR;
	else if (!jugada_valida(juego, jugada_jugador2))
		resultado.jugador2 = ATAQUE_ERROR;

	struct ataque *ataque_p1 = (struct ataque *)pokemon_buscar_ataque(p1, jugada_jugador1.ataque);
	struct ataque *ataque_p2 = (struct ataque *)pokemon_buscar_ataque(p2, jugada_jugador2.ataque);

	if (!ataque_p1 || !ataque_p2)
		return resultado;

	resultado.jugador1 = calcular_resultado(ataque_p1->tipo, pokemon_tipo(p2));
	resultado.jugador2 = calcular_resultado(ataque_p2->tipo, pokemon_tipo(p1));

	calcular_puntaje(ataque_p1, resultado.jugador1, &juego->jugador1->puntaje);
	calcular_puntaje(ataque_p2, resultado.jugador2, &juego->puntaje_adversario);

	size_t pos_ataque_usado = obtener_pos_ataque_usado(juego->jugador1->ataques_disponibles, ataque_p1);
	lista_quitar_de_posicion(juego->jugador1->ataques_disponibles, pos_ataque_usado);

	juego->rondas++;

	if (juego->rondas >= MAX_RONDAS_JUEGO)
		juego->terminado = true;

	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if (!juego)
		return 0;

	if (jugador == JUGADOR1)
		return juego->jugador1->puntaje;
	else if (jugador == JUGADOR2)
		return juego->puntaje_adversario;
	
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	return juego->terminado;
}

void juego_destruir(juego_t *juego)
{
	if (juego) {

		if (juego->info)
			pokemon_destruir_todo(juego->info);	

		if (juego->jugador1) {
			lista_destruir(juego->jugador1->pokemones_elegidos);
			lista_destruir(juego->jugador1->ataques_disponibles);
			free(juego->jugador1);
		}

		if(juego->pokemones)
			lista_destruir(juego->pokemones);

		free(juego);
	}
}
