/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 

/*Eventos*/
#define Arribo_Limpieza_Refinacion     1

/*Colas*/
#define Cola_Espera_Limpieza_Ref       1  

/*Servidores, continuar numeracion de las colas*/
#define Procesador_Limpieza            6

/*Demoras, otra numeracion distinta*/
#define Demora_A_Limpieza              1


#define Media_Procesador_limpieza      1
#define Media_Procesamiento_Ensamblado 2


/* Declaraci¢n de variables propias */

float ;
int ;
bool ;


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
	

}

void Rutina_Arribo_Limpieza_Ref(void)
{
	
	
}

void Rutina_Fin_Limpieza_Ref(void) {


}

void Rutina_Fin_Lub_Calib_A(void) {

	
}


void Rutina_Fin_Control_Humano_B(void) {
	
	
}

void Rutina_Fin_Ensamblado(void) {


}

void reporte(void)  
{
	/*printf("\nCantidad de pedidos de estaciones    : %i \n ", cantidad_pedidos_estaciones);

	printf("\nCantidad de pedidos de servidores    : %i \n ", cantidad_pedidos_servidores);*/

	/*filest(Procesador_Limpieza);*/
	timest(0.0, -Media_Procesador_limpieza);
	printf("\nUtilizacion del Procesador Limpieza      : %f \n ", transfer[1]);

	float x = transfer[1];

	printf("\nUtilizacion del Procesador Lado A      : %f \n ", filest(Procesador_Lubic_A));   /*A estos 3 que siguen los hice de esta forma*/
																								/*Porque me dio paja cambiar todo, y tambien sirve*/
	printf("\nUtilizacion del Procesador Lado B     : %f \n ", filest(Procesador_Control_Human_B));

	printf("\nUtilizacion del Procesador Ensamblado  (Esta mal, a ver si se dan cuenta porquee..)   : %f \n ", filest(Procesador_Ensamblado));

	timest(0.0, -Media_Procesamiento_Ensamblado);
	printf("\nUtilizacion Procesador Ensamblado mas preciso     : %f \n ", transfer[1]);
	
	sampst(0.0, -Demora_A_Limpieza);
	printf("\nDemora media en cola limpieza, pieza A     : %f Horas \n ", transfer[1]/60);

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

	/*printf("\nTiempo medio permanencia de una pieza en el sistema      : %f \n ", 21600/Total_piezas_atendidas);*/

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


