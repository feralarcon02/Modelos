/* Modelo MM1 - Un Servidor, Una Cola  */


#include "simlib.c"
#include <stdio.h>
#include <stdlib.h> 



#define Arribo_Colectivo_Taxi          1  
#define Arribo_Colectivo_Aeropuerto    2 
#define Arribo_Persona_Taxi            3  
#define Arribo_Persona_Aeropuerto      4
#define Revision					   5	
#define Cola_Taxi                      1  
#define Cola_Aeropuerto                2 
#define Cola_Colectivo                 3
#define Demora_Cola_Taxi               1 
#define Demora_Cola_Aeropuerto         2

/* Declaraci¢n de variables propias */

float enrevision, media_interarribos_taxi, media_interarribos_aeropuerto, una_persona, dos_personas, tres_personas, cuatro_personas, prob_personas, tiempo_revision, demora_mantenimiento;
int hola, contador_pasajeros, personas, minimo, maximo, inicio_colectivo, min_salida, max_salida, capacidad_colectivo;
bool mantenimiento;


/* Declaraci¢n de Funciones propias */

void inicializa(void);
void Rutina_Arribo_Colectivo_Aeropuerto(void);
void Rutina_Arribo_Colectivo_Taxi(void);
void Rutina_Arribo_Persona_Taxi(void);
void Rutina_Arribo_Persona_Aeropuerto(void);
void Rutina_Revision(void);
void reporte(void);


int main()  /* Main function. */
{
	/* Apertura de Archivos que sean necesarios */


	media_interarribos_taxi = 7.9;
	media_interarribos_aeropuerto = 12.8;
	inicio_colectivo = 30;
	tiempo_revision = 240;
	min_salida = 10;
	max_salida = 15;
	demora_mantenimiento = 15;
	mantenimiento = false;
	capacidad_colectivo = 25;
	una_persona = 0.3;
	dos_personas = 0.45;
	tres_personas = 0.15;
	cuatro_personas = 0.1;
	


	/* Initializar Simlib */
	init_simlib();


	/* Establecer maxatr = M ximo n£mero de Atributos utilizados (Numero máximo que voy a utilizar del transfer)  */
	maxatr = 4;

	/* Initializar el Sistema */
	inicializa();

	/* Ejecutar la simulaci¢n. */


	while (sim_time <= 10080)
	{
		if (sim_time >= 8000) {
			hola = hola + 1;
		}
		/* Determinar pr¢ximo Evento */
		timing();

		/* Invoke the appropriate event function. */

		switch (next_event_type)
		{
		case Arribo_Colectivo_Aeropuerto:
			Rutina_Arribo_Colectivo_Aeropuerto();
			break;
		case Arribo_Colectivo_Taxi:
			Rutina_Arribo_Colectivo_Taxi();
			break;
		case Arribo_Persona_Taxi:
			Rutina_Arribo_Persona_Taxi();
			break;
		case Arribo_Persona_Aeropuerto:
			Rutina_Arribo_Persona_Aeropuerto();
			break;
		case Revision:
			Rutina_Revision();
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

	transfer[1] = sim_time + expon(media_interarribos_taxi, Arribo_Persona_Taxi);
	transfer[2] = Arribo_Persona_Taxi;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + expon(media_interarribos_aeropuerto, Arribo_Persona_Aeropuerto);
	transfer[2] = Arribo_Persona_Aeropuerto;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + inicio_colectivo;
	transfer[2] = Arribo_Colectivo_Taxi;
	list_file(INCREASING, LIST_EVENT);

	transfer[1] = sim_time + tiempo_revision;
	transfer[2] = Revision;
	list_file(INCREASING, LIST_EVENT);



}

void Rutina_Revision(void)
{
	transfer[1] = sim_time + tiempo_revision;
	transfer[2] = Revision;
	list_file(INCREASING, LIST_EVENT);

	mantenimiento = true;
}

void Rutina_Arribo_Colectivo_Aeropuerto(void)
{
	/* Determinar pr¢ximo arribo y cargar en Lista de Eventos */
	transfer[1] = sim_time + uniform(min_salida, max_salida, Arribo_Colectivo_Taxi);
	transfer[2] = Arribo_Colectivo_Taxi;
	list_file(INCREASING, LIST_EVENT);

	while (list_size[Cola_Colectivo] > 0)
	{
		list_remove(FIRST, Cola_Colectivo);
	}

	while ((list_size[Cola_Aeropuerto] > 0) & (list_size[Cola_Colectivo] < capacidad_colectivo))
	{
		list_remove(FIRST, Cola_Aeropuerto);
		sampst((sim_time - transfer[1]), Demora_Cola_Aeropuerto); /*DUDA*/
		list_file(FIRST, Cola_Colectivo);
	}

}

void Rutina_Arribo_Colectivo_Taxi(void)
{
	enrevision = 0;
	if (mantenimiento == true)
	{
		mantenimiento = false;
		enrevision = sim_time + demora_mantenimiento;   /*ERRORRRRRRRRRRR si hay mantenimiento se le suma al pronostico del nuevo arribo*/
	}
	if (enrevision != 0) {
		transfer[1] = enrevision + uniform(min_salida, max_salida, Arribo_Colectivo_Aeropuerto);
	}
	else
	{
		transfer[1] = sim_time + uniform(min_salida, max_salida, Arribo_Colectivo_Aeropuerto);
	}
	transfer[2] = Arribo_Colectivo_Aeropuerto;
	list_file(INCREASING, LIST_EVENT);
	while (list_size[Cola_Colectivo] > 0)
	{
		list_remove(FIRST, Cola_Colectivo);
	}

	while ((list_size[Cola_Taxi] > 0) & (list_size[Cola_Colectivo] < capacidad_colectivo))
	{
		list_remove(FIRST, Cola_Taxi);
		sampst((sim_time - transfer[1]), Demora_Cola_Taxi);
		list_file(FIRST, Cola_Colectivo);
	}
}

void Rutina_Arribo_Persona_Aeropuerto(void)
{
	prob_personas = lcgrand(7);
	if (prob_personas < una_persona)
	{
		personas = 1;
	}
	else if (prob_personas < (una_persona + dos_personas))
	{
		personas = 2;
	}
	else if (prob_personas < (una_persona + dos_personas + tres_personas))
	{
		personas = 3;
	}
	else
	{
		personas = 4;
	}
	while (personas > 0)
	{
		transfer[1] = sim_time;
		list_file(LAST, Cola_Aeropuerto);
		personas = personas - 1;
	}
	transfer[1] = sim_time + expon(media_interarribos_aeropuerto, Arribo_Persona_Aeropuerto);
	transfer[2] = Arribo_Persona_Aeropuerto;
	list_file(INCREASING, LIST_EVENT);
}

void Rutina_Arribo_Persona_Taxi(void)
{
	prob_personas = lcgrand(7);
	if (prob_personas < una_persona)
	{
		personas = 1;
	}
	else if (prob_personas < (una_persona + dos_personas))
	{
		personas = 2;
	}
	else if (prob_personas < (una_persona + dos_personas + tres_personas))
	{
		personas = 3;
	}
	else
	{
		personas = 4;
	}
	while (personas > 0)
	{
		transfer[1] = sim_time;
		list_file(LAST, Cola_Taxi);
		personas = personas - 1;
	}
	transfer[1] = sim_time + expon(media_interarribos_taxi, Arribo_Persona_Taxi);
	transfer[2] = Arribo_Persona_Taxi;
	list_file(INCREASING, LIST_EVENT);
}


void reporte(void)  /* Generar Reporte de la Simulaci¢n */
{

	/* Mostrar Par metros de Entrada */

	/* -------- Por pantalla -------- */


	/* Estad¡sticos Escalaras - Sampst */
	sampst(0.0, -Demora_Cola_Taxi);
	printf("\nDemora media en Cola Taxi         : %f \n ", transfer[1]);
	printf("\nDemora maxima en Cola Taxi        : %f \n", transfer[3]);

	sampst(0.0, -Demora_Cola_Aeropuerto);
	printf("\nDemora media en Cola Aeropuerto   : %f \n ", transfer[1]);
	printf("\nDemora maxima en Cola Aeropuerto  : %f \n", transfer[3]);
	/* Estad¡sticos Temporales - Timest y Filest */

	filest(Cola_Taxi);
	printf("\nN£mero Promedio en Cola Taxi      : %f \n ", transfer[1]);

	filest(Cola_Aeropuerto);
	printf("\nN£mero Promedio en Cola Taxi      : %f \n ", transfer[1]);

	filest(Cola_Colectivo);
	printf("\nCantidad promedio de pasajeros transportados en el colectivo   : %f \n ", transfer[1]);

}


