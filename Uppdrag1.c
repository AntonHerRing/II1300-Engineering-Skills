#include <stdio.h>
#include "brick.h"
#include <unistd.h>
#include <stdlib.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */

#define MOTOR_RIGHT    	OUTA
#define MOTOR_LEFT    	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_SERVO     OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_LIGHT	IN2
#define SENSOR_SOUND	IN3
#define SENSOR_GYRO		IN4

#define MOTOR_BOTH_F     	( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */
#define MOTOR_BOTH_B		( MOTOR_LEFT | MOTOR_RIGHT )
#define MOTOR_LEFT_F		( MOTOR_LEFT )
#define MOTOR_LEFT_B		( MOTOR_LEFT )
#define MOTOR_RIGHT_F		( MOTOR_RIGHT )
#define MOTOR_RIGHT_B		( MOTOR_RIGHT )

int max_hastighet;			 /* variabel för max hastighet på motorn */

POOL_T touchSensor;			//lägg till touch variabel.
int TouchReturnValue = 0;	//tillhörande variabel.

POOL_T gyroSensor;			//lägg till Gyro variabel. 
int gyroAngle = 0;			//tillhörande variabler.
int PregyroAngle = 0;
//int gyroValue1 = 0; 
int max_gyro;

POOL_T sensor_ultrasonic;	//lägg till US-Sensor variabel.
int distance = 0;				//tillhörande variabel.
int us_value = 0;
int distance_min = 2000;


void activate_robot()
{
	if (!brick_init()) return (1); /* Initialiserar EV3-klossen */
	printf("*** ( EV3 ) Hello! ***\n");
	Sleep(2000);


	gyroSensor = sensor_search(LEGO_EV3_GYRO); //Registrera Gyro sensor

	//gyro_set_mode_gyro_ang(gyroSensor);//Gyron set på angel

	sensor_ultrasonic = sensor_search(LEGO_EV3_US); //registrera ultrasound
	us_set_mode_us_dist_cm(sensor_ultrasonic);  //ändra läge

	Sleep(2000);

	if (tacho_is_plugged(MOTOR_BOTH_F, TACHO_TYPE__NONE_)) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */
		max_hastighet = tacho_get_max_speed(MOTOR_LEFT, 0);    /* Kollar maxhastigheten som motorn kan ha */
		tacho_reset(MOTOR_BOTH_F);
		tacho_reset(MOTOR_BOTH_B);
	}
	else {
		printf("Anslut vänster motor i port A,\n""Anslut höger motor i port B.\n");
		brick_uninit();
		return (0);  /* Stänger av sig om motorer ej är inkopplade */
	}
}
void find_wall()
{


	distance = sensor_get_value(0, sensor_ultrasonic, 0); //hitta distansen

	printf("*** ( EV3 ) Hello! ***\n");
	printf("innan while");

	for (int i = 1; i <= 94; i++)		//0,2 hastighet 47s
	{
		tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * -0.1);
		tacho_run_forever(MOTOR_RIGHT_F);
		tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * 0.1);
		tacho_run_forever(MOTOR_LEFT_B);
		Sleep(100);
		distance = sensor_get_value(0, sensor_ultrasonic, 0); //hitta distansen
		if (distance < distance_min)
		{
			distance_min = distance;

		}

	}
	tacho_stop(MOTOR_RIGHT);
	tacho_stop(MOTOR_LEFT);
	Sleep(2000);
	while (distance > distance_min+5)
	{
		tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * -0.1);
		tacho_run_forever(MOTOR_RIGHT_F);
		tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * 0.1);
		tacho_run_forever(MOTOR_LEFT_B);
		distance = sensor_get_value(0, sensor_ultrasonic, 0); //hitta distansen
	}
	tacho_stop(MOTOR_RIGHT);
	tacho_stop(MOTOR_LEFT);
	Sleep(2000);
}

void reset_gyro()
{
	printf("reset gyro\n");
	printf("before reset Angle: %d\n", gyroAngle);
	gyro_set_mode_gyro_ang(gyroSensor);//Gyron set på angel
	gyroAngle = sensor_get_value(0, gyroSensor, 0);
	printf("Angle: %d\n", gyroAngle);


}

void turn_left(int angle)
{
	PregyroAngle = gyroAngle;
	while (gyroAngle > PregyroAngle - angle) //svänga åt Vänster 
	{

		tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * -0.1);
		tacho_run_forever(MOTOR_RIGHT_F);
		tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * 0.1);
		tacho_run_forever(MOTOR_LEFT_B);
		gyroAngle = sensor_get_value(0, gyroSensor, 0);
		printf("Turning: %d\n", gyroAngle);
	}
	tacho_stop(MOTOR_RIGHT);
	tacho_stop(MOTOR_LEFT);
	Sleep(2000);
}

void turn_right(int angle)
{
	PregyroAngle = gyroAngle;
	while (gyroAngle < PregyroAngle + angle) //svänga åt Höger 
	{

		tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * 0.1);
		tacho_run_forever(MOTOR_RIGHT_F);
		tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * -0.1);
		tacho_run_forever(MOTOR_LEFT_B);
		gyroAngle = sensor_get_value(0, gyroSensor, 0);
		printf("Turning 2: %d\n", gyroAngle);
	}
	tacho_stop(MOTOR_RIGHT);
	tacho_stop(MOTOR_LEFT);
	Sleep(2000);
}

void drop_book()
{
	tacho_stop(MOTOR_RIGHT);
	tacho_stop(MOTOR_LEFT);
	Sleep(2000);
	tacho_set_speed_sp(MOTOR_SERVO, max_hastighet * -0.4);
	tacho_run_forever(MOTOR_SERVO);
	Sleep(600);
	tacho_stop(MOTOR_SERVO);
	Sleep(1000);
	tacho_set_speed_sp(MOTOR_SERVO, max_hastighet * 0.4);
	tacho_run_forever(MOTOR_SERVO);
	Sleep(600);
	tacho_stop(MOTOR_SERVO);
	Sleep(1000);
	tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * -0.5);
	tacho_run_forever(MOTOR_RIGHT_F);
	tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * -0.5);
	tacho_run_forever(MOTOR_LEFT_B);
	Sleep(1000);
	tacho_stop(MOTOR_RIGHT);
	tacho_stop(MOTOR_LEFT);

}

void go_250(int time)
{
	tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * 0.4);
	tacho_run_forever(MOTOR_RIGHT_F);
	tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * 0.4);
	tacho_run_forever(MOTOR_LEFT_B);
	Sleep(time);
	tacho_stop(MOTOR_RIGHT);
	tacho_stop(MOTOR_LEFT);
	Sleep(2000);

}

void stop_at(int dist)
{
	distance = sensor_get_value(0, sensor_ultrasonic, 0);
	while (distance > dist)  //milimeter
	{
		tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * 0.2);
		tacho_run_forever(MOTOR_RIGHT_F);
		tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * 0.2);
		tacho_run_forever(MOTOR_LEFT_B);
		distance = sensor_get_value(0, sensor_ultrasonic, 0);
	}
}


int main(void)
{


	tacho_set_speed_sp(MOTOR_BOTH_F, max_hastighet * -0.5);		//båda motorer Frammåt.50% speed
	tacho_set_speed_sp(MOTOR_BOTH_B, max_hastighet * 0.5);		//båda motorer bakåt.50% speed
	tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * -0.5);		//Högra Motor frammåt.50% speed
	tacho_set_speed_sp(MOTOR_RIGHT_B, max_hastighet * 0.5);	//Högra Motor bakåt.50% speed
	tacho_set_speed_sp(MOTOR_LEFT_F, max_hastighet * -0.5);		//Vänstra Motor frammåt.50% speed
	tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * 0.5);		//Vänstra Motor bakåt.50% speed



	//TouchReturnValue = sensor_get_value(0, touchSensor, 0);

	//gyroAngle = sensor_get_value(0, gyroSensor, 0);
	activate_robot();

	find_wall();

	reset_gyro();

	turn_right(90);

	go_250(13000);

	reset_gyro();

	turn_left(90);
	//find_wall();

	stop_at(300); //milimeters

	drop_book();



}



