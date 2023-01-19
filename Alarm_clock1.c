
#include <stdio.h>

// Anton Herdin Ringstedt Klass TIEDB antonhr@kth.se

void clockwork();
int update_alarm();

int main()
{
    int usertime;
    int useralarm;
    int alarmnow = 0;

    printf("Set Time(HH:MM:SS): ");
    scanf("%d", &usertime);

    int sec1 = usertime % 100;
    int hour1 = usertime / 10000;
    int min1 = (usertime - (hour1 * 10000)) / 100;

    printf("Set Alarm(HH:MM:SS): ");
    scanf("%d", &useralarm);

    int sec2 = useralarm % 100;
    int hour2 = useralarm / 10000;
    int min2 = (useralarm - (hour2 * 10000)) / 100;

    printf("Time set for: %d:%d:%d\n", hour1, min1, sec1);
    printf("Alarm set for: %d:%d:%d\n", hour2, min2, sec2);

    clockwork(sec1, min1, hour1, sec2, min2, hour2);

    return 0;
}
void clockwork(int sec, int min, int hour, int Asec, int Amin, int Ahour)
{
   int alarm = 1;

   while(alarm == 1)
   {
        if (sec == 60)
        {
        	sec = 0;
        	min++;
        }
        if(min == 60)
        {
        	min = 0;
        	hour++;
        }
        if(hour == 24)
        	hour = 0;

        alarm = update_alarm(hour,min,sec,Ahour,Amin,Asec);

        printf("%d:%d:%d\n",hour,min,sec);
        sec++;
   }
   printf("ALARM");
}
int update_alarm(int hour, int min, int sec, int alarmhour, int alarmmin, int alarmsec)
{
    int alarmvalue = 1;
    if(hour == alarmhour && min == alarmmin && sec == alarmsec)
        alarmvalue = 0;

    //printf("alarmHour:  %d & Hour %d", alarmhour, hour);
    //printf("alarmMin:  %d & Min %d", alarmmin, min);
    //printf("alarmSec:  %d & Sec %d", alarmsec, sec);

    return alarmvalue;
}

