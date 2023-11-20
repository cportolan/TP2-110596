#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>
#include <stdbool.h>

// CONSTANTES:

#define MAX_NOMBRE 30
#define MAX_ATAQUES 3
#define MAX_LINEA 200

#define DELIMITADOR ';'

const int DATOS_LINEA_POKEMON = 2;
const int DATOS_LINEA_ATAQUES = 3;
const int LINEAS_POR_POKEMON = 4;
const int CERO_POKEMONES = 0;
const int CANT_DELIMITADORES_LINEA_POKEMON = 1;
const int CANT_DELIMITADORES_LINEA_ATAQUE = 2;

const int PRIMER_ATAQUE = 0;
const int SEGUNDO_ATAQUE = 1;
const int TERCER_ATAQUE = 2;

#define LETRA_ELECTRICO 'E'
#define LETRA_NORMAL 'N'
#define LETRA_PLANTA 'P'
#define LETRA_FUEGO 'F'
#define LETRA_AGUA 'A'
#define LETRA_ROCA 'R'

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo;
	struct ataque ataques[MAX_ATAQUES];
};

struct info_pokemon {
	pokemon_t **pokemones;
	unsigned int cantidad_pokemones;
};

// Recibe un caracter y devuelve el tipo segun mi enum TIPO.
enum TIPO elegir_tipo(char letra)
{
	switch (letra) {
	case LETRA_ELECTRICO:
		return ELECTRICO;
	case LETRA_FUEGO:
		return FUEGO;
	case LETRA_AGUA:
		return AGUA;
	case LETRA_PLANTA:
		return PLANTA;
	case LETRA_ROCA:
		return ROCA;
	case LETRA_NORMAL:
		return NORMAL;
	}
	return NORMAL;
}

// Devuelve cuantos delimitadores hay en una linea.
int cant_delimitadores(char linea[MAX_LINEA])
{
	int contador = 0;
	for (int i = 0; i < strlen(linea); i++) {
		if (linea[i] == DELIMITADOR) {
			contador++;
		}
	}

	return contador;
}

// Devuelve true si las lineas contienen los delimitadores correctos.
bool delimitadores_validos(char l1[MAX_LINEA], char l2[MAX_LINEA],
			   char l3[MAX_LINEA], char l4[MAX_LINEA])
{
	return ((cant_delimitadores(l1) == CANT_DELIMITADORES_LINEA_POKEMON) &&
		(cant_delimitadores(l2) == CANT_DELIMITADORES_LINEA_ATAQUE) &&
		(cant_delimitadores(l3) == CANT_DELIMITADORES_LINEA_ATAQUE) &&
		(cant_delimitadores(l4) == CANT_DELIMITADORES_LINEA_ATAQUE));
}

// Devuelve true si el char recibido es igual a algun tipo de ataque/pokemon.
bool tipo_valido(char tipo)
{
	return ((tipo == LETRA_ELECTRICO) || (tipo == LETRA_FUEGO) ||
		(tipo == LETRA_AGUA) || (tipo == LETRA_PLANTA) ||
		(tipo == LETRA_ROCA) || (tipo == LETRA_NORMAL));
}

// Devuelve true si el int recibido es > 0.
bool poder_valido(int poder)
{
	return (poder > 0);
}

// Devuelve true si los datos leidos son correctos respecto al formato de linea y validaciones del tipo y poder.
bool ataque_valido(char linea[MAX_LINEA])
{
	char nombre[MAX_NOMBRE] = " ";
	char tipo = ' ';
	int poder = 0;

	sscanf(linea, "%[^;];%c;%d", nombre, &tipo, &poder);

	return tipo_valido(tipo) && poder_valido(poder);
}

// Devuelve true si los datos leidos son correctos respecto al formato de linea y validaciones del tipo de pokemon.
bool pokemon_valido(char linea[MAX_LINEA])
{
	char nombre[MAX_NOMBRE] = " ";
	char tipo = ' ';

	sscanf(linea, "%[^;];%c\n", nombre, &tipo);

	return tipo_valido(tipo);
}

// Devuelve true si todos los bools llamados dentro (para validar pokemon y ataque) son validos.
bool lineas_validas(char l1[MAX_LINEA], char l2[MAX_LINEA], char l3[MAX_LINEA],
		    char l4[MAX_LINEA])
{
	return pokemon_valido(l1) && ataque_valido(l2) && ataque_valido(l3) &&
	       ataque_valido(l4) && delimitadores_validos(l1, l2, l3, l4);
}

// Recibe un pokemon y la linea para leer. Lee, almacena los datos en variables y parsea los datos en sus respectivos campos del struct.
void parsear_pokemon(pokemon_t *pokemon, char linea[])
{
	char nombre[MAX_NOMBRE];
	char tipo;

	int datos_pokemon = sscanf(linea, "%[^;];%c\n", nombre, &tipo);

	if (datos_pokemon == DATOS_LINEA_POKEMON) {
		strcpy(pokemon->nombre, nombre);
		pokemon->tipo = elegir_tipo(tipo);
	}
}

// Recibe un pokemon, una linea para leer y el indice en el cual se debe almacenar el ataque. Lee, almacena los datos en variables y parsea los datos en sus respectivos campos del struct.
void parsear_ataque(pokemon_t *pokemon, char linea[], int num_ataque)
{
	char nombre[MAX_NOMBRE];
	char tipo = ' ';
	int poder = 0;

	int datos_leidos =
		sscanf(linea, "%[^;];%c;%i\n", nombre, &tipo, &poder);

	if (datos_leidos == DATOS_LINEA_ATAQUES) {
		strcpy(pokemon->ataques[num_ataque].nombre, nombre);
		pokemon->ataques[num_ataque].tipo = elegir_tipo(tipo);
		pokemon->ataques[num_ataque].poder = (unsigned)poder;
	}
}

// Ordena los pokemones en orden alfabetico (segun su nombre) utilizando el metodo Bubble Sort.
void ordenar_pokemones(informacion_pokemon_t *ip)
{
	pokemon_t *aux;

	if (ip != NULL) {
		for (int i = 0; i < (int)ip->cantidad_pokemones - 1; i++) {
			for (int j = 0; j < (int)ip->cantidad_pokemones - i - 1;
			     j++) {
				if (strcmp(ip->pokemones[j]->nombre,
					   ip->pokemones[j + 1]->nombre) > 0) {
					aux = ip->pokemones[j];
					ip->pokemones[j] = ip->pokemones[j + 1];
					ip->pokemones[j + 1] = aux;
				}
			}
		}
	}
}

// Recibe las cuatro lineas leidas y llamo a las funciones parsear para almacenar los datos en un struct auxiliar. Pido un bloque de memoria con calloc y al mismo tiempo inicializo el struct pokemon_t.
pokemon_t *leer_pokemon(char l1[MAX_LINEA], char l2[MAX_LINEA],
			char l3[MAX_LINEA], char l4[MAX_LINEA])
{
	pokemon_t *p = calloc(1, sizeof(pokemon_t));

	if (!p)
		return NULL;

	parsear_pokemon(p, l1);
	parsear_ataque(p, l2, PRIMER_ATAQUE);
	parsear_ataque(p, l3, SEGUNDO_ATAQUE);
	parsear_ataque(p, l4, TERCER_ATAQUE);

	return p;
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL)
		return NULL;

	FILE *archivo = fopen(path, "r");

	if (archivo == NULL)
		return NULL;

	informacion_pokemon_t *informacion =
		calloc(1, sizeof(informacion_pokemon_t));
	pokemon_t *aux;

	if (informacion == NULL)
		return NULL;

	char linea_p[MAX_LINEA], linea_a1[MAX_LINEA], linea_a2[MAX_LINEA],
		linea_a3[MAX_LINEA];
	int leidos = 0;

	while ((fscanf(archivo, "%s\n%s\n%s\n%s\n", linea_p, linea_a1, linea_a2,
		       linea_a3) == LINEAS_POR_POKEMON) &&
	       lineas_validas(linea_p, linea_a1, linea_a2, linea_a3)) {
		aux = leer_pokemon(linea_p, linea_a1, linea_a2, linea_a3);

		if (aux != NULL) {
			informacion->pokemones =
				realloc(informacion->pokemones,
					(1 + informacion->cantidad_pokemones) *
						(sizeof(pokemon_t)));
			informacion->pokemones[informacion->cantidad_pokemones] =
				aux;
			informacion->cantidad_pokemones++;
			leidos++;
		} else {
			fclose(archivo);
			pokemon_destruir_todo(informacion);
			return NULL;
		}
	}

	if (leidos == 0) {
		fclose(archivo);
		pokemon_destruir_todo(informacion);
		return NULL;
	}

	fclose(archivo);
	return informacion;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (nombre == NULL) {
		return NULL;
	}

	bool encontrado = false;
	int pos_encontrado = 0;

	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		if (strcmp(nombre, ip->pokemones[i]->nombre) == 0) {
			encontrado = true;
			pos_encontrado = i;
		}
	}

	if (encontrado) 
		return ip->pokemones[pos_encontrado];
	else 
		return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL)
		return 0;

	return (int)ip->cantidad_pokemones;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NORMAL;

	return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	bool encontrado = false;
	int pos_encontrado = 0;

	for (int i = 0; i < MAX_ATAQUES; i++) {
		if (strcmp(pokemon->ataques[i].nombre, nombre) == 0) {
			encontrado = true;
			pos_encontrado = i;
		}
	}

	if (encontrado)
		return &pokemon->ataques[pos_encontrado];
	else 
		return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	int veces_aplicada = 0;

	if (!f || !ip)
		return veces_aplicada;

	ordenar_pokemones(ip);

	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		if (f != NULL) {
			f(ip->pokemones[i], aux);
			veces_aplicada++;
		} else {
			return veces_aplicada;
		}
	}

	return veces_aplicada;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	int veces_aplicada = 0;

	if (!f || !pokemon)
		return veces_aplicada;

	for (int i = 0; i < MAX_ATAQUES; i++) {
		if (f != NULL) {
			f(&pokemon->ataques[i], aux);
			veces_aplicada++;
		} else {
			return veces_aplicada;
		}
	}
	return veces_aplicada;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip) {
		for (int i = 0; i < ip->cantidad_pokemones; i++)
			free(ip->pokemones[i]);

		free(ip->pokemones);
		free(ip);
	}
}
