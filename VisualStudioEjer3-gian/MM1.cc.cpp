/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

#define Pedido_Uso_Red_Servidor_Esp    1
#define Pedido_Uso_Red_Servidor        2
#define Pedido_Uso_Red_Estacion        3
#define Finaliza_Uso_Red               4
#define Error_General                  5

#define Cola_Espera_Uso_Red            1  
#define Red                            2
#define Red_Aux                        3
#define Cola_Aux                       4
#define event_aux                      5

#define Demora_Cola_Estacion           1 
#define Demora_Servidor                2

/* Declaraci¢n de variables propias */

float l, prob_serv, numero, x, estaciones_en_red, servidores_en_red, nro_estaicon, est_o_serv, buscando_elemento, inicio_uso_red, tiempo_uso_red, min_error, max_error, media_pedido_estacion, media_uso_estacion, media_pedido_servidor, media_uso_servidor;
int ;
bool mantenimiento, utilizando_red, bandera;


/* Declaraci¢n de Funciones propias */

void Rutina_Pedido_Uso_Red_Estacion(void);
void Rutina_Pedido_Uso_Red_Servidor(void);
void Rutina_Finaliza_Uso_Red(void);
void Rutina_Error_General(void);
void inicializa(void);
void reporte(void);


int main()  /* Main function. */
{
	/* Apertura de Archivos que sean necesarios */

	media_pedido_estacion = 1.2;
	media_uso_estacion = 3.7;
	media_pedido_servidor = 3.7;
	media_uso_servidor = 2.4;
	min_error = 10800;
	max_error = 14400;
	list_rank[Cola_Espera_Uso_Red], 2;
	
	utilizando_red = false;
	
	
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
			
		case Pedido_Uso_Red_Estacion:
			Rutina_Pedido_Uso_Red_Estacion();
			break;
		case Pedido_Uso_Red_Servidor:
			Rutina_Pedido_Uso_Red_Servidor();
			break;
		case Error_General:
			Rutina_Error_General();
			break;
		case Finaliza_Uso_Red:
			Rutina_Finaliza_Uso_Red();
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

	transfer[1] = sim_time + expon(media_pedido_estacion, Pedido_Uso_Red_Estacion);
	transfer[2] = Pedido_Uso_Red_Estacion;
	transfer[3] = 1;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_pedido_estacion, Pedido_Uso_Red_Estacion);
	transfer[2] = Pedido_Uso_Red_Estacion;
	transfer[3] = 2;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_pedido_estacion, Pedido_Uso_Red_Estacion);
	transfer[2] = Pedido_Uso_Red_Estacion;
	transfer[3] = 3;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_pedido_estacion, Pedido_Uso_Red_Estacion);
	transfer[2] = Pedido_Uso_Red_Estacion;
	transfer[3] = 4;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_pedido_estacion, Pedido_Uso_Red_Estacion);
	transfer[2] = Pedido_Uso_Red_Estacion;
	transfer[3] = 5;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_pedido_estacion, Pedido_Uso_Red_Estacion);
	transfer[2] = Pedido_Uso_Red_Estacion;
	transfer[3] = 6;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + uniform(min_error, max_error, Error_General);
	transfer[2] = Error_General;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_pedido_servidor, Pedido_Uso_Red_Servidor);
	transfer[2] = Pedido_Uso_Red_Servidor;
	transfer[3] = 0;
	list_file(INCREASING, LIST_EVENT);

}

void Rutina_Error_General(void)
{

	/*PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR PREGUNTAR*/

	/*Lo que hice fue vaciar todas las listas y empezar todo de nuevo desde el inicializar.*/

	while (list_size[Cola_Espera_Uso_Red] > 0) {
		list_remove(LAST, Cola_Espera_Uso_Red);
	}

	while (list_size[LIST_EVENT] > 0) {
		list_remove(LAST, LIST_EVENT);
	}

	while (list_size[Red] > 0) {
		list_remove(LAST, Red);
	}

	while (list_size[Red_Aux] > 0) {
		list_remove(LAST, Red_Aux);
	}

	inicializa();
}

void Rutina_Pedido_Uso_Red_Estacion(void) {

	estaciones_en_red = 0;
	servidores_en_red = 0; 

	nro_estaicon = transfer[3]; /*Si es 0 es porque es un servidor, no se si sirve pero bueno*/

	/*Verifico cuantos servidores y estaciones hay en la red y paso todo a una RED AUXILIAR*/

	if (list_size[Red] < 3) {
		x = sim_time + expon(media_uso_estacion, Finaliza_Uso_Red);
		transfer[1] = x;
		transfer[2] = Finaliza_Uso_Red;
		transfer[3] = nro_estaicon; 
		list_file(INCREASING, LIST_EVENT);
		transfer[1] = x;
		transfer[2] = Finaliza_Uso_Red;
		transfer[3] = nro_estaicon;
		list_file(FIRST, Red);
	}
	else {
		transfer[1] = sim_time;
		transfer[2] = Pedido_Uso_Red_Estacion;
		transfer[3] = nro_estaicon;
		list_file(LAST, Cola_Espera_Uso_Red);
	}
	
	
	
}

void Rutina_Finaliza_Uso_Red(void) {
	
	nro_estaicon = transfer[3];
	
	/*Elimino el o los elementos de red que tenga que eliminar*/ 

	while (list_size[Red] > 0) {
		list_remove(FIRST, Red);
		x = transfer[1];
		if (x != sim_time) {
			list_file(FIRST, Red_Aux);
		}
	}
	while (list_size[Red_Aux] > 0) {
		list_remove(FIRST, Red_Aux);
		list_file(FIRST, Red);
	}

	/*Si lo que salio es una estacion, genero en seguida el evento de pedido de esa misma estacion*/

	if (nro_estaicon != 0 & nro_estaicon != 7) {
		transfer[1] = sim_time + expon(media_pedido_estacion, Pedido_Uso_Red_Estacion);
		transfer[2] = Pedido_Uso_Red_Estacion;
		transfer[3] = nro_estaicon;
		list_file(INCREASING, LIST_EVENT);
	}

	if (list_size[Cola_Espera_Uso_Red] != 0) {
		list_remove(FIRST, Cola_Espera_Uso_Red);
		while (list_size[Red] < 3) {
			if (transfer[4] != 0) {    /*Calculo el sim_time de la finalizacion segun el tipo*/
				x = sim_time + transfer[4];
			}
			else if (transfer[3] != 7 & transfer[3] != 0) {
				x = sim_time + expon(media_uso_estacion, Finaliza_Uso_Red);
			}
			else {
				x = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);
			}
			if (transfer[3] == 7) { /*Pongo super servidor, si estaba en conla es porque hay lugar*/
				while (list_size[Red] < 3) {
					transfer[1] = x;
					transfer[2] = Finaliza_Uso_Red;
					transfer[3] = 7;
					list_file(FIRST, Red);
				}
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = 7;
				list_file(INCREASING, LIST_EVENT);
			}
			else if (transfer[3] == 0 & list_size[Red] < 2) { /*Pongo servidor normal*/
				for (int i = 1; i = 2; i++) {
					transfer[1] = x;
					transfer[2] = Finaliza_Uso_Red;
					transfer[3] = 0;
					list_file(FIRST, Red);
				}
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = 0;
				list_file(INCREASING, LIST_EVENT);
			}
			else if (transfer[3] != 7 & transfer[3] != 0) {     /*Pongo una estacion*/
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = transfer[3];    /*No seria necesario pero bue*/
				list_file(FIRST, Red);
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = transfer[3];
				list_file(INCREASING, LIST_EVENT);
			}
			else {      /*Es un servidor el primero en cola, y ya hay uno usando la red*/
				list_file(FIRST, Cola_Aux);
			}
			if (list_size[Cola_Espera_Uso_Red] > 0 & list_size[Red] < 3) {  /*Busca el proximo de la cola*/
				list_remove(FIRST, Cola_Espera_Uso_Red);
			}
			else {     /*Se vacio la cola espera y sigue habiendo lugar en la Red*/
				break;
			}
		}
		while (list_size[Cola_Aux] < 0){
			list_remove(FIRST, Cola_Aux);
			list_file(FIRST, Cola_Espera_Uso_Red);
		}
	}	
}


void Rutina_Pedido_Uso_Red_Servidor(void) {
	

	nro_estaicon = transfer[3]; /*Si es 0 es porque es un servidor, no se si sirve pero bueno*/

	prob_serv = lcgrand(1);

	if (prob_serv <= 0.1) { /*Viene Servidor Especial*/
		if (list_size[Red] != 0) {
			list_remove(FIRST, Red);
			x = transfer[3];
			list_file(FIRST, Red);
			if (x == 7) { /*Hay un Servidor eespecial usando la red*/
				transfer[1] = sim_time;
				transfer[2] = Pedido_Uso_Red_Servidor_Esp;
				transfer[3] = 7;
				list_file(LAST, Cola_Espera_Uso_Red);
			}
			else {      /*Veo que Borra de red y encolo*/
				while (list_size[Red] > 0) {
					list_remove(FIRST, Red);
					if (transfer[3] == 0) {
						list_remove(FIRST, Red);
						transfer[2] = Pedido_Uso_Red_Servidor;
					}
					else {
						transfer[2] = Pedido_Uso_Red_Estacion;
					}
					transfer[1] = sim_time;
					transfer[4] = transfer[1] - sim_time; /*lo que le falta usar la red*/
					list_file(LAST, Cola_Espera_Uso_Red);
					x = transfer[3];
					event_cancel(Finaliza_Uso_Red);
					while (x != transfer[3]) {
						list_file(FIRST, event_aux);
						event_cancel(Finaliza_Uso_Red);
					}
					while (list_size[event_aux] > 0) {
						list_remove(FIRST, event_aux);
						list_file(INCREASING, LIST_EVENT);
					}
					x = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);
					while (list_size[Red] < 3) {
						transfer[1] = x;
						transfer[2] = Finaliza_Uso_Red;
						transfer[3] = 7;
						list_file(FIRST, Red);
					}
					transfer[1] = x;
					transfer[2] = Finaliza_Uso_Red;
					transfer[3] = 7;
					list_file(INCREASING, LIST_EVENT);
				}
			}
		}
		else {
			x = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);
			while (list_size[Red] < 3) {
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = 7;
				list_file(FIRST, Red);
			}
			transfer[1] = x;
			transfer[2] = Finaliza_Uso_Red;
			transfer[3] = 7;
			list_file(INCREASING, LIST_EVENT);
		}
	}
	else { /*  Viene un servidor común*/
		if (list_size[Red] != 0) {
			bandera = false;
			while ((list_size[Red] != 0) & (bandera == false)) { /*controlo si hay un servidor cualquiera usando la red*/
				list_remove(FIRST, Red);
				if (transfer[3] == 7 | transfer[3] == 0)
					bandera = true;
				list_file(FIRST, Red_Aux);
			}
			while (list_size[Red_Aux] != 0) {
				list_remove(FIRST, Red_Aux);
				list_file(FIRST, Red);
			}
			if (bandera == true) {/*hay un servidor cualquiera usando la red*/
				transfer[1] = sim_time;
				transfer[2] = Pedido_Uso_Red_Servidor;
				transfer[3] = 0;
				list_file(LAST, Cola_Espera_Uso_Red);
			}
			else/*hay por lo menos una estación y ningún servidor*/
			{
				while (list_size[Red] > 1) {
					list_remove(FIRST, Red);
					x = transfer[3];
					transfer[2] = Pedido_Uso_Red_Estacion;
					transfer[4] = transfer[1] - sim_time; /*lo que le falta usar la red*/
					list_file(LAST, Cola_Espera_Uso_Red);
					event_cancel(Finaliza_Uso_Red);
					l = transfer[3];
					while (x != transfer[3]) {
						list_file(FIRST, event_aux);
						event_cancel(Finaliza_Uso_Red);
					}
					while (list_size[event_aux] > 0) {
						list_remove(FIRST, event_aux);
						list_file(INCREASING, LIST_EVENT);
					}
				}
				for (int i = 1; i <= 2; i++) {
					transfer[1] = sim_time + expon(media_uso_servidor,Finaliza_Uso_Red);
					transfer[2] = Finaliza_Uso_Red;
					transfer[3] = 0;
					list_file(FIRST, Red);
				}
			}
		}
		else {
			for (int i = 1; i <= 2; i++) {
				transfer[1] = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = 0;
				list_file(FIRST, Red);
			}
		}
	}
}

void reporte(void)  /* Generar Reporte de la Simulaci¢n */
{

	/* Mostrar Par metros de Entrada */

	/* -------- Por pantalla -------- */
	printf("\nTiempo de utilizacion de la red   : %f \n ", filest(Red));
 
	/* Estad¡sticos Escalaras - Sampst */
	/*sampst(0.0, -Demora_Cola_Taxi);
	printf("\nDemora media en Cola Taxi         : %f \n ", transfer[1]);
	printf("\nDemora maxima en Cola Taxi        : %f \n", transfer[3]);

	sampst(0.0, -Demora_Cola_Aeropuerto);
	printf("\nDemora media en Cola Aeropuerto   : %f \n ", transfer[1]);
	printf("\nDemora maxima en Cola Aeropuerto  : %f \n", transfer[3]);
	/* Estad¡sticos Temporales - Timest y Filest */

	/*filest(Cola_Taxi);
	printf("\nN£mero Promedio en Cola Taxi      : %f \n ", transfer[1]);

	filest(Cola_Aeropuerto);
	printf("\nN£mero Promedio en Cola Taxi      : %f \n ", transfer[1]);

	filest(Cola_Colectivo);
	printf("\nCantidad promedio de pasajeros transportados en el colectivo   : %f \n ", transfer[1]);*/

}


