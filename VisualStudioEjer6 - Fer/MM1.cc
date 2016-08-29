/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

/*Eventos*/
#define Llegada_Camion                 1
#define Fin_Carga_Silo                 2
#define Llegada_tren                   3
#define Fin_Carga_Tren                 4
#define Inicio_Jornada_Laboral         5
#define Fin_Jornada_Laboral            6


/*Colas*/
#define Media_Cant_Silo_1              1  
#define Media_cant_Silo_2              2  
#define Media_cant_Silo_3              3
#define Cola_Camiones                  4

/*Servidores, continuar numeracion de las colas*/
#define Uso_Maquinaria                 5

/*Demoras, otra numeracion distinta*/
#define CamionA                        1
#define CamionB                        2
#define DemoraColaA                    3
#define DemoraColaB                    4 




/* Declaraci¢n de variables propias */

float silo_que_toca, timepo_descarga_cam_gra, timepo_descarga_cam_chi, capacidad_camion_chico, capacidad_camion_grande, tiempo_llegada_proximo_camion, timepo_carga_tren, timepo_proximo_tren, cant_silo_1, cant_tren, cant_silo_2, cant_silo_3;
int camion_a, camion_b;
bool trabajando;


/* Declaraci¢n de Funciones propias */

void Rutina_Llegada_Camion(void);
void Rutina_Fin_Carga_Silo(void);
void Rutina_Llegada_Tren(void);
void Rutina_Fin_Carga_Tren(void);
void Rutina_Inicio_Jornada_Laboral(void);
void Rutina_Fin_Jornada_Laboral(void);
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
			
		case Llegada_Camion:
			Rutina_Llegada_Camion();
			break;
		case Fin_Carga_Silo:
			Rutina_Fin_Carga_Silo();
			break;
		case Llegada_tren:
			Rutina_Llegada_Tren();
			break;
		case Fin_Carga_Tren:
			Rutina_Fin_Carga_Tren();
			break;
		case Inicio_Jornada_Laboral:
			Rutina_Inicio_Jornada_Laboral();
			break;
		case Fin_Jornada_Laboral:
			Rutina_Fin_Jornada_Laboral();
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

	timepo_proximo_tren = 1440;
	timepo_carga_tren = 40;
	tiempo_llegada_proximo_camion = 90;
	capacidad_camion_chico = 20;
	capacidad_camion_grande = 30;
	timepo_descarga_cam_chi = 25;
	timepo_descarga_cam_gra = 40;
	cant_silo_1 = 100;
	cant_silo_2 = 0;
	cant_silo_3 = 0;
	cant_tren = 0;
	trabajando = false;

	/* Se carga el primer Arribo en la Lista de Eventos */
	transfer[1] = sim_time;
	transfer[2] = Llegada_tren;
	transfer[3] = 1;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + 480;
	transfer[2] = Inicio_Jornada_Laboral;
	list_file(INCREASING, LIST_EVENT); 

	transfer[1] = sim_time + expon(tiempo_llegada_proximo_camion, Llegada_Camion);
	transfer[2] = Llegada_Camion;
	list_file(INCREASING, LIST_EVENT);
	
	timest(cant_silo_1, Media_Cant_Silo_1);
}

void Rutina_Inicio_Jornada_Laboral(void){
	
	transfer[1] = sim_time + 600;
	transfer[2] = Fin_Jornada_Laboral;
	list_file(INCREASING, LIST_EVENT);

	trabajando = true;

	transfer[1] = sim_time;
	transfer[2] = Fin_Carga_Silo;
	list_file(INCREASING, LIST_EVENT);
}

void Rutina_Fin_Jornada_Laboral(void) {

	transfer[1] = sim_time + 840;
	transfer[2] = Inicio_Jornada_Laboral;
	list_file(INCREASING, LIST_EVENT);

	trabajando = false;

	if (list_size[Uso_Maquinaria] == 1) {
		list_remove(FIRST, Uso_Maquinaria);
	}
}

void Rutina_Llegada_Camion(void) {

	transfer[1] = sim_time + expon(tiempo_llegada_proximo_camion, Llegada_Camion);
	transfer[2] = Llegada_Camion;
	list_file(INCREASING, LIST_EVENT);

	float hola = list_size[Uso_Maquinaria];

	if (lcgrand(1) <= 0.3) {
		transfer[1] = sim_time + timepo_descarga_cam_chi;
		transfer[3] = capacidad_camion_chico;
	}
	else {
		transfer[1] = sim_time + timepo_descarga_cam_gra;
		transfer[3] = capacidad_camion_grande;
	}

	if ((trabajando == true) && (list_size[Uso_Maquinaria] == 0)) {
		int cambio = 0;
		int falta_descargar = transfer[3];
		while (falta_descargar > 0) {  /*Lleno los silos correspondientes*/
			if (cant_silo_1 < 240) {
				cant_silo_1++;
				falta_descargar--;
			}
			else {
				transfer[1] = transfer[1] + 5;
				break;
			}
		}
		while (falta_descargar > 0) {
			if (cant_silo_2 < 240) {
				cant_silo_2++;
				falta_descargar--;
			}
			else {
				transfer[1] = transfer[1] + 5;
				break;
			}
		}
		while (falta_descargar > 0) {
			if (cant_silo_3 < 240) {
				cant_silo_3++;
				falta_descargar--;
			}
			else {
				transfer[1] = transfer[1] + 5;
				break;
			}
		}
		transfer[2] = Fin_Carga_Silo;
		list_file(INCREASING, LIST_EVENT);
		list_file(FIRST, Uso_Maquinaria);
	}
	else {
		if (transfer[3] == capacidad_camion_chico) {
			camion_a++;
			sampst(camion_a, CamionA);
		}
		else {
			camion_b++;
			sampst(camion_b, CamionB);
		}
		transfer[1] = sim_time;
		list_file(LAST, Cola_Camiones);
	}
}

void Rutina_Fin_Carga_Silo(void) {
	
	
	if (list_size[Uso_Maquinaria] == 1) {
		list_remove(FIRST, Uso_Maquinaria);
	}

	if (trabajando == true) {
		if (list_size[Cola_Camiones] > 0) {
			list_file(FIRST, Uso_Maquinaria);
			list_remove(FIRST, Cola_Camiones);
			if (transfer[3] == capacidad_camion_chico) {
				camion_a--;
				sampst(camion_a, CamionA);
				timest(sim_time - transfer[1], DemoraColaA);
			}
			else {
				camion_b--;
				sampst(camion_b, CamionB);
				timest(sim_time - transfer[1], DemoraColaB);
			}
			if (transfer[3] == capacidad_camion_chico) {
				transfer[1] = sim_time + timepo_descarga_cam_chi;
			}
			else {
				transfer[1] = sim_time + timepo_descarga_cam_gra;
			}
			int falta_descargar = transfer[3];
			while (falta_descargar > 0) {  /*Lleno los silos correspondientes*/
					if (cant_silo_1 < 240) {
						cant_silo_1++;
						falta_descargar--;
					}
					else {
						transfer[1] = transfer[1] + 5;
						break;
					}
				}
			while (falta_descargar > 0) {
					if (cant_silo_2 < 240) {
						cant_silo_2++;
						falta_descargar--;
					}
					else {
						transfer[1] = transfer[1] + 5;
						break;
					}
				}
			while (falta_descargar > 0) {
				if (cant_silo_3 < 240) {
					cant_silo_3++;
					falta_descargar--;
				}
				else {
					transfer[1] = transfer[1] + 5;
					break;
				}
			}
			transfer[2] = Fin_Carga_Silo;
			list_file(INCREASING, LIST_EVENT);
		}
	}
}

void Rutina_Llegada_Tren(void) {

	int x = transfer[3];

	transfer[1] = sim_time + timepo_proximo_tren;
	transfer[2] = Llegada_tren;
	if (transfer[3] == 3) {
		transfer[3] = 1;
	}
	else {
		transfer[3]++;
	}
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + 40;
	transfer[2] = Fin_Carga_Tren;
	list_file(INCREASING, LIST_EVENT);

	int count = 0;
	while (cant_tren < 400) {
		count++;
		if (count > 400) {
			break;
		}
		switch (x)
		{
		case 1:
			if (cant_silo_1 > 0) {
				cant_silo_1--;
				cant_tren++;
			}
			else {
				x = 2;
			}
			break;
		case 2:
			if (cant_silo_2 > 0) {
				cant_silo_2--;
				cant_tren++;
			}
			else {
				x = 3;
			}
			break;
		case 3:
			if (cant_silo_3 > 0) {
				cant_silo_3--;
				cant_tren++;
			}
			else {
				x = 1;
			}
			break;
		}
	}

}

void Rutina_Fin_Carga_Tren(void) {

	cant_tren = 0;
}


void reporte(void)  
{
	filest(Uso_Maquinaria);
	printf("\nUtilizacion maquinaria      : %.2f %%%\n ", transfer[1] * 100);

	sampst(0.0, -CamionA);
	printf("\nCantidad media de camiones chicos en cola     : %.2f \n ", transfer[1]);

	sampst(0.0, -CamionB);
	printf("\nCantidad media de camiones grandes en cola     : %.2f \n ", transfer[1]);

	timest(0.0, -DemoraColaA);
	printf("\nDemora en cola camiones chicos     : %.2f hs\n ", transfer[1]/60);

	timest(0.0, -DemoraColaB);
	printf("\nDemora en cola camiones grandes    : %.2f hs\n ", transfer[1]/60);

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


