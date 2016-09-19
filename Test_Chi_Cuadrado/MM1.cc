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

float t1, t2, t3, silo_que_toca, timepo_descarga_cam_gra, timepo_descarga_cam_chi, capacidad_camion_chico, capacidad_camion_grande, tiempo_llegada_proximo_camion, timepo_carga_tren, timepo_proximo_tren, cant_silo_1, cant_tren, cant_silo_2, cant_silo_3;
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




void reporte(void)  
{
	
	
}


