/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

/*Eventos*/
#define Arribo_Coche_Lado_A            1
#define Arribo_Coche_Lado_B            2
#define Fin_Atencion_Coche_A           3
#define Fin_Atencion_Coche_B           4
#define Fin_Cabina_Extra_A             5
#define Fin_Cabina_Extra_B             6

#define Inicio_Jornada_Trancitada      7
#define Fin_Jornada_Trancitada         8

#define Liberar_Transito               9


/*Colas*/
#define Cola_Cabina_1_A                1  
#define Cola_Cabina_2_A                2  
#define Cola_Cabina_1_B                3
#define Cola_Cabina_2_B                4

/*Servidores, continuar numeracion de las colas*/
#define Cabina_1_A                     5
#define Cabina_2_A                     6
#define Cabina_1_B                     7
#define Cabina_2_B                     8

/*Demoras, otra numeracion distinta*/
#define Demora_Cola_1_A                1
#define Demora_Cola_2_A                2
#define Demora_Cola_1_B                3
#define Demora_Cola_2_B                4



/* Declaraci¢n de variables propias */

float media_arribo_mucho_transito, media_arribo_poco_transito;
int vehiculos_no_pagan, cant_libera_cabina, tiempo_jornada_trancitada, tiempo_jornada_no_trancitada, tiempo_simulacion;
bool cabinas_extra, jornada_trancitada, cabina_extra_A, cabina_extra_B;


/* Declaraci¢n de Funciones propias */

void Rutina_Arribo_Coche_Lado_A(void);
void Rutina_Arribo_Coche_Lado_B(void);
void Rutina_Fin_Atencion_Coche_A(void);
void Rutina_Fin_Atencion_Coche_B(void);
void Rutina_Fin_Cabina_Extra_A(void);
void Rutina_Fin_Cabina_Extra_B(void);
void Rutina_Inicio_Jornada_Trancitada(void);
void Rutina_Fin_Jornada_Trancitada(void);
void Rutina_Liberar_Transito(void);
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


	while (sim_time <= tiempo_simulacion)
	{
		/* Determinar pr¢ximo Evento */
 		timing();

		/* Invoke the appropriate event function. */

		switch (next_event_type)
		{
			
		case Arribo_Coche_Lado_A:
			Rutina_Arribo_Coche_Lado_A();
			break;
		case Arribo_Coche_Lado_B:
			Rutina_Arribo_Coche_Lado_B();
			break;
		case Fin_Atencion_Coche_A:
			Rutina_Fin_Atencion_Coche_A();
			break;
		case Fin_Atencion_Coche_B:
			Rutina_Fin_Atencion_Coche_B();
			break;
		case Inicio_Jornada_Trancitada:
			Rutina_Inicio_Jornada_Trancitada();
			break;
		case Fin_Jornada_Trancitada:
			Rutina_Fin_Jornada_Trancitada();
			break;
		case Fin_Cabina_Extra_A:
			Rutina_Fin_Cabina_Extra_A();
			break;
		case Fin_Cabina_Extra_B:
			Rutina_Fin_Cabina_Extra_B();
			break;
		case Liberar_Transito:
			Rutina_Liberar_Transito();
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
	tiempo_simulacion = 10080;
	tiempo_jornada_trancitada = 600;
	tiempo_jornada_no_trancitada = 840;
	jornada_trancitada = false; /*Porque arranca a las 00:00hs*/
	media_arribo_mucho_transito = 0.5;
	media_arribo_poco_transito = 1.2;



	/* Se carga el primer Arribo en la Lista de Eventos */

	transfer[1] = sim_time + 420;  /*Porque arranca a las 00:00hs*/
	transfer[2] = Inicio_Jornada_Trancitada;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_arribo_poco_transito,Arribo_Coche_Lado_A);
	transfer[2] = Arribo_Coche_Lado_A;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_arribo_poco_transito, Arribo_Coche_Lado_B);
	transfer[2] = Arribo_Coche_Lado_B;
	list_file(INCREASING, LIST_EVENT);

}

void Rutina_Inicio_Jornada_Trancitada(void){

	transfer[1] = sim_time + tiempo_jornada_trancitada;
	transfer[2] = Fin_Jornada_Trancitada;
	list_file(INCREASING, LIST_EVENT);
	jornada_trancitada = true;
	
}

void Rutina_Fin_Jornada_Trancitada(void) {

	transfer[1] = sim_time + tiempo_jornada_no_trancitada;
	transfer[2] = Inicio_Jornada_Trancitada;
	list_file(INCREASING, LIST_EVENT);
	jornada_trancitada = false;
}

void Rutina_Arribo_Coche_Lado_A(void) {


	if (jornada_trancitada == false) {
		if ((list_size[Cabina_1_A] == 0) & (list_size[Cola_Cabina_1_A] == 0)) {
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_A);
			transfer[2] = Fin_Atencion_Coche_A;
			transfer[3] = Cabina_1_A;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Cabina_1_A);  /*Ocupo esa cabina*/
		}
		else {
			if (list_size[Cola_Cabina_1_A] > 15) {   /*---Todo lo que pasa si llego a 15 y necesito habilitar la otra---- vvvvv*/
				if (cabina_extra_A == false) {
					transfer[1] = sim_time + 30;
					transfer[2] = Fin_Cabina_Extra_A;
					transfer[3] = Cabina_1_A;
					list_file(INCREASING, LIST_EVENT);
					cabina_extra_A = true;
				}
				if (list_size[Cabina_2_A] == 0) {
					transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_A);
					transfer[2] = Fin_Atencion_Coche_A;
					transfer[3] = Cabina_2_A;
					list_file(INCREASING, LIST_EVENT);
					list_file(FIRST, Cabina_2_A);  /*Ocupo esa cabina*/
				}
				else {
					transfer[1] = sim_time;
					transfer[2] = Cabina_2_A;
					list_file(LAST, Cola_Cabina_2_A);
				}                              /*---Todo lo que pasa si llego a 15 y necesito habilitar la otra---- ^^^^^*/
			}
			else {
				transfer[1] = sim_time;
				transfer[2] = Cabina_1_A;
				list_file(LAST, Cola_Cabina_1_A);
			}
			
		}
		transfer[1] = sim_time + expon(media_arribo_poco_transito, Arribo_Coche_Lado_A);
		transfer[2] = Arribo_Coche_Lado_A;
		list_file(INCREASING, LIST_EVENT);
	}
	else{
		if ((list_size[Cabina_1_A] == 1) && (list_size[Cabina_2_A] == 1)) {  /*Ambas cabinas estan ocupadas, pongo en la de menor cola*/
			if (list_size[Cola_Cabina_1_A] < list_size[Cola_Cabina_2_A]) {
				transfer[1] = sim_time;
				transfer[2] = Cabina_1_A;
				list_file(LAST, Cola_Cabina_1_A);
			}
			else {
				transfer[1] = sim_time;
				transfer[2] = Cabina_2_A;
				list_file(LAST, Cola_Cabina_2_A);
			}
		}
		else if (list_size[Cabina_1_A] == 0) {   /*Hay una libre y miro cual es*/
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_A);
			transfer[2] = Fin_Atencion_Coche_A;
			transfer[3] = Cabina_1_A;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Cabina_1_A);  /*Ocupo esa cabina*/
		}
		else {
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_A);
			transfer[2] = Fin_Atencion_Coche_A;
			transfer[3] = Cabina_2_A;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Cabina_2_A);  /*Ocupo esa cabina*/
		}
		transfer[1] = sim_time + expon(media_arribo_mucho_transito, Arribo_Coche_Lado_A);
		transfer[2] = Arribo_Coche_Lado_A;
		list_file(INCREASING, LIST_EVENT);
	}
	
	
}

void Rutina_Arribo_Coche_Lado_B(void) {
	
	if (jornada_trancitada == false){
		if ((list_size[Cabina_1_B] == 0) & (list_size[Cola_Cabina_1_B] == 0)) {
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_B);
			transfer[2] = Fin_Atencion_Coche_B;
			transfer[3] = Cabina_1_B;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Cabina_1_B);  /*Ocupo esa cabina*/
		}
		else {
			if (list_size[Cola_Cabina_1_B] > 15) {   /*---Todo lo que pasa si llego a 15 y necesito habilitar la otra---- vvvvv*/
				if (cabina_extra_B == false) {
					transfer[1] = sim_time + 30;
					transfer[2] = Fin_Cabina_Extra_B;
					transfer[3] = Cabina_1_B;
					list_file(INCREASING, LIST_EVENT);
					cabina_extra_B = true;
				}
				if (list_size[Cabina_2_B] == 0) {
					transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_B);
					transfer[2] = Fin_Atencion_Coche_B;
					transfer[3] = Cabina_2_B;
					list_file(INCREASING, LIST_EVENT);
					list_file(FIRST, Cabina_2_B);  /*Ocupo esa cabina*/
				}
				else {
					transfer[1] = sim_time;
					transfer[2] = Cabina_2_B;
					list_file(LAST, Cola_Cabina_2_B);
				}                              /*---Todo lo que pasa si llego a 15 y necesito habilitar la otra---- ^^^^^*/
			}
			transfer[1] = sim_time;
			transfer[2] = Cabina_1_B;
			list_file(LAST, Cola_Cabina_1_B);
		}
		transfer[1] = sim_time + expon(media_arribo_poco_transito, Arribo_Coche_Lado_B);
		transfer[2] = Arribo_Coche_Lado_B;
		list_file(INCREASING, LIST_EVENT);
	}
	else {
		if ((list_size[Cabina_1_B] == 1) && (list_size[Cabina_2_B] == 1)) {  /*Ambas cabinas estan ocupadas, pongo en la de menor cola*/
			if (list_size[Cola_Cabina_1_B] < list_size[Cola_Cabina_2_B]) {
				transfer[1] = sim_time;
				transfer[2] = Cabina_1_B;
				list_file(LAST, Cola_Cabina_1_B);
			}
			else {
				transfer[1] = sim_time;
				transfer[2] = Cabina_2_B;
				list_file(LAST, Cola_Cabina_2_B);
			}
		}
		else if (list_size[Cabina_1_B] == 0) {   /*Hay una libre y miro cual es*/
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_B);
			transfer[2] = Fin_Atencion_Coche_B;
			transfer[3] = Cabina_1_B;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Cabina_1_B);  /*Ocupo esa cabina*/
		}
		else {
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_B);
			transfer[2] = Fin_Atencion_Coche_B;
			transfer[3] = Cabina_2_B;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Cabina_2_B);  /*Ocupo esa cabina*/
		}
		transfer[1] = sim_time + expon(media_arribo_mucho_transito, Arribo_Coche_Lado_B);
		transfer[2] = Arribo_Coche_Lado_B;
		list_file(INCREASING, LIST_EVENT);
	}
	
	
}

void Rutina_Fin_Atencion_Coche_A(void) {

	/*Cuando cambia el horario y quedan autos en las cabinas que tienen quedesactivarse, priemro las atienden*/
	if ((list_size[Cola_Cabina_1_A] > 20) && (list_size[Cola_Cabina_1_B] > 20) && (list_size[Cola_Cabina_2_A] > 20) && (list_size[Cola_Cabina_2_B] > 20)) {
		if (lcgrand(1) >= 0.5) {
			transfer[1] = sim_time + 10;
			transfer[2] = Liberar_Transito;
			list_file(INCREASING, LIST_EVENT);
		}
	}

	if (transfer[3] == 5) {
		if (list_size[Cola_Cabina_1_A] > 0) {
			list_remove(FIRST, Cola_Cabina_1_A);
			sampst(sim_time - transfer[1], Demora_Cola_1_A);
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_A);
			transfer[2] = Fin_Atencion_Coche_A;
			transfer[3] = 5;
			list_file(INCREASING, LIST_EVENT);
			 /*Ocupo esa cabina*/
		}
		else{
			list_remove(FIRST, Cabina_1_A);
		}
	}
	else {
		
		if (list_size[Cola_Cabina_2_A] > 0) {
			list_remove(FIRST, Cola_Cabina_2_A);
			sampst(sim_time - transfer[1], Demora_Cola_2_A);
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_A);
			transfer[2] = Fin_Atencion_Coche_A;
			transfer[3] = 6;
			list_file(INCREASING, LIST_EVENT);
		   /*Ocupo esa cabina*/
		}
		else {
			list_remove(FIRST, Cabina_2_A);
		}
	}
	
}

void Rutina_Fin_Atencion_Coche_B(void) {

	if (((list_size[Cola_Cabina_1_A] > 20) & (list_size[Cola_Cabina_1_A] > 20)) & ((list_size[Cola_Cabina_1_A] > 20) & (list_size[Cola_Cabina_1_A] > 20))) {
		if (lcgrand(1) >= 0.5) {
			transfer[1] = sim_time + 10;
			transfer[2] = Liberar_Transito;
			list_file(INCREASING, LIST_EVENT);
		}
	}

	if (transfer[3] == 7) {
		if (list_size[Cola_Cabina_1_B] > 0) {
			list_remove(FIRST, Cola_Cabina_1_B);
			sampst(sim_time - transfer[1], Demora_Cola_1_B);
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_B);
			transfer[2] = Fin_Atencion_Coche_B;
			transfer[3] = Cabina_1_B;
			list_file(INCREASING, LIST_EVENT);  /*Ocupo esa cabina*/
		}
		else {
			list_remove(FIRST, Cabina_1_B);
		}
	}
	else {
		if (list_size[Cola_Cabina_2_B] > 0) {
			list_remove(FIRST, Cola_Cabina_2_B);
			sampst(sim_time - transfer[1], Demora_Cola_2_B);
			transfer[1] = sim_time + uniform(0.5, 1, Fin_Atencion_Coche_B);
			transfer[2] = Fin_Atencion_Coche_B;
			transfer[3] = Cabina_2_B;
			list_file(INCREASING, LIST_EVENT);
			  /*Ocupo esa cabina*/
		}
		else {
			list_remove(FIRST, Cabina_2_B);
		}
	}
}

void Rutina_Fin_Cabina_Extra_A(void) {
	cabina_extra_A = false;
}

void Rutina_Fin_Cabina_Extra_B(void) {
	cabina_extra_B = false;
}

void Rutina_Liberar_Transito(void) {
	
	cant_libera_cabina++;
	while (list_size[Cola_Cabina_1_A] > 0)
	{
		list_remove(FIRST, Cola_Cabina_1_A);
		event_cancel(Fin_Atencion_Coche_A);
		vehiculos_no_pagan++;
	}
	while (list_size[Cola_Cabina_2_A] > 0)
	{
		list_remove(FIRST, Cola_Cabina_2_A);
		event_cancel(Fin_Atencion_Coche_A);
		vehiculos_no_pagan++;
	}
	while (list_size[Cola_Cabina_1_B] > 0)
	{
		list_remove(FIRST, Cola_Cabina_1_B);
		event_cancel(Fin_Atencion_Coche_B);
		vehiculos_no_pagan++;
	}
	while (list_size[Cola_Cabina_2_B] > 0)
	{
		list_remove(FIRST, Cola_Cabina_2_B);
		event_cancel(Fin_Atencion_Coche_B);
		vehiculos_no_pagan++;
	}
	
}

void reporte(void)  
{
	sampst(0.0, -Demora_Cola_1_A);
	printf("\nDemora media en cola, Cabina 1 A: %.2f minutos\n ", transfer[1]);

	sampst(0.0, -Demora_Cola_2_A);
	printf("\nDemora media en cola, Cabina 2 A: %.2f minutos\n ", transfer[1]);

	sampst(0.0, -Demora_Cola_1_B);
	printf("\nDemora media en cola, Cabina 1 B: %.2f minutos\n ", transfer[1]);

	sampst(0.0, -Demora_Cola_2_B);
	printf("\nDemora media en cola, Cabina 2 B: %.2f minutos\n ", transfer[1]);

	filest(Cola_Cabina_1_A);
	printf("\nNumero medio en cola cabina 1 A    : %.2f \n ", transfer[1]);

	filest(Cola_Cabina_2_A);
	printf("\nNumero medio en cola cabina 2 A    : %.2f \n ", transfer[1]);

	filest(Cola_Cabina_1_B);
	printf("\nNumero medio en cola cabina 1 B    : %.2f \n ", transfer[1]);

	filest(Cola_Cabina_2_B);
	printf("\nNumero medio en cola cabina 2 B    : %.2f \n ", transfer[1]);

	filest(Cabina_1_A);
	printf("\nUtilizacion Cabina 1 A    : %.2f  ", transfer[1]);
	filest(Cabina_2_A);
	printf("\nUtilizacion Cabina 2 A    : %.2f  ", transfer[1]);
	filest(Cabina_1_B);
	printf("\nUtilizacion Cabina 1 B    : %.2f  ", transfer[1]);
	filest(Cabina_2_B);
	printf("\nUtilizacion Cabina 2 B    : %.2f \n ", transfer[1]);

	printf("\nCantidada de vehiculos que pasan sin pagar    : %i \n ", vehiculos_no_pagan);

	printf("\nCantidadd de veces que se liberan las cabinas   : %i \n ", cant_libera_cabina);

	/*filest(Uso_Maquinaria);
	printf("\nUtilizacion maquinaria      : %.2f %%%\n ", transfer[1] * 100);

	filest(CamionA);
	printf("\nCantidad media de camiones chicos en cola     : %.2f \n ", transfer[1]);

	filest(CamionB);
	printf("\nCantidad media de camiones grandes en cola     : %.2f \n ", transfer[1]);

	sampst(0.0, -DemoraColaA);
	printf("\nDemora en cola camiones chicos     : %.2f hs\n ", transfer[1]/60);

	sampst(0.0, -DemoraColaB);
	printf("\nDemora en cola camiones grandes    : %.2f hs\n ", transfer[1]/60);

	
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


