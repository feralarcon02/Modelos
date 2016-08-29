#include "simlib.c"
#include <stdio.h>
#include <stdlib.h>

// Eventos
#define FinProduccionPieza				1
#define ArriboMontacargaEstacion		2	
#define ArriboMontacargaAlmacen	        3  

//Colas y Listas
#define ColaEstacion1					1
#define ColaEstacion2					2
#define ColaEstacion3					3
#define ColaEstacion4					4
#define ListaMontacarga					5
#define Piezas							6

#define DemoraColaEstacion1				1
#define DemoraColaEstacion2				2
#define DemoraColaEstacion3				3
#define DemoraColaEstacion4				4

// Estructura transfer eventos.
#define E1Tiempo						1
#define E2TipoEvento					2
#define E3NroEstacion					3


// Variables
int piezasPorHoraEst1yEst2, piezasPorHoraEst3yEst4, mediaPiezasEst1yEst2, mediaPiezasEst3yEst4, tiempoInterarribosAlmacen, tiempoFinalSimulacion, totalCircuitos, circuitoDoble, i;
float tiempoInterarribosEst;

enum Estaciones
{
	Estacion1 = 1,
	Estacion2 = 2,
	Estacion3 = 3,
	Estacion4 = 4
};

// Rutinas
int main();
void Inicializacion(void);
void RutinaArriboEstacion(void);
void RutinaArriboAlmacen(void);
void RutinaFinProduccionPieza(void);
void CargarMontaCargas(int cola);
int	 CargaActualMontaCargas(void);
void NuevoFinProduccionPieza(float tiempo, int nroEstacion);
void NuevoArriboMontacargaEstacion(float tiempo, int nroEstacion);
void Reporte(void);

//Función Principal
int main()
{
	// Inicializar SimLib
	init_simlib();

	// Rutina Inicializar
	Inicializacion();

	// Comienzo Simulación
	while (sim_time <= tiempoFinalSimulacion)
	{
		// Rutina Timing
		timing();

		// Determinar tipo de evento y llamar a su rutina
		switch (next_event_type)
		{
		case FinProduccionPieza:
			RutinaFinProduccionPieza();
			break;
		case ArriboMontacargaEstacion:
			RutinaArriboEstacion();
			break;
		case ArriboMontacargaAlmacen:
			RutinaArriboAlmacen();
			break;
		}
	}
	// Fin de Simulación - Generar Reporte
	Reporte();

	// Finalizar Simulación
	system("pause");
}

// Inicializar Sistema
void Inicializacion(void)
{
	// Inicializar variables del sistema.
	piezasPorHoraEst1yEst2 = 12;
	piezasPorHoraEst3yEst4 = 20;

	mediaPiezasEst1yEst2 = (60 / piezasPorHoraEst1yEst2)*60;
	mediaPiezasEst3yEst4 = (60 / piezasPorHoraEst3yEst4)*60;

	tiempoInterarribosEst = 30;
	tiempoInterarribosAlmacen = 300;

	// 60 minutos  * 24 horas * 5 días * 60 segundos.
	tiempoFinalSimulacion = 432000;

	totalCircuitos = 0;
	circuitoDoble = 0;

	// Generar eventos FinProduccionPieza para cada estación.
	NuevoFinProduccionPieza(expon(mediaPiezasEst1yEst2, FinProduccionPieza), Estaciones::Estacion1);

	NuevoFinProduccionPieza(expon(mediaPiezasEst1yEst2, FinProduccionPieza), Estaciones::Estacion2);

	NuevoFinProduccionPieza(expon(mediaPiezasEst3yEst4, FinProduccionPieza), Estaciones::Estacion3);

	NuevoFinProduccionPieza(expon(mediaPiezasEst3yEst4, FinProduccionPieza), Estaciones::Estacion4);

	// Generar ArriboMontacargaEstacion en la estación 1.
	NuevoArriboMontacargaEstacion(tiempoInterarribosEst, Estaciones::Estacion1);
}

void RutinaArriboEstacion(void)
{
	switch ((int)transfer[E3NroEstacion])
	{
		case(Estaciones::Estacion1):
		{
			CargarMontaCargas(ColaEstacion1);
			NuevoArriboMontacargaEstacion(tiempoInterarribosEst, Estaciones::Estacion2);

			break;
		}			
		case(Estaciones::Estacion2):
		{
			CargarMontaCargas(ColaEstacion2);
			NuevoArriboMontacargaEstacion(tiempoInterarribosEst, Estaciones::Estacion3);

			break;
		}			
		case(Estaciones::Estacion3):
		{
			CargarMontaCargas(ColaEstacion3);
			NuevoArriboMontacargaEstacion(tiempoInterarribosEst, Estaciones::Estacion4);

			break;
		}			
		case(Estaciones::Estacion4):
		{
			CargarMontaCargas(ColaEstacion4);

			// Verificar si el montacargas tiene más del 50% libre.
			if (CargaActualMontaCargas() < 3)
			{
				// Generar ArriboMontacargaEstacion en la estación 1.
				NuevoArriboMontacargaEstacion(tiempoInterarribosEst, Estaciones::Estacion1);

				circuitoDoble++;
			}
			else
			{
				// Generar próximo ArriboMontacargaAlmacen.
				transfer[E1Tiempo] = sim_time + tiempoInterarribosAlmacen;
				transfer[E2TipoEvento] = ArriboMontacargaAlmacen;
				transfer[E3NroEstacion] = 0;
				list_file(INCREASING, LIST_EVENT);
			}
			/*totalCircuitos++;*/
			break;
		}
			
	}
}

void RutinaArriboAlmacen(void)
{
	while (list_size[ListaMontacarga] > 0)
	{
		list_remove(FIRST, ListaMontacarga);
	}

	while (list_size[Piezas] > 0)
	{
		// Vacio la lista de piezas.
		list_remove(FIRST, Piezas);
	}

	totalCircuitos++;

	// Generar ArriboMontacargaEstacion en la estación 1.
	NuevoArriboMontacargaEstacion(tiempoInterarribosEst, Estaciones::Estacion1);
}

void RutinaFinProduccionPieza(void)
{
	switch ((int)transfer[3])
	{
		case(Estaciones::Estacion1):
		{
			transfer[E1Tiempo] = sim_time;
			list_file(LAST, ColaEstacion1);

			// Generar próximo FinProduccionPieza.
			NuevoFinProduccionPieza(expon(mediaPiezasEst1yEst2, FinProduccionPieza), Estaciones::Estacion1);

			break;
		}
		case(Estaciones::Estacion2):
		{
			transfer[E1Tiempo] = sim_time;
			list_file(LAST, ColaEstacion2);

			// Generar próximo FinProduccionPieza.
			NuevoFinProduccionPieza(expon(mediaPiezasEst1yEst2, FinProduccionPieza), Estaciones::Estacion2);

			break;
		}	
		case(Estaciones::Estacion3):
		{
			transfer[E1Tiempo] = sim_time;
			list_file(LAST, ColaEstacion3);

			// Generar próximo FinProduccionPieza.
			NuevoFinProduccionPieza(expon(mediaPiezasEst3yEst4, FinProduccionPieza), Estaciones::Estacion3);

			break;
		}		
		case(Estaciones::Estacion4):
		{
			transfer[E1Tiempo] = sim_time;
			list_file(LAST, ColaEstacion4);

			// Generar próximo FinProduccionPieza.
			NuevoFinProduccionPieza(expon(mediaPiezasEst3yEst4, FinProduccionPieza), Estaciones::Estacion4);

			break;
		}
	}
}

void CargarMontaCargas(int cola)
{
	switch (cola)
	{
		case(ColaEstacion1):
		{
			while ((list_size[ColaEstacion1] > 0) && ((6 - CargaActualMontaCargas()) >= 1))
			{
				list_remove(FIRST, ColaEstacion1);

				sampst(sim_time - transfer[E1Tiempo], DemoraColaEstacion1);

				list_file(LAST, ListaMontacarga);
				list_file(LAST, Piezas);
			}

			break;
		}
		case(ColaEstacion2):
		{
			while ((list_size[ColaEstacion2] > 0) && ((6 - CargaActualMontaCargas()) >= 2))
			{
				list_remove(FIRST, ColaEstacion2);

				sampst(sim_time - transfer[E1Tiempo], DemoraColaEstacion2);

				list_file(LAST, ListaMontacarga);
				list_file(LAST, Piezas);
			}

			break;
		}
		case(ColaEstacion3):
		{
			while ((list_size[ColaEstacion3] > 0) && ((6 - CargaActualMontaCargas()) >= 3))
			{
				list_remove(FIRST, ColaEstacion3);

				sampst(sim_time - transfer[E1Tiempo], DemoraColaEstacion3);

				list_file(LAST, ListaMontacarga);
				list_file(LAST, Piezas);
			}

			break;
		}
		case(ColaEstacion4):
		{
			while ((list_size[ColaEstacion4] > 0) && ((6 - CargaActualMontaCargas()) >= 3))
			{
				list_remove(FIRST, ColaEstacion4);

				sampst(sim_time - transfer[E1Tiempo], DemoraColaEstacion4);

				list_file(LAST, ListaMontacarga);
				list_file(LAST, Piezas);
			}

			break;
		}
	}
}

int CargaActualMontaCargas(void)
{
	int lugaresOcupados = 0;
	for (int i = 0; i < list_size[ListaMontacarga]; i++)
	{
		list_remove(FIRST, ListaMontacarga);
		switch ((int)transfer[E3NroEstacion])
		{
			case Estaciones::Estacion1:
			{
				lugaresOcupados++;
				break;
			}				
			case Estaciones::Estacion2:
			{
				lugaresOcupados += 2;
				break;
			}
			case Estaciones::Estacion3:
			{
				lugaresOcupados += 3;
				break;
			}
			case Estaciones::Estacion4:
			{
				lugaresOcupados += 3;
				break;
			}
		}
		list_file(LAST, ListaMontacarga);
	}
	return lugaresOcupados;
}

void NuevoFinProduccionPieza(float tiempo, int nroEstacion)
{
	transfer[E1Tiempo] = sim_time + tiempo;
	transfer[E2TipoEvento] = FinProduccionPieza;
	transfer[E3NroEstacion] = nroEstacion;
	list_file(INCREASING, LIST_EVENT);
}

void NuevoArriboMontacargaEstacion(float tiempo, int nroEstacion)
{
	transfer[E1Tiempo] = sim_time + tiempo;
	transfer[E2TipoEvento] = ArriboMontacargaEstacion;
	transfer[E3NroEstacion] = nroEstacion;
	list_file(INCREASING, LIST_EVENT);
}

// Generar Reporte de Simulación
void Reporte(void)
{
	// Impresión en Pantalla
	printf("TP - Sistema 5 \n\n");
	printf("Estadisticos:\n\n");

	sampst(0, -DemoraColaEstacion1);
	// Demora media y maxima de las piezas en cada estacion
	sampst(0, -DemoraColaEstacion1);
	printf("Estacion1: Demora media = %.3f segundos			Demora maxima = %.3f segundos\n", transfer[E1Tiempo], transfer[E3NroEstacion]);
	sampst(0, -DemoraColaEstacion2);
	printf("Estacion2: Demora media = %.3f segundos			Demora maxima = %.3f segundos\n", transfer[E1Tiempo], transfer[E3NroEstacion]);
	sampst(0, -DemoraColaEstacion3);
	printf("Estacion3: Demora media = %.3f segundos			Demora maxima = %.3f segundos\n", transfer[E1Tiempo], transfer[E3NroEstacion]);
	sampst(0, -DemoraColaEstacion4);
	printf("Estacion4: Demora media = %.3f segundos			Demora maxima = %.3f segundos\n", transfer[E1Tiempo], transfer[E3NroEstacion]);
	// Numero medio en cada una de las colas
	printf("\n");
	filest(ColaEstacion1);
	printf("Numero medio en la cola de la Estacion1 =		%.3f piezas\n", transfer[E1Tiempo]);
	filest(ColaEstacion2);
	printf("Numero medio en la cola de la Estacion2 =		%.3f piezas\n", transfer[E1Tiempo]);
	filest(ColaEstacion3);
	printf("Numero medio en la cola de la Estacion3 =		%.3f piezas\n", transfer[E1Tiempo]);
	filest(ColaEstacion4);
	printf("Numero medio en la cola de la Estacion4 =		%.3f piezas\n", transfer[E1Tiempo]);
	printf("\n");
	filest(Piezas);
	printf("Cantidad promedio de piezas transportadas por el Montacargas 			: %.3f piezas\n", transfer[E1Tiempo]);

	printf("Porcentaje de circuito doble							: %f \n\n", (float(circuitoDoble * 100) / totalCircuitos));

	printf("Viajes =		%i %\n", totalCircuitos);
	printf("Viajes con vuelta =		%i %\n", circuitoDoble);

	/*printf("\nTiempo Final de Simulation     : %10.3f minutes\n", sim_time);*/
	printf("\n");
}