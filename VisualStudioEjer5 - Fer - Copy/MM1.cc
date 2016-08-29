/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

/*Eventos*/
#define Fin_Generacion_Pieza           1
#define Cargar_Montacargas             2
#define Vaciar_Montacargas             3

/*Colas*/
#define Cola_Piezas_1                  1  
#define Cola_Piezas_2                  2 
#define Cola_Piezas_3                  3 
#define Cola_Piezas_4                  4 

/*Servidores, continuar numeracion de las colas*/
#define Montacargas                    5

/*Demoras, otra numeracion distinta*/
#define Demora_Cola_1                  1
#define Demora_Cola_2                  2
#define Demora_Cola_3                  3
#define Demora_Cola_4                  4
#define Uso_Tecnico_1                  5
#define Porcentaje_Repite_Circuito     6


#define Media_Procesador_limpieza      1
#define Media_Procesamiento_Ensamblado 2


/* Declaraci¢n de variables propias */

float tiempo_proxima_parada, tiempo_siguiente_carga;
int timepo_piezas_menor, tiempo_piezas_mayor;
bool trabajando, reparador_extra;


/* Declaraci¢n de Funciones propias */

void Rutina_Fin_Generacion_Pieza(void);
void Rutina_Cargar_Montacargas(void);
void Rutina_Vaciar_Montacargas(void);
void inicializa(void);
void reporte(void);


int main()  /* Main function. */
{

	/* Initializar Simlib */
	init_simlib();


	/* Establecer maxatr = M ximo n£mero de Atributos utilizados (Numero máximo que voy a utilizar del transfer)  */
	maxatr = 7;

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */


	while (sim_time <= 7200)
	{
		/* Determinar pr¢ximo Evento */
 		timing();

		/* Invoke the appropriate event function. */

		switch (next_event_type)
		{
			
		case Fin_Generacion_Pieza:
			Rutina_Fin_Generacion_Pieza();
			break;
		case Cargar_Montacargas:
			Rutina_Cargar_Montacargas();
			break;
		case Vaciar_Montacargas:
			Rutina_Vaciar_Montacargas();
			break;
		}
	}

	/* Llamada al Reporte para mostrar los resultados */
	reporte();
	//getchar();
	system("pause");
}


void inicializa(void)  /* Inicializar el Sistema */
{
	/* Se carga el primer Arribo en la Lista de Eventos */
	tiempo_siguiente_carga = 0.5;
	timepo_piezas_menor = 5;
	tiempo_piezas_mayor = 3;
	tiempo_proxima_parada = 0.5;

	transfer[1] = sim_time + tiempo_siguiente_carga;
	transfer[2] = Cargar_Montacargas;
	transfer[3] = Cola_Piezas_1;
	list_file(INCREASING, LIST_EVENT);

	for (int i = 1; i <= 4; i++) {
		if (i <= 2) {
			transfer[1] = sim_time + expon(timepo_piezas_menor, Fin_Generacion_Pieza);
			transfer[3] = i;
		}
		else {
			transfer[1] = sim_time + expon(tiempo_piezas_mayor, Fin_Generacion_Pieza);
			transfer[3] = i;
		}
		transfer[2] = Fin_Generacion_Pieza;
		list_file(INCREASING, LIST_EVENT);
	}
}

void Rutina_Fin_Generacion_Pieza(void)
{
	int x = transfer[3];
	transfer[1] = sim_time;
	switch (x)
	{
		case 1:
			list_file(LAST, Cola_Piezas_1);
			break;
		case 2:
			list_file(LAST, Cola_Piezas_2);
			break;
		case 3:
			list_file(LAST, Cola_Piezas_3);
			break;
		case 4:
			list_file(LAST, Cola_Piezas_4);
			break;
	}

	if (x <= 2) {
		transfer[1] = sim_time + expon(timepo_piezas_menor, Fin_Generacion_Pieza);
	}
	else {
		transfer[1] = sim_time + expon(tiempo_piezas_mayor, Fin_Generacion_Pieza);
	}
	transfer[2] = Fin_Generacion_Pieza;
	transfer[3] = x;
	list_file(INCREASING, LIST_EVENT);

}

void Rutina_Cargar_Montacargas(void) {

	int x = transfer[3];
	switch (x)
	{
	case 1:
		while ((list_size[Cola_Piezas_1] > 0) & (list_size[Montacargas] < 6)) {
			list_remove(FIRST, Cola_Piezas_1);
			sampst(sim_time - transfer[1], Demora_Cola_1);
			list_file(FIRST, Montacargas);
		}
		break;
	case 2:
		while ((list_size[Cola_Piezas_2] > 0) & (list_size[Montacargas] <= 4)) {
			list_remove(FIRST, Cola_Piezas_2);
			sampst(sim_time - transfer[1], Demora_Cola_2);
			list_file(FIRST, Montacargas);
			list_file(FIRST, Montacargas);
		}
		break;
	case 3:
		while ((list_size[Cola_Piezas_3] > 0) & (list_size[Montacargas] <= 3)) {
			list_remove(FIRST, Cola_Piezas_3);
			sampst(sim_time - transfer[1], Demora_Cola_3);
			list_file(FIRST, Montacargas);
			list_file(FIRST, Montacargas);
			list_file(FIRST, Montacargas);
		}
		break;
	case 4:
		while ((list_size[Cola_Piezas_4] > 0) & (list_size[Montacargas] <= 3)) {
			list_remove(FIRST, Cola_Piezas_4);
			sampst(sim_time - transfer[1], Demora_Cola_4);
			list_file(FIRST, Montacargas);
			list_file(FIRST, Montacargas);
			list_file(FIRST, Montacargas);	
		}
		break;
	}

	/*Me fijo cual es la proxima parada.*/

	if (x == 4) {
		if (list_size[Montacargas] > 3) {
			transfer[1] = sim_time + 5;
			transfer[2] = Vaciar_Montacargas;
			transfer[3] = 0;
			list_file(INCREASING, LIST_EVENT);
			timest(0, Porcentaje_Repite_Circuito);
		}
		else
		{
			transfer[1] = sim_time + tiempo_proxima_parada;
			transfer[2] = Cargar_Montacargas;
			transfer[3] = 1;
			list_file(INCREASING, LIST_EVENT);
			timest(1, Porcentaje_Repite_Circuito);
		}
	}
	else {
		transfer[1] = sim_time + tiempo_proxima_parada;
		transfer[2] = Cargar_Montacargas;
		transfer[3] = x + 1;
		list_file(INCREASING, LIST_EVENT);
	}
}

void Rutina_Vaciar_Montacargas(void) {

	while (list_size[Montacargas] > 0) {
		list_remove(FIRST, Montacargas);
	}

	transfer[1] = sim_time + 0.5;
	transfer[2] = Cargar_Montacargas;
	transfer[3] = Cola_Piezas_1;
	list_file(INCREASING, LIST_EVENT);
	
}


void reporte(void)  
{
	
	sampst(0.0, -Demora_Cola_1);
	printf("\nDemora media en cola Piezas 1    : %f minutos\n ", transfer[1]);
	printf("\nDemora maxima en cola Piezas 1    : %f minutos\n ", transfer[3]);

	sampst(0.0, -Demora_Cola_2);
	printf("\nDemora media en cola Piezas 2    : %f minutos\n ", transfer[1]);
	printf("\nDemora maxima en cola Piezas 2    : %f minutos\n ", transfer[3]);

	sampst(0.0, -Demora_Cola_3);
	printf("\nDemora media en cola Piezas 3    : %f minutos\n ", transfer[1]);
	printf("\nDemora maxima en cola Piezas 3    : %f minutos\n ", transfer[3]);

	sampst(0.0, -Demora_Cola_4);
	printf("\nDemora media en cola Piezas 4    : %f minutos\n ", transfer[1]);
	printf("\nDemora maxima en cola Piezas 4    : %f minutos\n ", transfer[3]);

	filest(Cola_Piezas_1);
	printf("\nNumero medio cola 1            : %f \n ", transfer[1]);
	filest(Cola_Piezas_2);
	printf("\nNumero medio cola 2           : %f \n ", transfer[1]);
	filest(Cola_Piezas_3);
	printf("\nNumero medio cola 3           : %f \n ", transfer[1]);
	filest(Cola_Piezas_4);
	printf("\nNumero medio cola 4           : %f \n ", transfer[1]);
	filest(Montacargas);
	printf("\nNumero medio Montacargas          : %f \n ", transfer[1]);

	timest(0.0, -Porcentaje_Repite_Circuito);
	printf("\nPorcentaje de veces que se repite el circuito antes de descargar         : %f \n ", transfer[1]);

	/*
	sampst(0.0, -Demora_B_Limpieza);
	printf("\nDemora media en cola limpieza, pieza B      : %f Horas\n ", transfer[1]/60);

	printf("\n No se si son tan locos esos numeros, ya que si pensas el ejercicio, cada 2 minutos llegan piezas,\n y el servidor nunca se libera, asi");
	printf("que el tiempo de demora en cola simepre va a ir aumentenado.. \n Pero igual no se..\n");
	
	sampst(0.0, -Demora_A_Lubricacion);
	printf("\nDemora media en cola Lubricacion, pieza A      : %f min \n ", transfer[1]);

	sampst(0.0, -Demora_B_Control_Hum);
	printf("\nDemora media en cola Lubricacion, pieza A      : %f min \n ", transfer[1]);

	printf("\n No me gustan los valores que me da.. Me parece raro que la espera del lado de A sea menor \n que la del de B ");
	printf(" porque supuestamente del lado de B lo controla un humano y tarda mas.. :/ \n\n");

	/*sampst(0.0, -Demora_Procesamiento_General);
	printf("\nTiempo medio permanencia de una pieza en el sistema      : %f hs \n ", transfer[1]/60);*/

	/*printf("\nTiempo medio permanencia de una pieza en el sistema      : %f \n ", 21600/Total_piezas_atendidas);

	printf("\nCantidad de piezas B rechazadas      :  %i  \n", B_Rechazadas);
	
	printf("\nProductos terminados en una jornada de 8 horas      :  %i  \n", Productos_8_hs);
	
	/*sampst(0.0, -Demora_Cola_Estacion_1);
	printf("\nDemora media en cola estacion 1      : %f \n ", transfer[1]);
	
	sampst(0.0, -Demora_Cola_Estacion_2);
	printf("\nDemora media en cola estacion 2      : %f \n ", transfer[1]);
	
	sampst(0.0, -Demora_Cola_Estacion_3);
	printf("\nDemora media en cola estacion 3      : %f \n ", transfer[1]);

	sampst(0.0, -Demora_Cola_Estacion_4);
	printf("\nDemora media en cola estacion 4      : %f \n ", transfer[1]);
	
	sampst(0.0, -Demora_Cola_Estacion_5);
	printf("\nDemora media en cola estacion 5      : %f \n ", transfer[1]);
	
	sampst(0.0, -Demora_Cola_Estacion_6);
	printf("\nDemora media en cola estacion 6      : %f \n ", transfer[1]);

	filest(Cola_Espera_Uso_Red);
	printf("\nCantidad media de pedidos en cola                    : %f \n ", transfer[1]);

	filest(Cola_Cant_Ped_Serv);
	printf("\nCantidad media de pedidos de servidores en cola      : %f \n ", transfer[1]);

	printf("\nCantidad de pedidos expulsados                       : %i \n ", cantidad_pedidos_expulsados);

	printf("\nCantidad de fallas generales                         : %i \n ", fallas);*/
	
}


