/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

#define Arribo_Limpieza_Refinacion     1
#define Finaliza_Limpieza_Refinacion   2
#define Fin_Lub_Calib_A                3
#define Fin_Control_Humano_B           4
#define Fin_Ensamblado                 5

#define Cola_Espera_Limpieza_Ref       1  
#define Cola_Espera_Lub_A              2
#define Cola_Espera_Control_Humano_B   3
#define Cola_Ensamblado_A              4
#define Cola_Ensamblado_B              5

#define Procesador_Limpieza            6
#define Procesador_Lubic_A             7
#define Procesador_Control_Human_B     8
#define Procesador_Ensamblado          9

#define Pieza_A                        1
#define Pieza_B                        2
#define Pieza_D                        3

#define Demora_Cola_Estacion_1         1
#define Demora_Cola_Estacion_2         2
#define Demora_Cola_Estacion_3         3
#define Demora_Cola_Estacion_4         4
#define Demora_Cola_Estacion_5         5
#define Demora_Cola_Estacion_6         6

#define Media_Procesador_limpieza      1


/* Declaraci¢n de variables propias */

float  min_arribo_limp_ref, max_arribo_limp_ref, tiempo_constante_limpieza, media_lub_calib_A, media_timepo_ensamblado;
int pieza, Proc_limpieza;
bool bandera;


/* Declaraci¢n de Funciones propias */

void Rutina_Arribo_Limpieza_Ref(void);
void Rutina_Fin_Limpieza_Ref(void);
void Rutina_Fin_Lub_Calib_A(void);
void Rutina_Fin_Control_Humano_B(void);
void Rutina_Fin_Ensamblado(void);
void inicializa(void);
void reporte(void);


int main()  /* Main function. */
{
	/* Apertura de Archivos que sean necesarios */


	/* Initializar Simlib */
	init_simlib();


	/* Establecer maxatr = M ximo n£mero de Atributos utilizados (Numero máximo que voy a utilizar del transfer)  */
	maxatr = 7;

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */


	while (sim_time <= 21600)
	{
		/* Determinar pr¢ximo Evento */
 		timing();

		/* Invoke the appropriate event function. */

		switch (next_event_type)
		{
			
		case Arribo_Limpieza_Refinacion:
			Rutina_Arribo_Limpieza_Ref();
			break;
		case Finaliza_Limpieza_Refinacion:
			Rutina_Fin_Limpieza_Ref();
			break;
		case Fin_Lub_Calib_A:
			Rutina_Fin_Lub_Calib_A();
			break;
		case Fin_Control_Humano_B:
 			Rutina_Fin_Control_Humano_B();
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
	min_arribo_limp_ref = 1;
	max_arribo_limp_ref = 2.5;
	tiempo_constante_limpieza = 2.8;
	media_lub_calib_A = 3.5;
	media_timepo_ensamblado = 3.1;
	Proc_limpieza = 0;

	transfer[1] = sim_time + uniform(min_arribo_limp_ref, max_arribo_limp_ref, Arribo_Limpieza_Refinacion);
	transfer[2] = Arribo_Limpieza_Refinacion;
	list_file(INCREASING, LIST_EVENT);

}

void Rutina_Arribo_Limpieza_Ref(void)
{
	transfer[1] = sim_time + uniform(min_arribo_limp_ref, max_arribo_limp_ref, Arribo_Limpieza_Refinacion);
	transfer[2] = Arribo_Limpieza_Refinacion;
	list_file(INCREASING, LIST_EVENT);

	if (lcgrand(1) >= 0.5) {
		transfer[3] = Pieza_A;
	}
	else {
		transfer[3] = Pieza_B;
	}

	if (list_size[Cola_Espera_Limpieza_Ref] < 0) {
		transfer[1] = sim_time;
		transfer[2] = Finaliza_Limpieza_Refinacion;
		list_file(LAST, Cola_Espera_Limpieza_Ref);
	}
	else {
		transfer[1] = sim_time + tiempo_constante_limpieza;
		transfer[2] = Finaliza_Limpieza_Refinacion;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Procesador_Limpieza);
		timest(1, Media_Procesador_limpieza);
	}
	
}

void Rutina_Fin_Limpieza_Ref(void) {

	if (transfer[3] == 1) {
		if (list_size[Procesador_Lubic_A] > 0) {
			transfer[1] = sim_time;
			transfer[2] = Fin_Lub_Calib_A;
			list_file(LAST, Cola_Espera_Limpieza_Ref);
		}
		else{
			transfer[1] = sim_time + expon(media_lub_calib_A, Fin_Lub_Calib_A);
			transfer[2] = Fin_Lub_Calib_A;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Procesador_Lubic_A);
		}
	}
	else {
		if (list_size[Procesador_Control_Human_B] > 0) {
			transfer[1] = sim_time;
			transfer[2] = Fin_Control_Humano_B;
			list_file(LAST, Cola_Espera_Control_Humano_B);
		}
		else {
			transfer[1] = sim_time + uniform(3, 5, Fin_Control_Humano_B);
			transfer[2] = Fin_Control_Humano_B;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Procesador_Control_Human_B);
		}
	}
	if (list_size[Cola_Espera_Limpieza_Ref] > 0) {
		list_remove(FIRST, Cola_Espera_Limpieza_Ref);
		transfer[1] = sim_time + tiempo_constante_limpieza;
		transfer[2] = Finaliza_Limpieza_Refinacion;
		list_file(INCREASING, LIST_EVENT);
	}
	else {
		list_remove(FIRST, Procesador_Limpieza);
		timest(0, Media_Procesador_limpieza);
	}

	
	
}

void Rutina_Fin_Lub_Calib_A(void) {

	if (list_size[Procesador_Ensamblado] > 0) {
		transfer[1] = sim_time;
		transfer[2] = Fin_Ensamblado;
		transfer[3] = Pieza_A;
		list_file(LAST, Cola_Ensamblado_A);
	}
	else {
		transfer[1] = sim_time + expon(media_timepo_ensamblado, Fin_Ensamblado);
		transfer[2] = Fin_Ensamblado;
		transfer[3] = Pieza_A;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Procesador_Ensamblado);
	}
	if (list_size[Cola_Espera_Lub_A] > 0) {
		list_remove(FIRST, Cola_Espera_Lub_A);
		transfer[1] = sim_time + expon(media_lub_calib_A, Fin_Lub_Calib_A);
		transfer[2] = Fin_Lub_Calib_A;
		list_file(INCREASING, LIST_EVENT);
	}
	list_remove(FIRST, Procesador_Lubic_A);
}


void Rutina_Fin_Control_Humano_B(void) {
	
	if (lcgrand(1) > 0.05) {
		if (list_size[Procesador_Ensamblado] > 0) {
			transfer[1] = sim_time;
			transfer[2] = Fin_Ensamblado;
			transfer[3] = Pieza_B;
			list_file(LAST, Cola_Ensamblado_B);
		}
		else {
			transfer[1] = sim_time + expon(media_timepo_ensamblado, Fin_Ensamblado);
			transfer[2] = Fin_Ensamblado;
			transfer[3] = Pieza_B;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Procesador_Ensamblado);
		}
	}
	else {
		if (list_size[Cola_Espera_Limpieza_Ref] < 0) {
			transfer[1] = sim_time + tiempo_constante_limpieza;
			transfer[2] = Finaliza_Limpieza_Refinacion;
			transfer[3] = Pieza_B;
		    list_file(INCREASING, LIST_EVENT);
		}
		else {
			transfer[1] = sim_time;
			transfer[2] = Finaliza_Limpieza_Refinacion;
			transfer[3] = Pieza_B;
			list_file(LAST, Cola_Espera_Limpieza_Ref);
		}
	}

	if (list_size[Cola_Espera_Control_Humano_B] > 0) {
		list_remove(FIRST, Cola_Espera_Control_Humano_B);
		transfer[1] = sim_time + uniform(3, 5, Fin_Control_Humano_B);
		transfer[2] = Fin_Control_Humano_B;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Procesador_Control_Human_B);
	}
	list_remove(FIRST, Procesador_Control_Human_B);
}

void Rutina_Fin_Ensamblado(void) {

	int x = transfer[3];

	if (lcgrand(1) < 0.04) {
		if (list_size[Cola_Espera_Limpieza_Ref] > 0) {
			transfer[1] = sim_time;
			transfer[2] = Finaliza_Limpieza_Refinacion;
			list_file(LAST, Cola_Espera_Limpieza_Ref);
		}
		else {
			transfer[1] = sim_time + tiempo_constante_limpieza;
			transfer[2] = Finaliza_Limpieza_Refinacion;
			list_file(INCREASING, LIST_EVENT);
		}
	}

	transfer[3] = x;

	if (transfer[3] = 1) {
		if (list_size[Cola_Ensamblado_B] > 0) {
			list_remove(FIRST, Cola_Ensamblado_B);
			transfer[1] = sim_time + expon(media_timepo_ensamblado, Fin_Ensamblado);
			transfer[2] = Fin_Ensamblado;
			transfer[3] = Pieza_B;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Procesador_Ensamblado);
		}
		else if (list_size[Cola_Ensamblado_A] > 0) {
				list_remove(FIRST, Cola_Ensamblado_A);
				transfer[1] = sim_time + expon(media_timepo_ensamblado, Fin_Ensamblado);
				transfer[2] = Fin_Ensamblado;
				transfer[3] = Pieza_A;
				list_file(INCREASING, LIST_EVENT);
				list_file(FIRST, Procesador_Ensamblado);
		}
	}
	else {
		if (list_size[Cola_Ensamblado_A] > 0) {
			list_remove(FIRST, Cola_Ensamblado_A);
			transfer[1] = sim_time + expon(media_timepo_ensamblado, Fin_Ensamblado);
			transfer[2] = Fin_Ensamblado;
			transfer[3] = Pieza_A;
			list_file(INCREASING, LIST_EVENT);
			list_file(FIRST, Procesador_Ensamblado);
		}
		else {
			if (list_size[Cola_Ensamblado_B] > 0) {
				list_remove(FIRST, Cola_Ensamblado_B);
				transfer[1] = sim_time + expon(media_timepo_ensamblado, Fin_Ensamblado);
				transfer[2] = Fin_Ensamblado;
				transfer[3] = Pieza_B;
				list_file(INCREASING, LIST_EVENT);
				list_file(FIRST, Procesador_Ensamblado);
			}
		}
	}
	list_remove(FIRST, Procesador_Ensamblado);
}

void reporte(void)  
{
	/*printf("\nCantidad de pedidos de estaciones    : %i \n ", cantidad_pedidos_estaciones);

	printf("\nCantidad de pedidos de servidores    : %i \n ", cantidad_pedidos_servidores);*/

	/*filest(Procesador_Limpieza);*/
	timest(0.0, -Media_Procesador_limpieza);
	printf("\nUtilizacion del Procesador Limpieza      : %f \n ", transfer[1]);

 
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


