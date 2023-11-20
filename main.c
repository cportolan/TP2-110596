#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/menu.h"
#include "src/adversario.h"
#include <stdio.h>

bool mostrar_poke(void *p, void* aux)
{
	if (!p)
		return false;
	
	pokemon_t *pokemon = p;
	printf("Nombre: %s\n", pokemon_nombre(pokemon));
	return true;
}

void listar_pokemones(juego_t *juego)
{
	lista_con_cada_elemento(juego_listar_pokemon(juego), mostrar_poke, NULL);
}

/**
* Este main debe ser modificado para que el usuario pueda jugar el juego. Las
* instrucciones existentes son solamente a modo ilustrativo del funcionamiento
* muy alto nivel del juego.
*
* Las interacciones deben realizarse por entrada/salida estandar y estar validadas.
*
* Se aconseja en todo momento mostrar información relevante para el jugador (por
* ejemplo, mostrar puntaje actual y movimientos disponibles) para hacer que el
* juego sea facil de utilizar.
*/

// int main(int argc, char *argv[])
// {
// 	juego_t *juego = juego_crear();

// 	//Pide al usuario un nombre de archivo de pokemones
// 	char *archivo = pedir_archivo();

// 	//Carga los pokemon
// 	juego_cargar_pokemon(juego, archivo);

// 	//Crea un adversario que será utilizado como jugador 2
// 	adversario_t *adversario =
// 		adversario_crear(juego_listar_pokemon(juego));

// 	//Mostrar el listado de pokemones por consola para que el usuario sepa las opciones que tiene
// 	mostrar_pokemon_disponibles(juego);

// 	//Pedirle al jugador por consola que ingrese los 3 nombres de pokemon que quiere utilizar
// 	char *eleccionJugador1, *eleccionJugador2, *eleccionJugador3;
// 	jugador_seleccionar_pokemon(juego, &eleccionJugador1,
// 				    &eleccionJugador2,
// 				    &eleccionJugador3);

// 	//pedirle al adversario que indique los 3 pokemon que quiere usar
// 	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
// 	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
// 				       &eleccionAdversario2,
// 				       &eleccionAdversario3);

// 	//Seleccionar los pokemon de los jugadores
// 	juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1,
// 				  eleccionJugador2, eleccionJugador3);
// 	juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1,
// 				  eleccionAdversario2, eleccionAdversario3);

// 	//informarle al adversario cuales son los pokemon del jugador
// 	adversario_pokemon_seleccionado(adversario, eleccionJugador1,
// 					eleccionJugador2, eleccionJugador3);

// 	while (!juego_finalizado(juego)) {
// 		resultado_jugada_t resultado_ronda;

// 		//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
// 		jugada_t jugada_jugador = jugador_pedir_nombre_y_ataque();

// 		//Pide al adversario que informe el pokemon y ataque para la siguiente ronda
// 		jugada_t jugada_adversario =
// 			adversario_proxima_jugada(adversario);

// 		//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
// 		resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
// 						    jugada_adversario);

// 		//Si se pudo jugar el turno, le informo al adversario la jugada realizada por el jugador
// 		adversario_informar_jugada(adversario, jugada_jugador);
// 	}

// 	juego_destruir(juego);
// }

int main(int argc, char const *argv[])
{
	// Creo el juego.
	juego_t *juego = juego_crear();

	// Creo el menu y agrego sus comandos.
	menu_t *menu = menu_crear();
	menu_agregar_comando(menu, "l", "Listar pokemones", listar_pokemones);
	menu_agregar_comando(menu, "c", "Cargar un archivo", menu_pedir_archivo);
	// menu_agregar_comando(menu, "h", "Mostrar ayuda", mostrar_ayuda);

	printf("Hola USER!\n\nPor favor, ingrese la ruta junto al nombre del archivo con pokemones para comenzar a jugar:\n");
	menu_ejecutar_comando(menu, "c", juego);

	printf("Ingrese comandos a continuacion o escriba 'h' para obtener ayuda.\n");

	while (!juego_finalizado(juego)) {
		printf("TP2> ");
		char linea[10];
		fgets(linea, 10, stdin);
		linea[1] = 0;

		if(!menu_ejecutar_comando(menu, linea, juego))
			printf("Ese comando no existe, por favor, coloque un comando existente o 'h' para obtener ayuda.\n");
	}

	// Destruyo el juego.
	juego_destruir(juego);
}

