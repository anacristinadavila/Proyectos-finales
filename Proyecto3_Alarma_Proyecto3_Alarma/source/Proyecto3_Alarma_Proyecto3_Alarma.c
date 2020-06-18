
/**
 * @file    Proyecto3_Alarma_Proyecto3_Alarma.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_tpm.h"
#include "keypad.h"
#include "string.h"


void LCD_Data(uint8_t *high, uint8_t *low, uint8_t c);
void Write_Data(uint8_t data);
void LCD_CMD(uint8_t cmd);
void LCD_Write(uint8_t data);
void LCD_Clear();
void LCD_Return();
void LCD_Mode(uint8_t ID, uint8_t S);
void LCD_Set(uint8_t D, uint8_t C, uint8_t B);
void LCD_Cursor(uint8_t SC, uint8_t RL);
void LCD_Activate(uint8_t DL, uint8_t N, uint8_t F);
void LCD_CGRAM(uint8_t Address);
void LCD_DDRAM(uint8_t Address);
void Delay (uint32_t delay);
void DelayTPM();

#define DELAY 100000

/* TODO: insert other definitions and declarations here. */

#define BUZZER 7u/*PIN PTC7*/
#define LED_ROJO 0u/*PIN PTC0*/
#define LED_VERDE 3u/*PIN PTC3*/
#define LED_AMARILLO 3u/*PIN PTC4*/
#define SENSOR_1 5u/*PIN PTC5*/
#define SENSOR_2 6u/*PIN PTC6*/
#define SENSOR_3 10u/*PIN PTC10*/
#define SENSOR_4 11u/*PIN PTC11*/

/* TODO: insert other definitions and declarations here. */
typedef struct {
	uint8_t BUZZER1;
	uint8_t LEDROJO;
	uint8_t LEDVERDE;
	uint8_t LEDAMARILLO;
	uint16_t TIMER_MOD;
} state;



/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();



    tpm_config_t config;
    TPM_GetDefaultConfig (&config);
    config.prescale= kTPM_Prescale_Divide_128;
    TPM_Init (TPM0, &config);
    TPM_Init (TPM1, &config);

    keypad k;
    keypad_config config1;

    get_default_keypad_config (&config1);

    set_keypad_config(&k, &config1);

    char key=0, before;
    char password [12]={'A','C','A','B','A','D','A','1','3','7','9',0};
    char DATA_IN [12]={'0','0','0','0','0','0','0','0','0','0','0',0};
    char reset [12]={'0','0','0','0','0','0','0','0','0','0','0',0};

    int j=0;

    state FSM[9];

    	FSM[0]=(state){.LEDROJO=0u, .LEDVERDE=1u, .LEDAMARILLO=0u, .BUZZER1=0u};/*ALARMA DESACTIVADA*/
    	FSM[1]=(state){.LEDROJO=0u, .LEDVERDE=1u, .LEDAMARILLO=0u, .BUZZER1=0u};
    	FSM[2]=(state){.LEDROJO=0u, .LEDVERDE=1u, .LEDAMARILLO=0u, .BUZZER1=0u};
    	FSM[3]=(state){.LEDROJO=0u, .LEDVERDE=0u, .LEDAMARILLO=1u, .BUZZER1=0u, .TIMER_MOD=5120u};/*ALARMA ACTIVADA por 20 seg*/
    	FSM[4]=(state){.LEDROJO=0u, .LEDVERDE=0u, .LEDAMARILLO=1u, .BUZZER1=0u};/*ALARMA DESACTIVADA*/
    	FSM[5]=(state){.LEDROJO=1u, .LEDVERDE=0u, .LEDAMARILLO=0u, .BUZZER1=1u};/*ALARMA ACTIVADA*/
    	FSM[6]=(state){.LEDROJO=1u, .LEDVERDE=0u, .LEDAMARILLO=0u, .BUZZER1=1u};/*ALARMA ACTIVADA*/
    	FSM[7]=(state){.LEDROJO=1u, .LEDVERDE=0u, .LEDAMARILLO=0u, .BUZZER1=1u};/*ALARMA ACTIVADA*/
    	FSM[8]=(state){.LEDROJO=0u, .LEDVERDE=1u, .LEDAMARILLO=0u, .BUZZER1=0u};/*ALARMA DESACTIVADA*/


    	uint8_t message=0;
    	uint8_t SENSOR1;
    	uint8_t SENSOR2;
    	uint8_t SENSOR3;
    	uint8_t SENSOR4;
    	uint8_t estados=0;
    	uint16_t TIMER_ON=0;
    	uint32_t TIMER_FLAG;

    	while(1){
    		GPIO_WritePinOutput(GPIOC, LED_ROJO, FSM[estados] .LEDROJO);
    		GPIO_WritePinOutput(GPIOC, LED_VERDE, FSM[estados] .LEDVERDE);
    		GPIO_WritePinOutput(GPIOC, LED_AMARILLO, FSM[estados] .LEDAMARILLO);
    		GPIO_WritePinOutput(GPIOC, BUZZER, FSM[estados] .BUZZER1);


    		SENSOR1=GPIO_ReadPinInput(GPIOC, SENSOR_1);
    		SENSOR2=GPIO_ReadPinInput(GPIOC, SENSOR_2);
    		SENSOR3=GPIO_ReadPinInput(GPIOC, SENSOR_3);
    		SENSOR4=GPIO_ReadPinInput(GPIOC, SENSOR_4);


    		TIMER_FLAG=TPM_GetStatusFlags(TPM0);

    		before=key;
    		key=read_keypad(&k);

    		switch(estados){

    		case 0:
    			if(message==0){
    				LCD_Set (1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('A');
    				LCD_Write('L');
    				LCD_Write('A');
    				LCD_Write('R');
    				LCD_Write('M');
    				LCD_Write('A');
    				LCD_DDRAM(40u);
    				LCD_Write('D');
    				LCD_Write('E');
    				LCD_Write('S');
    				LCD_Write('A');
    				LCD_Write('C');
    				LCD_Write('T');
    				LCD_Write('I');
    				LCD_Write('V');
    				LCD_Write('A');
    				LCD_Write('D');
    				LCD_Write('A');
    				message=1;
    			}

    			if((key==0)&&(before!=0)){
    				key=before;
    				if(key=='*'){
    					message=0;
    					estados=1;
    				}


    				key=0;
    			}
    			break;


    		case 1:
    			if(message==0){
    				LCD_Set(1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('C');
    				LCD_Write('L');
    				LCD_Write('A');
    				LCD_Write('V');
    				LCD_Write('E');
    				LCD_DDRAM(40u);
    				message=1;
    			}
    			if((key==0)&&(before !=0)){
    				key=before;
    				if(key!='#'){
    					DATA_IN[j]=key;
    					LCD_Set(1u,1u,1u);
    					LCD_Write('*');
    					j++;
    					DelayTPM();
    				}
    			else if (key=='#'){
    				printf("\n");
    				message=0;
    				j=0;
    				estados=2;
    			}
    			key=0;
    			}

    			break;

    		case 2:
    			if(strcmp(password,DATA_IN)==0){
    				strcpy(DATA_IN,reset);
    				key=0;
    				estados=3;
    			}
    			else {
    				printf("\nERROR INTENTE DE NUEVO\n");
    				key=0;
    				strcpy(DATA_IN,reset);
    				estados=8;
    			}
    			break;

    		case 3:
    			if(message==0){
    				LCD_Set (1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('A');
    				LCD_Write('L');
    				LCD_Write('A');
    				LCD_Write('R');
    				LCD_Write('M');
    				LCD_Write('A');
    				LCD_DDRAM(40u);
    				LCD_Write('A');
    				LCD_Write('R');
    				LCD_Write('M');
    				LCD_Write('A');
    				LCD_Write('D');
    				LCD_Write('A');
    				message=1;
    			}
    			if(TIMER_ON==0){
    				TPM_SetTimerPeriod(TPM0, FSM[3] .TIMER_MOD);
    				TPM_StartTimer(TPM0, kTPM_SystemClock);
    				TIMER_ON=1;
    			}
    			if (TIMER_FLAG){
    				TPM_ClearStatusFlags(TPM0, 1u<8u);
    				TIMER_ON=0;
    				message=0;
    				TPM_StopTimer(TPM0);
    				TPM0->CNT=0;
    				estados=4;
    			}
    			break;


    		case 4:
    			if(message==0){
    				LCD_Set (1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('A');
    				LCD_Write('L');
    				LCD_Write('A');
    				LCD_Write('R');
    				LCD_Write('M');
    				LCD_Write('A');
    				LCD_Write('-');
    				LCD_Write('A');
    				LCD_Write('R');
    				LCD_Write('M');
    				LCD_Write('A');
    				LCD_Write('D');
    				LCD_Write('A');
    				LCD_Write('-');
    				LCD_Write('>');
    				LCD_DDRAM(40u);
    				LCD_Write('S');
    				LCD_Write('E');
    				LCD_Write('N');
    				LCD_Write('S');
    				LCD_Write('O');
    				LCD_Write('R');
    				LCD_Write('E');
    				LCD_Write('S');
    				LCD_Write('-');
    				LCD_Write('A');
    				LCD_Write('C');
    				LCD_Write('T');
    				LCD_Write('I');
    				LCD_Write('V');
    				LCD_Write('O');
    				LCD_Write('S');
    				message=1;
    			}
    			if(SENSOR1==0){
    				message=0;
    				estados=5;
    			}
    			if (SENSOR2==0){
    				message=0;
    				estados=5;
    			}
    			if (SENSOR3==0){
    				message=0;
    				estados=5;
    			}
    			if (SENSOR4==0){
    				message=0;
    				estados=5;
    			}
    			break;


    		case 5:
    			if(SENSOR1==0){
    				LCD_Set (1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('S');
    				LCD_Write('E');
    				LCD_Write('N');
    				LCD_Write('S');
    				LCD_Write('O');
    				LCD_Write('R');
    				LCD_Write('1');
    				LCD_Write('-');
    				LCD_Write('A');
    				LCD_Write('C');
    				LCD_Write('T');
    				LCD_Write('I');
    				LCD_Write('V');
    				LCD_Write('A');
    				LCD_Write('D');
    				LCD_Write('O');
    				LCD_DDRAM(40u);
    				LCD_Write('I');
    				LCD_Write('N');
    				LCD_Write('G');
    				LCD_Write('R');
    				LCD_Write('E');
       				LCD_Write('S');
    				LCD_Write('A');
      				LCD_Write('R');
    				LCD_Write('-');
    				LCD_Write('C');
       				LCD_Write('L');
     				LCD_Write('A');
     				LCD_Write('V');
    				LCD_Write('E');

    			}
    			if(SENSOR2==0){
    				LCD_Set (1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('S');
    				LCD_Write('E');
    				LCD_Write('N');
    				LCD_Write('S');
    				LCD_Write('O');
    				LCD_Write('R');
    				LCD_Write('2');
    				LCD_Write('-');
    				LCD_Write('A');
    				LCD_Write('C');
    				LCD_Write('T');
    				LCD_Write('I');
    				LCD_Write('V');
    				LCD_Write('A');
    				LCD_Write('D');
    				LCD_Write('O');
    				LCD_DDRAM(40u);
    				LCD_Write('I');
    				LCD_Write('N');
    				LCD_Write('G');
    				LCD_Write('R');
    				LCD_Write('E');
    				LCD_Write('S');
    				LCD_Write('A');
    				LCD_Write('R');
    				LCD_Write('-');
    				LCD_Write('C');
       				LCD_Write('L');
       				LCD_Write('A');
    				LCD_Write('V');
    				LCD_Write('E');

    			}

    			if(SENSOR3==0){
    				LCD_Set (1u,1u,1u);
    			   	LCD_Activate(1u,1u,0u);
    			   	LCD_Clear();
    			   	LCD_Write('S');
    			   	LCD_Write('E');
    				LCD_Write('N');
    				LCD_Write('S');
    				LCD_Write('O');
    				LCD_Write('R');
    				LCD_Write('3');
    				LCD_Write('-');
    				LCD_Write('A');
    				LCD_Write('C');
    				LCD_Write('T');
    				LCD_Write('I');
    				LCD_Write('V');
    				LCD_Write('A');
    				LCD_Write('D');
    				LCD_Write('O');
    				LCD_DDRAM(40u);
    				LCD_Write('I');
    				LCD_Write('N');
    				LCD_Write('G');
    				LCD_Write('R');
    				LCD_Write('E');
       				LCD_Write('S');
       				LCD_Write('A');
      				LCD_Write('R');
      				LCD_Write('-');
    				LCD_Write('C');
    				LCD_Write('L');
     				LCD_Write('A');
     				LCD_Write('V');
    				LCD_Write('E');

    			}
    			if(SENSOR4==0){
    			    LCD_Set (1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('S');
    				LCD_Write('E');
    				LCD_Write('N');
    				LCD_Write('S');
    				LCD_Write('O');
    				LCD_Write('R');
    				LCD_Write('4');
    				LCD_Write('-');
    				LCD_Write('A');
    				LCD_Write('C');
    				LCD_Write('T');
    				LCD_Write('I');
    				LCD_Write('V');
    				LCD_Write('A');
    				LCD_Write('D');
    				LCD_Write('O');
    				LCD_DDRAM(40u);
    				LCD_Write('I');
    				LCD_Write('N');
    				LCD_Write('G');
    				LCD_Write('R');
    				LCD_Write('E');
       				LCD_Write('S');
       				LCD_Write('A');
      				LCD_Write('R');
      				LCD_Write('-');
      				LCD_Write('C');
       				LCD_Write('L');
       				LCD_Write('A');
     				LCD_Write('V');
     				LCD_Write('E');

    			}
    			if ((key==0)&&(before!=0)){
    				key=before;
    				if (key=='#'){
    					key=0;
    					estados=6;
    				}
    				key=0;
    			}
    			break;


    		case 6:
    			if(message==0){
    				LCD_Set(1u,1u,1u);
    				LCD_Clear();
    				LCD_Write('C');
    				LCD_Write('L');
    				LCD_Write('A');
    				LCD_Write('V');
    				LCD_Write('E');
    				key=0;
    				message=1;
    			}
    			if ((key==0)&&(before!=0)){
    				key=before;
    				if (key!='#'){
    					DATA_IN[j]=key;
    					LCD_Set(1u,1u,1u);
    					LCD_Write('*');
    					j++;
    					DelayTPM();
    				}
    				else {
    					printf("\n");
    					message=0;
    					j=0;
    					estados=7;
    					key=0;
    				}
    				key=0;
    			}
    			break;


    		case 7:
    			if(strcmp(password,DATA_IN)==0){
    				printf("\nCLAVE CORRECTA\n");
    				strcpy(DATA_IN,reset);
    				SENSOR1=1;
    				SENSOR2=1;
    				SENSOR3=1;
    				SENSOR4=1;
    				key=0;
    				estados=0;
    			}
    			else {
    				printf("\nCLAVE INCORRECTA\n");
    				strcpy(DATA_IN,reset);
    				key=0;
    				estados=6;
    			}
    			break;

    		case 8:
    			if(message==0){
    				LCD_Set(1u,1u,1u);
    				LCD_Activate(1u,1u,0u);
    				LCD_Clear();
    				LCD_Write('C');
    				LCD_Write('L');
    				LCD_Write('A');
    				LCD_Write('V');
    				LCD_Write('E');
    				LCD_Write('-');
    				LCD_Write('I');
    				LCD_Write('N');
    				LCD_Write('C');
    				LCD_Write('O');
    				LCD_Write('R');
    				LCD_Write('R');
    				LCD_Write('E');
    				LCD_Write('C');
    				LCD_Write('T');
    				LCD_Write('A');
    				message=1;
    			}
    			if ((key==0)&&(before!=0)){
    				key=before;
    				if (key=='#'){
    					message=0;
    					estados=1;
    				}
    				key=0;
    			}
    			break;






    		}


    	}


    return 0 ;
}
void LCD_Data(uint8_t *high, uint8_t *low, uint8_t c)
{
  *high=(c & 0xF0) >> 4;
  *low= c & 0x0F;
}
void Write_Data(uint8_t data)
{

	for (uint8_t i=0;i<4;i++) //low
	{
		if(data & (1u<<i))
		{
			GPIO_SetPinsOutput(GPIOB, 1u<<i);
		}
		else
		{
			GPIO_ClearPinsOutput(GPIOB, 1u<<i);
		}
	}


	for (uint8_t i=0;i<4;i++) //High
	{
		if(data & (1u<<(i+4)))
		{
			GPIO_SetPinsOutput(GPIOE, 1u<<(i+20));
		}
		else
		{
			GPIO_ClearPinsOutput(GPIOE, 1u<<(i+20));
		}
	}
}

void LCD_CMD(uint8_t cmd)
{
	//Pone LCD_RS en bajo
	GPIO_ClearPinsOutput(GPIOC, 1u<<BOARD_INITPINS_LCD_RS_PIN);
	//Pone LCD_RW en bajo
	GPIO_ClearPinsOutput(GPIOC, 1u<<BOARD_INITPINS_LCD_RW_PIN);
	//Pone LCD_E en alto
	GPIO_SetPinsOutput(GPIOE, 1u<<BOARD_INITPINS_LCD_E_PIN);
	Write_Data(cmd);
	//Pone LCD_E en bajo
	GPIO_ClearPinsOutput(GPIOE, 1u<<BOARD_INITPINS_LCD_E_PIN);
	Delay(DELAY);

}
void LCD_Write(uint8_t data)
{
	//Pone LCD_RS en alto
	GPIO_SetPinsOutput(GPIOC, 1u<<BOARD_INITPINS_LCD_RS_PIN);
	//Pone LCD_RW en bajo
	GPIO_ClearPinsOutput(GPIOC, 1u<<BOARD_INITPINS_LCD_RW_PIN);
	//Pone LCD_E en alto
	GPIO_SetPinsOutput(GPIOE, 1u<<BOARD_INITPINS_LCD_E_PIN);
	Write_Data(data);
	//Pone LCD_E en bajo
	GPIO_ClearPinsOutput(GPIOE, 1u<<BOARD_INITPINS_LCD_E_PIN);
	Delay(DELAY);
}
void LCD_Clear()
{
	uint8_t cmd=0x01u;
	LCD_CMD(cmd);
}
void LCD_Return()
{
	uint8_t cmd=0x02u;
	LCD_CMD(cmd);
}
void LCD_Mode(uint8_t ID, uint8_t S)
{
	uint8_t cmd=4u;
	if(ID)
		cmd |=2u;
	if(S)
		cmd |=1u;
	LCD_CMD(cmd);
}
void LCD_Set(uint8_t D, uint8_t C, uint8_t B)
{
	uint8_t cmd=8u;
	if(D)
		cmd |= 4u;
	if(C)
		cmd |= 2u;
	if(B)
		cmd |= 1u;
	LCD_CMD(cmd);
}
void LCD_Cursor(uint8_t SC, uint8_t RL)
{
	uint8_t cmd=16u;
	if(SC)
		cmd |=8u;
	if(RL)
		cmd |=4u;
	LCD_CMD(cmd);
}
void LCD_Activate(uint8_t DL, uint8_t N, uint8_t F)
{
	uint8_t cmd=32u;
	if(DL)
		cmd |=16u;
	if(N)
		cmd |=8u;
	if(F)
		cmd |=4u;
	LCD_CMD(cmd);
}
void LCD_CGRAM(uint8_t Address)
{
	uint8_t cmd = 0x40u;
	cmd |= Address & 0x3Fu;
	LCD_CMD(cmd);
}
void LCD_DDRAM(uint8_t Address)
{
	uint8_t cmd = 0x80u;
	cmd |= Address & 0x7Fu;
	LCD_CMD(cmd);
}
void Delay(uint32_t delay)
{
	for(uint32_t i=0;i<delay;i++)
		__asm("NOP");
}

void DelayTPM(){
	uint32_t Mask=1u<<8u;
	uint32_t Mask_off=Mask;

	TPM_SetTimerPeriod(TPM1, 100u);
	TPM_StartTimer(TPM1, kTPM_SystemClock);
	while(!(TPM1->STATUS & Mask)){
	}
	if (TPM1->STATUS & Mask){
		TPM1->STATUS &= Mask_off;
		TPM_StopTimer(TPM1);
		TPM1->CNT=0;
	}
}
