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
#define Silo_1                         1  
#define Silo_2                         2  
#define Silo_3                         3  
#define Cola_Camiones                  4

/*Servidores, continuar numeracion de las colas*/
#define Montacargas                    5

/*Demoras, otra numeracion distinta*/
#define Demora_Cola_1                  1



#define Media_Procesador_limpieza      1
#define Media_Procesamiento_Ensamblado 2


/* Declaraci¢n de variables propias */

float ;
int timepo_proximo_tren, silo_1, silo_2, silo_3;
bool ;


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


	/* Se carga el primer Arribo en la Lista de Eventos */
	transfer[1] = sim_time;
	transfer[2] = Llegada_tren;
	list_file(INCREASING,LIST_EVENT);

	transfer[1] = sim_time + 480;
	transfer[2] = Inicio_Jornada_Laboral;
	list_file(INCREASING, LIST_EVENT); 
}

void Rutina_Inicio_Jornada_Laboral(void){
	

}

void Rutina_Fin_Jornada_Laboral(void) {

	
}

void Rutina_Llegada_Camion(void) {

}

void Rutina_Fin_Carga_Silo(void) {

}

void Rutina_Llegada_Tren(void) {

	transfer[1] = sim_time + timepo_proximo_tren;
	transfer[2] = Llegada_tren;
	list_file(INCREASING, LIST_EVENT);
}

void Rutina_Fin_Carga_Tren(void) {

}


void reporte(void)  
{
	

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


