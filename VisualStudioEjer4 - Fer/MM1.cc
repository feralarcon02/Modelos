/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

/*Eventos*/
#define Inicio_Jornada_Laboral         1
#define Fin_Jornada_Laboral            2
#define Rotura_Maquina                 3
#define Fin_Reparacion                 4

/*Colas*/
#define Cola_Espera_Arreglo            1  

/*Servidores, continuar numeracion de las colas*/
#define Tecnicos_Reparadores           6
#define Reparador_Nocturno             7
#define Tecnicos_Rep_Aux               8

/*Demoras, otra numeracion distinta*/
#define Demora_Cola_Espera_Reparacion  1
#define Uso_Tecnico_1                  2


#define Media_Procesador_limpieza      1
#define Media_Procesamiento_Ensamblado 2


/* Declaraci¢n de variables propias */

float costo_total_sistema, demora_llegar_tecnico, media_rotura_maq, min_rep_rapida, max_rep_rapida, min_rep_lenta, max_rep_lenta;
int jornada_laboral, numero_tecnico;
bool trabajando, reparador_extra;


/* Declaraci¢n de Funciones propias */

void Rutina_Inicio_Jornada_Laboral(void);
void Rutina_Fin_Jornada_Laboral(void);
void Rutina_Rotura_Maquina(void);
void Rutina_Fin_Reparacion(void);
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


	while (sim_time <= 48000)
	{
		/* Determinar pr¢ximo Evento */
 		timing();

		/* Invoke the appropriate event function. */

		switch (next_event_type)
		{
			
		case Inicio_Jornada_Laboral:
			Rutina_Inicio_Jornada_Laboral();
			break;
		case Fin_Jornada_Laboral:
			Rutina_Fin_Jornada_Laboral();
			break;
		case Rotura_Maquina:
			Rutina_Rotura_Maquina();
			break;
		case Fin_Reparacion:
 			Rutina_Fin_Reparacion();
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
  
	media_rotura_maq = 480;
	min_rep_rapida = 20;
	max_rep_rapida = 40;
	min_rep_lenta = 90;
	max_rep_lenta = 150;
	demora_llegar_tecnico = 30;
	jornada_laboral = 720;
	trabajando = false;
	costo_total_sistema = 0;

	for (int i = 0; i < 6; i++) {
		transfer[1] = sim_time + expon(media_rotura_maq, Rotura_Maquina);
		transfer[2] = Rotura_Maquina;
		list_file(INCREASING, LIST_EVENT);
	}
	
	transfer[1] = sim_time + jornada_laboral;
	transfer[2] = Fin_Jornada_Laboral;
	trabajando = true;
	list_file(INCREASING, LIST_EVENT);
}

void Rutina_Inicio_Jornada_Laboral(void)
{

	transfer[1] = sim_time + jornada_laboral;
	transfer[2] = Fin_Jornada_Laboral;
	trabajando = false;

	list_file(INCREASING, LIST_EVENT);
	if (list_size[Cola_Espera_Arreglo] > 0) {
		list_remove(FIRST, Cola_Espera_Arreglo);

		numero_tecnico = transfer[3];
		float horas_reparacion = (sim_time - transfer[1]) / 60;
		costo_total_sistema = costo_total_sistema + (horas_reparacion * 120);

		float r = sim_time - transfer[1];
		sampst(sim_time - transfer[1], Demora_Cola_Espera_Reparacion);
		costo_total_sistema = costo_total_sistema + (((sim_time - transfer[1]) / 60) * 120);
		if (lcgrand(1) <= 0.3) {  /*Reparaicon rapida o lenta*/
			transfer[1] = sim_time + uniform(min_rep_rapida, max_rep_rapida, Fin_Reparacion);
		}
		else {
			transfer[1] = sim_time + uniform(min_rep_lenta, max_rep_lenta, Fin_Reparacion);
		}
		transfer[2] = Fin_Reparacion;
		transfer[3] = numero_tecnico;
		list_file(INCREASING, LIST_EVENT);
		transfer[1] = sim_time; /*Ocupo un reparador mas*/
		transfer[2] = Fin_Reparacion;
		transfer[3] = numero_tecnico;
		if (trabajando == true) {
			list_file(LAST, Tecnicos_Reparadores);
			costo_total_sistema = costo_total_sistema + (horas_reparacion * 40);
		}
		else {
			list_file(FIRST, Reparador_Nocturno);
			costo_total_sistema = costo_total_sistema + (horas_reparacion * 90);
		}
	}
}

void Rutina_Fin_Jornada_Laboral(void) {

	transfer[1] = sim_time + jornada_laboral;
	transfer[2] = Inicio_Jornada_Laboral;
	trabajando = false;
	list_file(INCREASING, LIST_EVENT);
}

void Rutina_Rotura_Maquina(void) {

	if (trabajando == true) { /*estan en horario de trabajo*/
		if ((list_size[Tecnicos_Reparadores] < 3) | (((list_size[Tecnicos_Reparadores] == 3) & (reparador_extra == false)) & (list_size[Cola_Espera_Arreglo] == 0))) {
			if (lcgrand(1) <= 0.3) {  /*Reparaicon rapida o lenta*/
				transfer[1] = sim_time + uniform(min_rep_rapida, max_rep_rapida, Fin_Reparacion);
			}
			else {
				transfer[1] = sim_time + uniform(min_rep_lenta, max_rep_lenta, Fin_Reparacion);
			}
			transfer[2] = Fin_Reparacion;
			transfer[3] = list_size[Tecnicos_Reparadores] + 1;
			list_file(INCREASING, LIST_EVENT);
			if (reparador_extra == false & list_size[Tecnicos_Reparadores] == 3) {
				reparador_extra = true;
			}
			transfer[1] = sim_time; /*Ocupo un reparador mas*/
			transfer[2] = Fin_Reparacion;
			transfer[3] = list_size[Tecnicos_Reparadores] + 1;
			list_file(LAST, Tecnicos_Reparadores);
			sampst(0, Demora_Cola_Espera_Reparacion);
		}
		else {
			transfer[1] = sim_time;
			list_file(LAST, Cola_Espera_Arreglo);
		}
	} /*No estan en horario de trabajo*/
	else {
		if (list_size[Reparador_Nocturno] < 1) {
			if (lcgrand(1) <= 0.3) {  /*Reparaicon rapida o lenta*/
				transfer[1] = sim_time + demora_llegar_tecnico + uniform(min_rep_rapida, max_rep_rapida, Fin_Reparacion);
			}
			else {
				transfer[1] = sim_time + demora_llegar_tecnico + uniform(min_rep_lenta, max_rep_lenta, Fin_Reparacion);
			}
			transfer[2] = Fin_Reparacion;
			transfer[3] = 4; /*Reparador nocturno, ver que hacer cuando finaliza..*/
			list_file(INCREASING, LIST_EVENT);
			transfer[1] = sim_time; /*Ocupo un reparador mas*/
			transfer[2] = Fin_Reparacion;
			list_file(LAST, Reparador_Nocturno);
		}
		else {
			transfer[1] = sim_time;
			list_file(LAST, Cola_Espera_Arreglo);
		}
	}
}


void Rutina_Fin_Reparacion(void) {

	numero_tecnico = transfer[3];
	float horas_reparacion = (sim_time - transfer[1])/60;
	costo_total_sistema = costo_total_sistema + (horas_reparacion * 120);

	while (list_size[Tecnicos_Reparadores] > 0) {
		list_remove(FIRST, Tecnicos_Reparadores);
		if (numero_tecnico != transfer[3]) {
			list_file(FIRST, Tecnicos_Rep_Aux);
		}
		else {
			if (numero_tecnico == 1) {
				sampst(sim_time - transfer[1], Uso_Tecnico_1);
			}
		}
	}
	while (list_size[Tecnicos_Rep_Aux] > 0){
		list_remove(FIRST, Tecnicos_Rep_Aux);
		list_file(FIRST, Tecnicos_Reparadores);
	}
	if (reparador_extra == true) {
		reparador_extra = false;
	}
	else {
		if (list_size[Cola_Espera_Arreglo] > 0) {
			list_remove(FIRST, Cola_Espera_Arreglo);
			float r = sim_time - transfer[1];
			sampst(sim_time - transfer[1], Demora_Cola_Espera_Reparacion);
			costo_total_sistema = costo_total_sistema + (((sim_time-transfer[1])/60) * 120);
			if (lcgrand(1) <= 0.3) {  /*Reparaicon rapida o lenta*/
				transfer[1] = sim_time + uniform(min_rep_rapida, max_rep_rapida, Fin_Reparacion);
			}
			else {
				transfer[1] = sim_time + uniform(min_rep_lenta, max_rep_lenta, Fin_Reparacion);
			}
			transfer[2] = Fin_Reparacion;
			transfer[3] = numero_tecnico;
			list_file(INCREASING, LIST_EVENT);
			transfer[1] = sim_time; /*Ocupo un reparador mas*/
			transfer[2] = Fin_Reparacion;
			transfer[3] = numero_tecnico;
			if (trabajando == true) {
				list_file(LAST, Tecnicos_Reparadores);
				costo_total_sistema = costo_total_sistema + (horas_reparacion * 40);
			}
			else {
				list_file(FIRST, Reparador_Nocturno);
				costo_total_sistema = costo_total_sistema + (horas_reparacion * 90);
			}
		}
	}
	transfer[1] = sim_time + expon(media_rotura_maq, Rotura_Maquina);
	transfer[2] = Rotura_Maquina;
	list_file(INCREASING, LIST_EVENT);
}



void reporte(void)  
{
	/*printf("\nCantidad de pedidos de estaciones    : %i \n ", cantidad_pedidos_estaciones);

	printf("\nCantidad de pedidos de servidores    : %i \n ", cantidad_pedidos_servidores);*/

	/*filest(Procesador_Limpieza);
	timest(0.0, -Media_Procesador_limpieza);
	printf("\nUtilizacion del Procesador Limpieza      : %f \n ", transfer[1]);

	float x = transfer[1];

	printf("\nUtilizacion del Procesador Lado A      : %f \n ", filest(Procesador_Lubic_A));   
	printf("\nUtilizacion del Procesador Lado B     : %f \n ", filest(Procesador_Control_Human_B));

	printf("\nUtilizacion del Procesador Ensamblado  (Esta mal, a ver si se dan cuenta porquee..)   : %f \n ", filest(Procesador_Ensamblado));

	timest(0.0, -Media_Procesamiento_Ensamblado);
	printf("\nUtilizacion Procesador Ensamblado mas preciso     : %f \n ", transfer[1]);*/
	
	sampst(0.0, -Uso_Tecnico_1);
	printf("\nMedia timepo trabajo tecnico 1    : %f hs \n ", transfer[1]/60);
	printf("\nMaximo timepo trabajo tecnico 1    : %f hs \n ", transfer[3]/60);

	sampst(0.0, -Demora_Cola_Espera_Reparacion);
	printf("\nDemora media en cola Espera reparacion    : %f hs \n ", transfer[1]/60);

	sampst(0.0, -Demora_Cola_Espera_Reparacion);
	printf("\nDemora minima en con=la Espera reparacion     : %f hs \n ", transfer[4]/60);

	sampst(0.0, -Demora_Cola_Espera_Reparacion);
	printf("\nDemora maxima en cola Espera reparacion     : %f hs \n ", transfer[3]/60);

	filest(Cola_Espera_Arreglo);
	printf("\nNumero medio de maquinas en la cola de espera     : %f  \n ", transfer[1]);


	printf("\nCosto total del sistema de reparacion    : %f $ \n ", costo_total_sistema);
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


