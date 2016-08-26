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
#define Event_Aux                      5
#define Cola_Cant_Ped_Serv             6

#define Demora_Cola_Estacion_1         1
#define Demora_Cola_Estacion_2         2
#define Demora_Cola_Estacion_3         3
#define Demora_Cola_Estacion_4         4
#define Demora_Cola_Estacion_5         5
#define Demora_Cola_Estacion_6         6


/* Declaraci¢n de variables propias */

float numero_estacion, prob_serv, numero, x, nro_estaicon, min_error, max_error, media_pedido_estacion, media_uso_estacion, media_pedido_servidor, media_uso_servidor;
int fallas, cantidad_pedidos_expulsados, cantidad_pedidos_estaciones, cantidad_pedidos_servidores;
bool bandera;


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
	
	cantidad_pedidos_expulsados = 0;
	fallas = 0;
	cantidad_pedidos_estaciones = 0;
	cantidad_pedidos_servidores = 0;


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
	media_pedido_estacion = 1.2;
	media_uso_estacion = 3.7;
	media_pedido_servidor = 3.7;
	media_uso_servidor = 2.4;
	min_error = 10800;
	max_error = 14400;
	list_rank[Cola_Espera_Uso_Red] = 2;


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
	fallas = fallas + 1;

	while (list_size[Cola_Espera_Uso_Red] > 0) {
		list_remove(LAST, Cola_Espera_Uso_Red);
	}

	while (list_size[LIST_EVENT] > 0) {
		list_remove(LAST, LIST_EVENT);
	}

	while (list_size[Red] > 0) {
		list_remove(LAST, Red);
	}

	while (list_size[Cola_Cant_Ped_Serv] > 0) {
		list_remove(LAST, Cola_Cant_Ped_Serv);
	}

	while (list_size[Red_Aux] > 0) {
		list_remove(LAST, Red_Aux);
	}

	inicializa();
}

void Rutina_Pedido_Uso_Red_Estacion(void) {

	cantidad_pedidos_estaciones = cantidad_pedidos_estaciones + 1;

	nro_estaicon = transfer[3]; 

	if (list_size[Red] < 3) {        /*Hay lugar para la estacion*/
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
		list_file(INCREASING, Cola_Espera_Uso_Red);
	}
}

void Rutina_Finaliza_Uso_Red(void) {
	
	nro_estaicon = transfer[3];
	
	/*Elimino el o los elementos de red que tenga que eliminar*/ 

	while (list_size[Red] > 0) {
		list_remove(FIRST, Red);
		if (nro_estaicon != transfer[3]) {
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
		numero_estacion = transfer[3];
		while (list_size[Red] < 3) {
			if (transfer[4] != 0) {    
				x = sim_time + transfer[4];   /*Algo que lo interrumpieron y lo sacaron*/
			}
			else if (transfer[3] != 7 & transfer[3] != 0) {                  /*Hay una estacion por entrar que nunca entro a la red*/
				x = sim_time + expon(media_uso_estacion, Finaliza_Uso_Red);
			}
			else {               
				x = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);             /*Es un servidor que nunca entro a la red*/
			}
			if (transfer[3] == 7) {        /*Pongo un super servidor que estaba en cola y entra directo porque la unica forma es que haya salido un super servidor*/
				while (list_size[Red] < 3) {   /*Cargo los 3 casilleros que ocupa el super servidor*/
					transfer[1] = x;
					transfer[2] = Finaliza_Uso_Red;
					transfer[3] = 7;
					list_file(FIRST, Red);
				}
				transfer[1] = x;    /*Cargo una sola vez el pedido de finalizacion para el super servidor*/
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = 7;
				list_file(INCREASING, LIST_EVENT);
				list_remove(FIRST, Cola_Cant_Ped_Serv);
			}
			else if (transfer[3] == 0 & list_size[Red] < 2) { /*Pongo un servidor normal porque hay lugar*/
				for (int i = 1; i <= 2; i++) {
					transfer[1] = x;
					transfer[2] = Finaliza_Uso_Red;
					transfer[3] = 0;
					list_file(FIRST, Red);
				}
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = 0;
				list_file(INCREASING, LIST_EVENT);
				list_remove(FIRST, Cola_Cant_Ped_Serv);
			}
			else if (transfer[3] != 7 & transfer[3] != 0) {     /*Pongo una estacion*/
				int Demora = numero_estacion;
				switch (Demora)
				{
				case 1:
					sampst((sim_time - transfer[1]), Demora_Cola_Estacion_1);
					break;
				case 2:
					sampst((sim_time - transfer[1]), Demora_Cola_Estacion_2);
					break;
				case 3:
					sampst((sim_time - transfer[1]), Demora_Cola_Estacion_3);
					break;
				case 4:
					sampst((sim_time - transfer[1]), Demora_Cola_Estacion_4);
					break;
				case 5:
					sampst((sim_time - transfer[1]), Demora_Cola_Estacion_5);
					break;
				case 6:
					sampst((sim_time - transfer[1]), Demora_Cola_Estacion_6);
					break;
				}
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = numero_estacion;    
				list_file(FIRST, Red);
				transfer[1] = x;
				transfer[2] = Finaliza_Uso_Red;
				transfer[3] = numero_estacion;
				list_file(INCREASING, LIST_EVENT);
			}
			else {      /*Es un servidor el primero en cola, y ya hay uno usando la red*/
				list_file(FIRST, Cola_Aux);
			}
			if (list_size[Cola_Espera_Uso_Red] > 0 & list_size[Red] < 3) {  /*Busca el proximo de la cola porque no esta llena la red y hay alguno en cola*/
				list_remove(FIRST, Cola_Espera_Uso_Red);
				numero_estacion = transfer[3];
			}
			else {     /*Se vacio la cola espera o se lleno la red*/
				break;
			}
		}
		while (list_size[Cola_Aux] > 0){   /*Vuelvo los servidores a la cola de espera los cuales no puedieron entrar a la red porque habia otro servidor usando*/
			list_remove(FIRST, Cola_Aux);
			list_file(INCREASING, Cola_Espera_Uso_Red);
		}
	}	
}


void Rutina_Pedido_Uso_Red_Servidor(void) {
	
	cantidad_pedidos_servidores = cantidad_pedidos_servidores + 1;

	nro_estaicon = transfer[3]; 

	prob_serv = lcgrand(1);

	transfer[1] = sim_time + expon(media_pedido_servidor, Pedido_Uso_Red_Servidor);
	transfer[2] = Pedido_Uso_Red_Servidor;
	transfer[3] = 0;
	list_file(INCREASING, LIST_EVENT);

	if (prob_serv <= 0.1) {               /*Viene servidor especial*/
		if (list_size[Red] != 0) {      
			list_remove(FIRST, Red);        
			x = transfer[3];
			list_file(FIRST, Red);
			if (x == 7) {              /*Hay un servidor especial usando la red*/
				transfer[1] = sim_time;
				transfer[2] = Pedido_Uso_Red_Servidor_Esp;            
				transfer[3] = 7;
				list_file(INCREASING, Cola_Espera_Uso_Red);            /*Encolo el servidor especial*/

				list_file(FIRST, Cola_Cant_Ped_Serv);       /*Para sacar cantidad media de servidores en cola*/

			}
			else {                                /*Hay una estacion o un servidor normal en la red*/
				while (list_size[Red] > 0) {
					list_remove(FIRST, Red);
					if (transfer[3] == 0) {           /*Si hay un servidor en la red*/
						list_file(FIRST, Cola_Cant_Ped_Serv);        

						list_remove(FIRST, Red);                   /*Borra la segunda casilla que ocupa el servidor normal a sacar*/
						transfer[2] = Pedido_Uso_Red_Servidor;
					}
					else {                                                 
						transfer[2] = Pedido_Uso_Red_Estacion;    /*Lo que saca de la red es una estacion*/
					}
					transfer[4] = transfer[1] - sim_time;        /*Lo que le falta usar de la red*/
					transfer[1] = sim_time;
					x = transfer[3];
					list_file(INCREASING, Cola_Espera_Uso_Red);
					cantidad_pedidos_expulsados = cantidad_pedidos_expulsados + 1; 
					event_cancel(Finaliza_Uso_Red);              
					while (x != transfer[3]) {                  /*Sirve para cancelar el evento que fue interrumpido*/
						list_file(FIRST, Event_Aux);    
						event_cancel(Finaliza_Uso_Red);
					}
					while (list_size[Event_Aux] > 0) {          /*Sirve para cancelar el evento que fue interrumpido*/
						list_remove(FIRST, Event_Aux);
						list_file(INCREASING, LIST_EVENT);
					}
				}
				x = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);
				while (list_size[Red] < 3) {                   /*Carga de los tres casilleros que oucpa el servidor especial*/
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
		else {                     /*La red esta vacia para el servidor especial*/
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
	else {          /*Viene un servidor común*/
		if (list_size[Red] != 0) {
			bandera = false;
			while ((list_size[Red] != 0) & (bandera == false)) {     /*Controlo si hay un servidor cualquiera usando la red*/
				list_remove(FIRST, Red);
				if (transfer[3] == 7 | transfer[3] == 0)
					bandera = true;
				list_file(FIRST, Red_Aux);
			}
			while (list_size[Red_Aux] != 0) {
				list_remove(FIRST, Red_Aux);
				list_file(FIRST, Red);
			}
			if (bandera == true) {                                /*Hay un servidor cualquiera usando la red*/
				transfer[1] = sim_time;
				transfer[2] = Pedido_Uso_Red_Servidor;
				transfer[3] = 0;
				list_file(INCREASING, Cola_Espera_Uso_Red);

				list_file(FIRST, Cola_Cant_Ped_Serv);
			}
			else          /*Hay por lo menos una estación y ningún servidor*/
			{
				while (list_size[Red] > 1) {        /*Vacio la red hasta que haya un lugar para que entre el servidor normal*/
					list_remove(FIRST, Red);
					x = transfer[3];
					transfer[2] = Pedido_Uso_Red_Estacion;
					transfer[4] = transfer[1] - sim_time;        /*Lo que le falta usar de la red a la estacion*/
					transfer[1] = sim_time;
					list_file(INCREASING, Cola_Espera_Uso_Red);
					cantidad_pedidos_expulsados = cantidad_pedidos_expulsados + 1;
					event_cancel(Finaliza_Uso_Red);
					while (x != transfer[3]) {
						list_file(FIRST, Event_Aux);
						event_cancel(Finaliza_Uso_Red);
					}
					while (list_size[Event_Aux] > 0) {
						list_remove(FIRST, Event_Aux);
						list_file(INCREASING, LIST_EVENT);
					}
				}
				x = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);
				for (int i = 1; i <= 2; i++) {
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
		}
		else {      /*La red esta vacia para el servidor normal*/
			x = sim_time + expon(media_uso_servidor, Finaliza_Uso_Red);
			for (int i = 1; i <= 2; i++) {         
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
	}
}

void reporte(void)  
{
	printf("\nCantidad de pedidos de estaciones    : %i \n ", cantidad_pedidos_estaciones);

	printf("\nCantidad de pedidos de servidores    : %i \n ", cantidad_pedidos_servidores);

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

	printf("\nCantidad de fallas generales                         : %i \n ", fallas);
	
}


