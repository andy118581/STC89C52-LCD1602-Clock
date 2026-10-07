#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"
void main()
{
	unsigned int count=1;
	unsigned int sec=4;
	unsigned int min=55;
	unsigned int hr=6;
	unsigned int day=7;
	unsigned int mon=10;
	unsigned int year=2026;
		
	LCD_Init();
	while(1)
	{		
		LCD_ShowNum(1,9,hr,2);
		LCD_ShowChar(1,11,':');
		LCD_ShowNum(1,12,min,2);
		LCD_ShowChar(1,14,':');
		LCD_ShowNum(1,15,sec,2);
		LCD_ShowNum(2,7,year,4);
		LCD_ShowChar(2,11,'/');
		LCD_ShowNum(2,12,mon,2);
		LCD_ShowChar(2,14,'/');
		LCD_ShowNum(2,15,day,2);
		Delay(1000);
		sec++;
		if(sec==60)
		{
			sec=0;
			min++;
		}
		if(min==60)
		{
			min=0;
			hr++;
		}
		if(hr==13)
		{
			hr=1;
			day++;
			count++;
		}
		if(count%2==0)
		{
			LCD_ShowString(1,1,"AM:");
		}else
		{
			LCD_ShowString(1,1,"PM:");
		}
		if(mon==1||mon==3||mon==5||mon==7||mon==8||mon==10||mon==12)
		{
			if(day==32)
			{
				day=1;
				mon++;
			}
		}else if(mon==4||mon==6||mon==9||mon==11)
		{
			if(day==31)
			{
				day=1;
				mon++;
			}
		}else if(mon==2&&((year%4==0&&year%100!=0)||(year%100==0&&year%400==0)))
		{
			if(day==30)
			{
				day=1;
				mon++;
			}
		}else if((mon==2&&!(year%4==0&&year%100!=0))||(mon==2&&!(year%100==0&&year%400==0)))
		{
			if(day==29)
			{
				day=1;
				mon++;
			}
		}
		if(mon==13)
		{
			mon=1;
			year++;
		}
	}
}