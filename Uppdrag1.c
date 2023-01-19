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

#define MOTOR_BOTH_F     	( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att b�da motorerna styrs samtidigt */
#define MOTOR_BOTH_B		( MOTOR_LEFT | MOTOR_RIGHT )
#define MOTOR_LEFT_F		( MOTOR_LEFT )
#define MOTOR_LEFT_B		( MOTOR_LEFT )
#define MOTOR_RIGHT_F		( MOTOR_RIGHT )
#define MOTOR_RIGHT_B		( MOTOR_RIGHT )

int max_hastighet;			 /* variabel f�r max hastighet p� motorn */

POOL_T touchSensor;			//l�gg till touch variabel.
int TouchReturnValue = 0;	//tillh�rande variabel.

POOL_T gyroSensor;			//l�gg till Gyro variabel. 
int gyroAngle = 0;			//tillh�rande variabler.
int PregyroAngle = 0;
//int gyroValue1 = 0; 
int max_gyro;

POOL_T sensor_ultrasonic;	//l�gg till US-Sensor variabel.
int distance = 0;				//tillh�rande variabel.
int us_value = 0;
int distance_min = 2000;


void activate_robot()
{
	if (!brick_init()) return (1); /* Initialiserar EV3-klossen */
	printf("*** ( EV3 ) Hello! ***\n");
	Sleep(2000);


	gyroSensor = sensor_search(LEGO_EV3_GYRO); //Registrera Gyro sensor

	//gyro_set_mode_gyro_ang(gyroSensor);//Gyron set p� angel

	sensor_ultrasonic = sensor_search(LEGO_EV3_US); //registrera ultrasound
	us_set_mode_us_dist_cm(sensor_ultrasonic);  //�ndra l�ge

	Sleep(2000);

	if (tacho_is_plugged(MOTOR_BOTH_F, TACHO_TYPE__NONE_)) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */
		max_hastighet = tacho_get_max_speed(MOTOR_LEFT, 0);    /* Kollar maxhastigheten som motorn kan ha */
		tacho_reset(MOTOR_BOTH_F);
		tacho_reset(MOTOR_BOTH_B);
	}
	else {
		printf("Anslut v�nster motor i port A,\n""Anslut h�ger motor i port B.\n");
		brick_uninit();
		return (0);  /* St�nger av sig om motorer ej �r inkopplade */
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
	gyro_set_mode_gyro_ang(gyroSensor);//Gyron set p� angel
	gyroAngle = sensor_get_value(0, gyroSensor, 0);
	printf("Angle: %d\n", gyroAngle);


}

void turn_left(int angle)
{
	PregyroAngle = gyroAngle;
	while (gyroAngle > PregyroAngle - angle) //sv�nga �t V�nster 
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
	while (gyroAngle < PregyroAngle + angle) //sv�nga �t H�ger 
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


	tacho_set_speed_sp(MOTOR_BOTH_F, max_hastighet * -0.5);		//b�da motorer Framm�t.50% speed
	tacho_set_speed_sp(MOTOR_BOTH_B, max_hastighet * 0.5);		//b�da motorer bak�t.50% speed
	tacho_set_speed_sp(MOTOR_RIGHT_F, max_hastighet * -0.5);		//H�gra Motor framm�t.50% speed
	tacho_set_speed_sp(MOTOR_RIGHT_B, max_hastighet * 0.5);	//H�gra Motor bak�t.50% speed
	tacho_set_speed_sp(MOTOR_LEFT_F, max_hastighet * -0.5);		//V�nstra Motor framm�t.50% speed
	tacho_set_speed_sp(MOTOR_LEFT_B, max_hastighet * 0.5);		//V�nstra Motor bak�t.50% speed



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



