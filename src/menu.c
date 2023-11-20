#include "menu.h"
#include "lista.h"
#include "pokemon.h"
#include "juego.h"

struct comando {
    char *instruccion;
    char *descripcion;
    void (*funcion)(juego_t *);
};

struct menu {
    lista_t* comandos;
};

void menu_pedir_archivo(juego_t *juego) {
	char *a = malloc(256);

	if (!a)
		return;

	printf("Debes ingresar e archivo con pokemones para iniciar el juego.\n");
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

// menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion)
// {
// 	if (!menu || !instruccion)
// 		return NULL;

// 	size_t pos = 0;

// 	lista_iterador_t *lista_iterador = lista_iterador_crear(menu->comandos);

// 	bool encontrado = false;

// 	while (lista_iterador_tiene_siguiente(lista_iterador) && !encontrado) {
// 		pos++;
// 		comando_t *comando = lista_iterador_elemento_actual(lista_iterador);

// 		if (strcmp(comando->instruccion, instruccion) == 0) {
// 			pos--;
// 			encontrado = true;
// 		}
// 	}

// 	if (!encontrado)
// 		return NULL;

// 	comando_t *eliminado = lista_quitar_de_posicion(menu->comandos, pos);
// 	free(eliminado);
// 	lista_iterador_destruir(lista_iterador);
// 	return menu;
// }

void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;

	lista_destruir_todo(menu->comandos, free);
	free(menu);
}