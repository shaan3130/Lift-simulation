//Header files
#include <at89c5131.h>
#include <stdio.h>	//for sprintf functions
#include <stdlib.h>

//Bit definitions
sbit RS=P0^0;
sbit RW=0x81;	//Also can use P0^1 or 0x80^1
sbit EN=P0^2;

//Global variables

//LCD functions
void lcd_init(void);
void lcd_cmd(unsigned int i);
void lcd_char(unsigned char ch);
void lcd_write_string(unsigned char *s);
void port_init(void);


//Delay function for time*1ms
void msdelay(unsigned int time)
{
	int i,j;
	for(i=0;i<time;i++)
	{
		for(j=0;j<318;j++);
	}
}
//LCD utility functions
void lcd_cmd(unsigned int i)
{
	RS=0;
	RW=0;
	EN=1;
	P2=i;
	msdelay(10);
	EN=0;
}
void lcd_char(unsigned char ch)
{
	RS=1;
	RW=0;
	EN=1;
	P2=ch;
	msdelay(10);
	EN=0;
}
void lcd_write_string(unsigned char *s)
{
	while(*s!='\0')		//Can use while(*s)
	{
		lcd_char(*s++);
	}
}
void lcd_init(void) using 3
{
	lcd_cmd(0x38);
	msdelay(4);
	lcd_cmd(0x06);
	msdelay(4);
	lcd_cmd(0x0C);
	msdelay(4);
	lcd_cmd(0x01);
	msdelay(4);
}

//Port initialization
void port_init(void)
{
	P2=0x00;
	EN=0;
	RS=0;
	RW=0;
}

short sgn(short v){
	if(v < 0) return -1;
	if(v > 0) return 1;
	return 0;
}

//Main function
int main(void)
{
	unsigned char floor_num1[16];
	unsigned char floor_num2[16];
	short curr_floor1 = 0;   //current floor for lift 1
	short curr_floor2 = 0;   //current floor for lift 2
	short init_pos = 0;      //initial position of switch after each loop
	short inp = 0;             // input from switch
	unsigned char prev_p0;
	port_init();
	lcd_init();
	P1 =0x00;
	sprintf(floor_num1,"Floor 1 = 0%d",0);
	lcd_cmd(0x80);
	lcd_write_string(floor_num1);
	sprintf(floor_num2,"Floor 2 = 0%d",0);
	lcd_cmd(0xC0);
	lcd_write_string(floor_num2);
	msdelay(500); 
	P1 |= 0x0F;															//port reading enabled
	prev_p0 = P1&0x01;
	while(1)
	{
		while(P1_0==prev_p0){P1_5=1;}     //toggle P1_0 to tell the lift that a user is going to give its location
		prev_p0 = (~prev_p0)&0x01;
		while(P1_0==prev_p0){P1_5=0;}
		P1_7 = 1; 														//ready to take input
		msdelay(10000);													//give 10 sec time to user to give whole input
		inp = P1&0x0F;   									    //current postion of switches 	
		P1_7 = 0;                               //now lift action processing
		
		//if lift 1 is closer
		if(abs(curr_floor1 - inp) <= abs(curr_floor2 - inp)){
			short iter;
			short diff = abs(curr_floor1 - inp);
			for(iter=1;iter<=diff;iter++){
				msdelay(1000);
				curr_floor1 = curr_floor1 + sgn(inp - curr_floor1);   //increase or decrease the current floor with 1sec delay
				if (curr_floor1 < 10){
					sprintf(floor_num1," 0%d",curr_floor1);
					lcd_cmd(0x89);
					lcd_write_string(floor_num1);
				}
				else{
					sprintf(floor_num1," %d",curr_floor1);
					lcd_cmd(0x89);
					lcd_write_string(floor_num1);
				}
			}
			init_pos = P1&0x0F;  								//initial postion of the switch
			inp = init_pos; 
			P1_4 = 1;
			while(inp==init_pos){
				inp = P1&0x0F;    				//current postion of switches
				msdelay(5000);													//give time to user to give whole input
				P1_4 = 0;
				msdelay(1000);
				P1_4 = 1; 
			}
			P1_4 = 0;                       //now lift action processing
			diff = abs(curr_floor1 - inp);
			for(iter=1; iter<=diff; iter++){
				msdelay(1000);
				curr_floor1 = curr_floor1 + sgn(inp - curr_floor1);   //increase or decrease the current floor with 1sec delay
				if (curr_floor1 < 10){
					sprintf(floor_num1," 0%d",curr_floor1);
					lcd_cmd(0x89);
					lcd_write_string(floor_num1);
				}
				else{
					sprintf(floor_num1," %d",curr_floor1);
					lcd_cmd(0x89);
					lcd_write_string(floor_num1);
				}
			}
		}
		
		//if lift 2 is closer
		else{
			short iter;
			short diff = abs(curr_floor2 - inp);
			for(iter=1; iter<=diff; iter++){
				msdelay(1000);
				curr_floor2 = curr_floor2 + sgn(inp - curr_floor2);   //increase or decrease the current floor with 1sec delay
				if (curr_floor2 < 10){
					sprintf(floor_num2," 0%d",curr_floor2);
					lcd_cmd(0xC9);
					lcd_write_string(floor_num2);
				}
				else{
					sprintf(floor_num2," %d",curr_floor2);
					lcd_cmd(0xC9);
					lcd_write_string(floor_num2);
				}
			}
			init_pos = P1&0x0F;  								//initial postion of the switch
			inp = init_pos;
			P1_4 = 1;
			while(inp==init_pos){
				inp = P1&0x0F;                  //current postion of switches
				msdelay(5000);													//give time to user to give whole input
				P1_4 = 0;
				msdelay(1000);
				P1_4 = 1; 
			}
			P1_4 = 0;                           //now lift action processing
			diff = abs(curr_floor2 - inp);
			for(iter=1; iter<=diff; iter++){
				msdelay(1000);
				curr_floor2 = curr_floor2 + sgn(inp - curr_floor2);   //increase or decrease the current floor with 1sec delay
				if (curr_floor2 < 10){
					sprintf(floor_num2," 0%d",curr_floor2);
					lcd_cmd(0xC9);
					lcd_write_string(floor_num2);
				}
				else{
					sprintf(floor_num2," %d",curr_floor2);
					lcd_cmd(0xC9);
					lcd_write_string(floor_num2);
				}
			}
		}
		prev_p0 = P1_0;           //store the current position of switch so as toggle for next iteration
	}
}