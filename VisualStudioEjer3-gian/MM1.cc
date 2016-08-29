/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

#define Arribo_Pieza_LimYRef           1
#define Finaliza_LimpiezaYRefinacion   2
#define Finaliza_LubricacionYRef       3
#define Finaliza_Operador_Humano       4
#define Fin_Ensamblado                 5

#define Cola_LimpiezaYRefinacion       1  
#define Servidor_LimpiezaYRefinacion   2
#define Cola_Espera_LimYRef            3
#define Servidor_LubYCal_A             4
#define Cola_Espera_Servidor_LubYCal_A 5
#define Servidor_Operador_Hum_B        6
#define Cola_Espera_Servidor_Operador_Hum_B 7
#define Servidor_Ensamblado            8
#define Cola_Ensamblado_A              9
#define Cola_Ensamblado_B              10



#define Demora_Cola_LimyRef_A          1
#define Demora_Cola_LimyRef_B          2
#define Demora_Cola_LubyCal_A          3
#define Demora_Cola_Oper_Hum_B         4
#define Demora_Ensasmblador_A          5
#define Demora_Ensamblador_B           6
#define Demora_general                 7


/* Declaraci¢n de variables propias */

float Min_arribo_pieza, Media_Procesamiento_Ensamblado, Max_arribo_pieza, Tiempo_LimpiezaYRefinacion, Tiempo_LubyCal_A, Tiempo_Control_Humano, Tiempo_Ensambladora;
int dfdF;
bool efef;


/* Declaraci¢n de Funciones propias */

void Rutina_Arribo_Pieza_LimYRef(void);
void Rutina_Finaliza_LimpiezaYRefinacion(void);
void Rutina_Finaliza_Operador_Humano(void);
void Rutina_Finaliza_LubricacionYRef(void);
void Rutina_Fin_Ensamblado(void);
void inicializa(void);
void reporte(void);


int main()  /* Main function. */
{
	/* Apertura de Archivos que sean necesarios */
	
	/*cantidad_pedidos_expulsados = 0;
	fallas = 0;
	cantidad_pedidos_estaciones = 0;
	cantidad_pedidos_servidores = 0;*/


	/* Initializar Simlib */
	init_simlib();


	/* Establecer maxatr = M ximo n£mero de Atributos utilizados (Numero máximo que voy a utilizar del transfer)  */
	maxatr = 7;

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */


	while (sim_time <= 86400)
	{
		/* Determinar pr¢ximo Evento */
 		timing();

		/* Invoke the appropriate event function. */
		switch (next_event_type)
		{
			
		case Arribo_Pieza_LimYRef:
			Rutina_Arribo_Pieza_LimYRef();
			break;
		case Finaliza_LimpiezaYRefinacion:
			Rutina_Finaliza_LimpiezaYRefinacion();
			break;
		case Finaliza_LubricacionYRef:
			Rutina_Finaliza_LubricacionYRef();
			break;
		case Finaliza_Operador_Humano:
			Rutina_Finaliza_Operador_Humano();
			break;
		case Fin_Ensamblado:
			Rutina_Fin_Ensamblado();
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
	Min_arribo_pieza = 1;
	Max_arribo_pieza = 2.5;
	Tiempo_LimpiezaYRefinacion = 2.8;
	Tiempo_LubyCal_A = 3.5;
	Tiempo_Control_Humano = 3.5;
	Tiempo_Ensambladora = 3.1;
	Media_Procesamiento_Ensamblado = 0;


	transfer[1] = sim_time + uniform(Min_arribo_pieza, Max_arribo_pieza, Arribo_Pieza_LimYRef);
	transfer[2] = Arribo_Pieza_LimYRef;
	transfer[4] = sim_time;
	list_file(INCREASING, LIST_EVENT);

}

void Rutina_Arribo_Pieza_LimYRef(void)
{
	transfer[1] = sim_time + uniform(Min_arribo_pieza, Max_arribo_pieza, Arribo_Pieza_LimYRef);
	transfer[2] = Arribo_Pieza_LimYRef;
	transfer[4] = sim_time;
	list_file(INCREASING, LIST_EVENT);

	float AoB = lcgrand(1);
	if (AoB <= 0.5) {
		transfer[3] = 1;
	}
	else {
		transfer[3] = 2;
	}
	if (list_size[Servidor_LimpiezaYRefinacion] == 0) {
		if (transfer[3] == 1) {
			sampst(0, Demora_Cola_LimyRef_A);
		}
		else {
			sampst(0, Demora_Cola_LimyRef_B);
		}
		transfer[1] = sim_time + Tiempo_LimpiezaYRefinacion;
		transfer[2] = Finaliza_LimpiezaYRefinacion;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Servidor_LimpiezaYRefinacion);
	}
	else {
		transfer[1] = sim_time;
		transfer[2] = Finaliza_LimpiezaYRefinacion;
		list_file(LAST, Cola_Espera_LimYRef);
	}
}


void Rutina_Finaliza_LimpiezaYRefinacion(void) {

	if (transfer[3] == 1) {
		if (list_size[Servidor_LubYCal_A] == 0) {
			transfer[1] = sim_time + expon(Tiempo_LubyCal_A, Finaliza_LubricacionYRef);
			transfer[2] = Finaliza_LubricacionYRef;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Servidor_LubYCal_A);
			sampst(0, Demora_Cola_LubyCal_A);
		}
		else {
			transfer[1] = sim_time;
			transfer[2] = Finaliza_LubricacionYRef;
			list_file(LAST, Cola_Espera_Servidor_LubYCal_A);
		}
	}
	else {
		if (list_size[Servidor_Operador_Hum_B] == 0) {
			transfer[1] = sim_time + uniform(3 , 5 , Finaliza_Operador_Humano);
			transfer[2] = Finaliza_Operador_Humano;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Servidor_Operador_Hum_B);
			sampst(0, Demora_Cola_Oper_Hum_B);
		}
		else {
			transfer[1] = sim_time;
			transfer[2] = Finaliza_Operador_Humano;
			list_file(LAST, Cola_Espera_Servidor_Operador_Hum_B);
		}
	}
	list_remove(FIRST, Servidor_LimpiezaYRefinacion);
	if (list_size[Cola_Espera_LimYRef] > 0) {
		list_remove(FIRST, Cola_Espera_LimYRef);
		if (transfer[3] == 1) {
			sampst(sim_time - transfer[1], Demora_Cola_LimyRef_A);
		}
		else {
			sampst(sim_time - transfer[1], Demora_Cola_LimyRef_B);
		}
		transfer[1] = sim_time + +Tiempo_LimpiezaYRefinacion;
		transfer[2] = Finaliza_LimpiezaYRefinacion;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Servidor_LimpiezaYRefinacion);
	}
}

void Rutina_Finaliza_LubricacionYRef(void){

	list_remove(FIRST, Servidor_LubYCal_A);

	if (list_size[Servidor_Ensamblado] == 0) {
		transfer[1] = sim_time;
		transfer[2] = 1;
		list_file(FIRST, Servidor_Ensamblado);
		timest(0, Media_Procesamiento_Ensamblado);
		sampst(0, Demora_Ensasmblador_A);
	}
	else if (list_size[Servidor_Ensamblado] == 1) {
		list_remove(FIRST, Servidor_Ensamblado);
		int x = transfer[2];
		list_file(FIRST, Servidor_Ensamblado);
		if (x == 1) {
			transfer[1] = sim_time;
			transfer[2] = Fin_Ensamblado;
			transfer[3] = 1;
			list_file(LAST, Cola_Ensamblado_A);
			timest(0, Media_Procesamiento_Ensamblado);
		}
		else {
			transfer[1] = sim_time;
			transfer[2] = 1;
			list_file(LAST, Servidor_Ensamblado);
			sampst(0, Demora_Ensasmblador_A);
		}
	}
	else {
		transfer[1] = sim_time;
		transfer[2] = Fin_Ensamblado;
		transfer[3] = 1;
		list_file(LAST, Cola_Ensamblado_A);
	}
	if (list_size[Servidor_Ensamblado] == 2) {
		transfer[1] = sim_time + expon(Tiempo_Ensambladora, Fin_Ensamblado);
		transfer[2] = Fin_Ensamblado;
		timest(1, Media_Procesamiento_Ensamblado);
		list_file(INCREASING, LIST_EVENT);
	}
	if (list_size[Cola_Espera_Servidor_LubYCal_A] > 0){
		list_remove(FIRST, Cola_Espera_Servidor_LubYCal_A);
		sampst(sim_time - transfer[1], Demora_Cola_LubyCal_A);
		transfer[1] = sim_time + expon(Tiempo_LubyCal_A, Finaliza_LubricacionYRef);
		transfer[2] = Finaliza_LubricacionYRef;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Servidor_LubYCal_A);
	}
}

void Rutina_Finaliza_Operador_Humano(void) {

	list_remove(FIRST, Servidor_Operador_Hum_B);
	float prob_falla = lcgrand(1);
	if (prob_falla > 0.05) {
		if (list_size[Servidor_Ensamblado] == 0) {
			transfer[1] = sim_time;
			transfer[2] = 2;
			list_file(FIRST, Servidor_Ensamblado);
			timest(0, Media_Procesamiento_Ensamblado);
			sampst(0, Demora_Ensamblador_B);
		}
		else if (list_size[Servidor_Ensamblado] == 1) {
			list_remove(FIRST, Servidor_Ensamblado);
			int x = transfer[2];
			list_file(FIRST, Servidor_Ensamblado);
			if (x == 2) {
				transfer[1] = sim_time;
				transfer[2] = Fin_Ensamblado;
				transfer[3] = 2;
				list_file(LAST, Cola_Ensamblado_B);
				timest(0, Media_Procesamiento_Ensamblado);
			}
			else {
				transfer[1] = sim_time;
				transfer[2] = 2;
				list_file(LAST, Servidor_Ensamblado);
				sampst(0, Demora_Ensamblador_B);
			}
		}
		else {
			transfer[1] = sim_time;
			transfer[2] = Fin_Ensamblado;
			transfer[3] = 2;
			list_file(LAST, Cola_Ensamblado_B);
		}
		if (list_size[Servidor_Ensamblado] == 2) {
			transfer[1] = sim_time + expon(Tiempo_Ensambladora, Fin_Ensamblado);
			transfer[2] = Fin_Ensamblado;
			timest(1, Media_Procesamiento_Ensamblado);
			list_file(INCREASING, LIST_EVENT);
		}
	}
	else {
		if (list_size[Servidor_LimpiezaYRefinacion] == 0) {
			transfer[1] = sim_time + Tiempo_LimpiezaYRefinacion;
			transfer[2] = Finaliza_LimpiezaYRefinacion;
			transfer[3] = 2;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Servidor_LimpiezaYRefinacion);
			sampst(0, Demora_Cola_LimyRef_B);
		}
		else {
			transfer[1] = sim_time;
			transfer[2] = Finaliza_LimpiezaYRefinacion;
			list_file(LAST, Cola_Espera_LimYRef);
		}
	}
	if (list_size[Cola_Espera_Servidor_Operador_Hum_B] > 0) {
		list_remove(FIRST, Cola_Espera_Servidor_Operador_Hum_B);
		sampst(sim_time - transfer[1], Demora_Cola_Oper_Hum_B);
		transfer[1] = sim_time + uniform(3, 5, Finaliza_Operador_Humano);
		transfer[2] = Finaliza_Operador_Humano;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Servidor_Operador_Hum_B);
	}

}
void Rutina_Fin_Ensamblado(void) {

	timest(0, Media_Procesamiento_Ensamblado);

	float prob_falla_2 = lcgrand(1);
	if (prob_falla_2 <= 0.04) {
		for (int i = 0; i <= 2; i++) {
			if (list_size[Servidor_LimpiezaYRefinacion] == 0) {
				transfer[1] = sim_time + Tiempo_LimpiezaYRefinacion;
				transfer[2] = Finaliza_LimpiezaYRefinacion;
				list_file(INCREASING, LIST_EVENT);
				list_file(FIRST, Servidor_LimpiezaYRefinacion);
			}
			else {
				transfer[1] = sim_time;
				transfer[2] = Finaliza_LimpiezaYRefinacion;
				list_file(LAST, Cola_Espera_LimYRef);
			}
		}
	}
	while (list_size[Servidor_Ensamblado] > 0) {
		list_remove(FIRST, Servidor_Ensamblado);
		sampst(sim_time - transfer[4], Demora_general);
	}
	if (list_size[Cola_Ensamblado_A] > 0) {
		list_remove(FIRST, Cola_Ensamblado_A);
		sampst(sim_time - transfer[1], Demora_Ensasmblador_A);
		transfer[1] = sim_time;
		transfer[2] = 1;
		list_file(FIRST, Servidor_Ensamblado);
	}
	if (list_size[Cola_Ensamblado_B] > 0) {
		list_remove(FIRST, Cola_Ensamblado_B);
		sampst(sim_time - transfer[1], Demora_Ensamblador_B);
		transfer[1] = sim_time;
		transfer[2] = 2;
		list_file(LAST, Servidor_Ensamblado);
	}
	if (list_size[Servidor_Ensamblado] == 2) {
		transfer[1] = sim_time + expon(Tiempo_Ensambladora, Fin_Ensamblado);
		transfer[2] = Fin_Ensamblado;
		list_file(INCREASING, LIST_EVENT);
		timest(0, Media_Procesamiento_Ensamblado);
	}
}

void reporte(void)  
{

	filest(Servidor_LimpiezaYRefinacion);
	float r = transfer[1];
	printf("\nUtilizacion servidor Limpieza y Refinacion       : %f %%% \n ", (transfer[1] * 100));

	filest(Servidor_LubYCal_A);
	printf("\nUtilizacion servidor Lubricacion y Refinacion    : %f %%% \n ", (transfer[1] * 100));

	filest(Servidor_Operador_Hum_B);
	printf("\nUtilizacion servidor Operador Humano             : %f %%% \n ", (transfer[1] * 100));

	timest(0.0, -Media_Procesamiento_Ensamblado);
	printf("\nUtilizacion Procesador Ensamblado                : %f %%% \n ", (transfer[1]) * 100);

	sampst(0.0, -Demora_Cola_LimyRef_A);
	printf("\nDemora media en cola limpieza, pieza A     : %.2f min \n ", transfer[1] );

	sampst(0.0, -Demora_Cola_LimyRef_B);
	printf("\nDemora media en cola limpieza, pieza B     : %f min \n ", transfer[1]);

	sampst(0.0, -Demora_Cola_LubyCal_A);
	printf("\nDemora media en cola de Lub. y Cal.        : %f min \n ", transfer[1]);

	sampst(0.0, -Demora_Cola_Oper_Hum_B);
	printf("\nDemora media en cola de oper. Humano       : %f min \n ", transfer[1]);

	sampst(0.0, -Demora_Ensasmblador_A);
	printf("\nDemora media en cola del ensambladorA      : %f min \n ", transfer[1]);

	sampst(0.0, -Demora_Ensamblador_B);
	printf("\nDemora media en cola del ensambladorB      : %f min \n ", transfer[1]);

	sampst(0.0, -Demora_general);
	printf("\nDemora media de las piesas e todo el proc. : %f min \n ", transfer[1]);



	
	/*printf("\nCantidad de pedidos de servidores    : %i \n ", cantidad_pedidos_servidores);

	filest(Red);
	printf("\nTiempo de utilizacion de la red      : %f %%%\n ", (transfer[1]/3 * 100));
 
	
	sampst(0.0, -Demora_Cola_Estacion_1);
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


