#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/funcionamiento.h"
#include "src/adversario.h"
#include <stdio.h>
#include <time.h>

#define LETRA_INICIAR_JUEGO 'i'
#define LETRA_SALIR_JUEGO 'q'

int main(int argc, char const *argv[])
{
	srand((unsigned int)time(NULL));

	bool arrancar_juego = false;
	while (!arrancar_juego) {
		mostrar_bienvenida();
		char comando = ingresar_comando();
		if (comando == LETRA_INICIAR_JUEGO)
			arrancar_juego = true;
		else if (comando == LETRA_SALIR_JUEGO) {
			quit();
			return 0;
		}
	}

	juego_t *juego = juego_crear();

	if (!juego)
		return 0;

	adversario_t *adversario =
		adversario_crear(juego_listar_pokemon(juego));

	if (!adversario) {
		juego_destruir(juego);
		return 0;
	}

	partida_t *partida = crear_struct_partida(juego);

	if (!partida) {
		juego_destruir(juego);
		adversario_destruir(adversario);
		return 0;
	}

	int rondas = 0;
	inicializar_juego(juego, adversario, partida);
	jugada_t jugada_jugador = { .pokemon = " ", .ataque = " " };

	while (!juego_finalizado(juego)) {
		jugada_jugador = elegir_jugada(partida);

		while (!jugada_valida(juego, jugada_jugador, partida)) {
			jugada_jugador = elegir_jugada(partida);
		}

		jugada_t jugada_adversario =
			adversario_proxima_jugada(adversario);
		resultado_jugada_t resultado_ronda = juego_jugar_turno(
			juego, jugada_jugador, jugada_adversario);

		if (resultado_ronda.jugador1 != ATAQUE_ERROR) {
			printf("\n-> El jugador 1 tiene %i puntos.\n",
			       juego_obtener_puntaje(juego, JUGADOR1));
			printf("-> El adversario tiene %i puntos.\n",
			       juego_obtener_puntaje(juego, JUGADOR2));
			printf("Se jugo la ronda correctamente.\n");
			rondas++;
			printf("-> RONDAS: %i", rondas);
		}
	}

	elegir_ganador(juego);
	destruir_struct_partida(partida);
	adversario_destruir(adversario);
	juego_destruir(juego);
}
