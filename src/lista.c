#include "lista.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *nodo_inicio;
	nodo_t *nodo_final;
	size_t cantidad;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *actual;
};

// Reserva memoria en el heap utilizando malloc, crea un nodo con el elemento pasado por parametro.
nodo_t *crear_nuevo_nodo_lista(void *elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));

	if (!nodo)
		return NULL;

	nodo->elemento = elemento;
	nodo->siguiente = NULL;

	return nodo;
}

nodo_t *nodo_en_posicion(lista_t *lista, size_t pos)
{
	if (!lista || !lista->nodo_inicio)
		return NULL;

	nodo_t *actual = lista->nodo_inicio;

	for (size_t i = 0; i < pos; i++)
		actual = actual->siguiente;

	return actual;
}

lista_t *lista_crear()
{
	lista_t *lista = malloc(sizeof(lista_t));

	if (!lista)
		return NULL;

	lista->cantidad = 0;
	lista->nodo_inicio = NULL;
	lista->nodo_final = NULL;

	return lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (!lista)
		return NULL;

	nodo_t *nuevo_nodo = crear_nuevo_nodo_lista(elemento);

	nuevo_nodo->siguiente = NULL;

	if (lista->cantidad == 0)
		lista->nodo_inicio = nuevo_nodo;
	else
		lista->nodo_final->siguiente = nuevo_nodo;

	lista->nodo_final = nuevo_nodo;
	lista->cantidad++;

	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (!lista)
		return NULL;

	nodo_t *nuevo = crear_nuevo_nodo_lista(elemento);

	if (posicion >= lista->cantidad) {
		lista_insertar(lista, elemento);
		free(nuevo);
		return lista;

	} else if (posicion == 0) {
		nuevo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo;
		lista->cantidad++;
		return lista;
	}

	nodo_t *pos_anterior = nodo_en_posicion(lista, posicion - 1);
	nuevo->siguiente = pos_anterior->siguiente;
	pos_anterior->siguiente = nuevo;
	lista->cantidad++;

	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	nodo_t *borrado = lista->nodo_final;
	void *elemento = borrado->elemento;

	if (lista->nodo_final == lista->nodo_inicio) {
		lista->nodo_final = NULL;
		lista->nodo_inicio = NULL;

	} else {
		nodo_t *nuevo_final =
			nodo_en_posicion(lista, lista->cantidad - 2);
		lista->nodo_final = nuevo_final;
		lista->nodo_final->siguiente = NULL;
	}

	free(borrado);
	lista->cantidad--;
	return elemento;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || lista_vacia(lista))
		return NULL;

	if (posicion >= lista->cantidad)
		return lista_quitar(lista);

	nodo_t *nodo_eliminar;
	void *elemento_borrado;

	if (posicion == 0) {
		nodo_eliminar = lista->nodo_inicio;
		elemento_borrado = lista->nodo_inicio->elemento;

		if (lista->nodo_inicio->siguiente)
			lista->nodo_inicio = lista->nodo_inicio->siguiente;

		free(nodo_eliminar);
		lista->cantidad--;
		return elemento_borrado;
	}

	nodo_t *pos_anterior = nodo_en_posicion(lista, posicion - 1);
	nodo_eliminar = pos_anterior->siguiente;
	pos_anterior->siguiente = nodo_eliminar->siguiente;
	elemento_borrado = nodo_eliminar->elemento;
	free(nodo_eliminar);
	lista->cantidad--;

	return elemento_borrado;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (!lista || posicion >= lista->cantidad)
		return NULL;

	if (posicion == 0)
		return lista->nodo_inicio->elemento;
	else if (posicion == lista->cantidad - 1)
		return lista->nodo_final->elemento;

	nodo_t *pedido = nodo_en_posicion(lista, posicion);

	return pedido->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (!lista || !comparador || lista_vacia(lista))
		return NULL;

	nodo_t *actual = lista->nodo_inicio;

	for (size_t i = 0; i < lista->cantidad; i++) {
		if (!actual)
			return NULL;
		else if (comparador(actual->elemento, contexto) == 0)
			return actual->elemento;

		actual = actual->siguiente;
	}

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (!lista || !lista->nodo_inicio)
		return NULL;

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (!lista || !lista->nodo_final)
		return NULL;

	return lista->nodo_final->elemento;
}

bool lista_vacia(lista_t *lista)
{
	return lista_tamanio(lista) == 0;
}

size_t lista_tamanio(lista_t *lista)
{
	if (!lista)
		return 0;

	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (lista) {
		if (!lista_vacia(lista)) {
			nodo_t *actual = lista->nodo_inicio;
			nodo_t *aux;

			for (size_t i = 0; i < lista->cantidad; i++) {
				aux = actual->siguiente;
				free(actual);
				actual = aux;
			}
			free(actual);
		}
		free(lista);
	}
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista) {
		nodo_t *actual = lista->nodo_inicio;

		for (size_t i = 0; i < lista->cantidad; i++) {
			if (funcion)
				funcion(actual->elemento);
			actual = actual->siguiente;
		}

		lista_destruir(lista);
	}
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista)
		return NULL;

	struct lista_iterador *it = malloc(sizeof(struct lista_iterador));

	if (!it)
		return NULL;

	it->actual = lista->nodo_inicio;
	it->lista = lista;

	return it;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista))
		return false;

	return iterador->actual != NULL;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista))
		return NULL;

	if (iterador->actual != NULL) {
		iterador->actual = iterador->actual->siguiente;
		if (!iterador->actual)
			return false;

		return true;
	}

	return false;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (!iterador || lista_vacia(iterador->lista) || !(iterador->actual))
		return NULL;

	return iterador->actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador)
		free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (!lista || !funcion)
		return 0;

	nodo_t *actual = lista->nodo_inicio;
	size_t iterados = 0;

	for (size_t i = 0; i < lista->cantidad; i++) {
		iterados++;

		if (!funcion(actual->elemento, contexto))
			return iterados;

		actual = actual->siguiente;
	}

	return iterados;
}
