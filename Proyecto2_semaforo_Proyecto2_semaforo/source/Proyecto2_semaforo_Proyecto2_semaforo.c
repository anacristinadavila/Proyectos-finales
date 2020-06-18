
/**
 * @file    Proyecto2_semaforo_Proyecto2_semaforo.c
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

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

#define N_Rojo 2u /* PTB2 */
#define N_Amarillo 1u /* PTB1 */
#define N_Verde 0u /* PTB0 */
#define N_Vuelta 3u /* PTB3 */
#define N_Sensor 2u /* PTC2 */

#define S_Rojo 22u /* PTE22 */
#define S_Amarillo 21u /* PTE21 */
#define S_Verde 20u /* PTE20 */
#define S_Vuelta 23u /* PTE23 */
#define S_Sensor 29u /* PTE29 */

#define E_Rojo 3u /* PTC3 */
#define E_Amarillo 4u /* PTC4 */
#define E_Verde 5u /* PTC5 */
#define E_Vuelta 0u /* PTC0 */
#define E_Sensor 7u /* PTC7 */

#define O_Rojo 4U /* PTD4 */
#define O_Amarillo 12u /* PTA12 */
#define O_Verde 4u /* PTA4 */
#define O_Vuelta 2u /* PTA2 */
#define O_Sensor 1u /* PTA1 */

typedef struct{
	uint8_t NROJO;
	uint8_t NAMARILLO;
	uint8_t NVERDE;
	uint8_t NVUELTA;
	uint8_t SROJO;
	uint8_t SAMARILLO;
	uint8_t SVERDE;
	uint8_t SVUELTA;
	uint8_t EROJO;
	uint8_t EAMARILLO;
	uint8_t EVERDE;
	uint8_t EVUELTA;
	uint8_t OROJO;
	uint8_t OAMARILLO;
	uint8_t OVERDE;
	uint8_t OVUELTA;
	uint16_t TIMER_MOD;

} state;
/*
 * @brief   Application entry point.
 */
int main(void) {

	int i;

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

    state FSM [23];

    FSM[0]=(state){.NROJO=0u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=256u}; /*Extra*/

    FSM[1]=(state){.NROJO=0u, .NAMARILLO=0u, .NVERDE=1u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=0u, .SVERDE=1u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=2040u}; /*N y S en VERDE por 8 s*/
    FSM[2]=(state){.NROJO=0u, .NAMARILLO=0u, .NVERDE=1u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=0u, .SVERDE=1u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=1792u}; /*N y S en VERDE por 7 s*/
    FSM[3]=(state){.NROJO=0u, .NAMARILLO=1u, .NVERDE=0u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=1u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*N y S en amarillo por 2 s*/

    FSM[4]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=0u, .EVERDE=1u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=0u, .OVERDE=1u, .OVUELTA=0u, .TIMER_MOD=2040u}; /*E y O en VERDE por 8 s*/
    FSM[5]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=0u, .EVERDE=1u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=0u, .OVERDE=1u, .OVUELTA=0u, .TIMER_MOD=1792u}; /*E y O en VERDE por 7 s*/
    FSM[6]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=1u, .EVERDE=0u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=1u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*E y O en amarillo por 2 s*/

    FSM[7]=(state){.NROJO=0u, .NAMARILLO=0u, .NVERDE=1u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=1u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*SENSOR de N activo 2 s y S en amarillo*/
    FSM[8]=(state){.NROJO=0u, .NAMARILLO=0u, .NVERDE=1u, .NVUELTA=1u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=1280u}; /*VERDE de vuelta de N se enciende 5 s*/
    FSM[9]=(state){.NROJO=0u, .NAMARILLO=1u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*N cambia a amarillo por 2 s*/

    FSM[10]=(state){.NROJO=0u, .NAMARILLO=1u, .NVERDE=0u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=0u, .SVERDE=1u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*SENSOR de S activo 2 s y N en amarillo */
    FSM[11]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=0u, .SVERDE=1u, .SVUELTA=1u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=1792u}; /*VERDE de vuelta de S se enciende 5 s*/
    FSM[12]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=1u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*S cambia a amarillo por 2 s*/

    FSM[13]=(state){.NROJO=0u, .NAMARILLO=1u, .NVERDE=0u, .NVUELTA=0u, .SROJO=0u, .SAMARILLO=1u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*N y S cambian a amarillo para vueltas*/
    FSM[14]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=1u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=1u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=1792u}; /*Las vueltas de N y S estan en verde*/

    FSM[15]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=0u, .EVERDE=1u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=1u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*SENSOR de E activo 2 s y O en amarillo */
    FSM[16]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=0u, .EVERDE=1u, .EVUELTA=1u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=1280u}; /*VERDE de vuellta de E se enciende 5 s*/
    FSM[17]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=1u, .EVERDE=0u, .EVUELTA=0u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*E cambia a amarillo por 2 s*/

    FSM[18]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=1u, .EVERDE=0u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=0u, .OVERDE=1u, .OVUELTA=0u, .TIMER_MOD=512u}; /*SENSOR de O activo 2 s y E en amarillo*/
    FSM[19]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=0u, .OVERDE=1u, .OVUELTA=1u, .TIMER_MOD=1280u}; /*VERDE de vuelta de O se enciende 5 s*/
    FSM[20]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=1u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*O cambia a amarillo por 2 s*/

    FSM[21]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=0u, .EAMARILLO=1u, .EVERDE=0u, .EVUELTA=0u, .OROJO=0u, .OAMARILLO=1u, .OVERDE=0u, .OVUELTA=0u, .TIMER_MOD=512u}; /*E y O cambian a a amarillo para las vueltas*/
    FSM[22]=(state){.NROJO=1u, .NAMARILLO=0u, .NVERDE=0u, .NVUELTA=0u, .SROJO=1u, .SAMARILLO=0u, .SVERDE=0u, .SVUELTA=0u, .EROJO=1u, .EAMARILLO=0u, .EVERDE=0u, .EVUELTA=1u, .OROJO=1u, .OAMARILLO=0u, .OVERDE=0u, .OVUELTA=1u, .TIMER_MOD=1792u}; /*Las vueltas de E y O estan en verde*/


    uint8_t SensorN_REG=1;
    uint8_t SensorS_REG=1;
    uint8_t SensorE_REG=1;
    uint8_t SensorO_REG=1;
    uint8_t SensorN=0;
    uint8_t SensorS=0;
    uint8_t SensorE=0;
    uint8_t SensorO=0;
    uint8_t estados=0;
    uint8_t TIMER_ON=0;
    uint32_t TIMER_FLAG;
    uint32_t MASK=1u<<8u;
    uint32_t MASK_OFF = MASK;


    while (1) {
    	GPIO_WritePinOutput (GPIOB, N_Rojo, FSM[estados] .NROJO);
    	GPIO_WritePinOutput (GPIOB, N_Amarillo, FSM[estados] .NAMARILLO);
    	GPIO_WritePinOutput (GPIOB, N_Verde, FSM[estados] .NVERDE);
    	GPIO_WritePinOutput (GPIOB, N_Vuelta, FSM[estados] .NVUELTA);
    	GPIO_WritePinOutput (GPIOE, S_Rojo, FSM[estados] .SROJO);
    	GPIO_WritePinOutput (GPIOE, S_Amarillo, FSM[estados] .SAMARILLO);
    	GPIO_WritePinOutput (GPIOE, S_Verde, FSM[estados] .SVERDE);
    	GPIO_WritePinOutput (GPIOE, S_Vuelta, FSM[estados] .SVUELTA);
    	GPIO_WritePinOutput (GPIOC, E_Rojo, FSM[estados] .EROJO);
    	GPIO_WritePinOutput (GPIOC, E_Amarillo, FSM[estados] .EAMARILLO);
    	GPIO_WritePinOutput (GPIOC, E_Verde, FSM[estados] .EVERDE);
    	GPIO_WritePinOutput (GPIOC, E_Vuelta, FSM[estados] .EVUELTA);
    	GPIO_WritePinOutput (GPIOD, O_Rojo, FSM[estados] .OROJO);
    	GPIO_WritePinOutput (GPIOA, O_Amarillo, FSM[estados] .OAMARILLO);
    	GPIO_WritePinOutput (GPIOA, O_Verde, FSM[estados] .OVERDE);
    	GPIO_WritePinOutput (GPIOA, O_Vuelta, FSM[estados] .OVUELTA);

    	SensorN = GPIO_ReadPinInput (GPIOC, N_Sensor);
    	SensorS = GPIO_ReadPinInput (GPIOE, S_Sensor);
    	SensorE = GPIO_ReadPinInput (GPIOC, E_Sensor);
    	SensorO = GPIO_ReadPinInput (GPIOA, O_Sensor);

    	if (SensorN == 0){
    		   SensorN_REG = 0;
    	}
    	if (SensorS == 0){
    		   SensorS_REG = 0;
    	}

    	if (SensorE == 0){
    	    		   SensorE_REG = 0;
    	}

    	if (SensorO == 0){
    	    		   SensorO_REG = 0;
    	}

    	TIMER_FLAG = TPM_GetStatusFlags (TPM0);

    	switch (estados){

    	case 0:

    		TPM_SetTimerPeriod (TPM1, FSM[0] .TIMER_MOD);
    		TPM_StartTimer (TPM1, kTPM_SystemClock);

    		for (i=0; i<=9; i++){
    			GPIO_TogglePinsOutput (GPIOB, 1u<<1u);
    			GPIO_TogglePinsOutput (GPIOE, 1u<<21u);
    			GPIO_TogglePinsOutput (GPIOA, 1u<<12u);
    			GPIO_TogglePinsOutput (GPIOC, 1u<<4u);

    			do{

    			}while (!(TPM1 ->STATUS & MASK));
    			if (TPM1 -> STATUS & MASK){
    				TPM1 -> STATUS &= MASK_OFF;
    				TPM1 -> CNT=0;
    			}
    		}
    		TPM1 -> STATUS &= MASK_OFF;
    		TPM1 -> CNT=0;
    		TPM_StopTimer(TPM1);
    		estados=1;

    	case 1:
    		if (TIMER_ON == 0){
    			   TIMER_ON = 1;
    			   TPM_SetTimerPeriod (TPM0, FSM[1] .TIMER_MOD);
    			   TPM_StartTimer (TPM0, kTPM_SystemClock);
    		}
    		if(TIMER_FLAG){
    			TPM_ClearStatusFlags (TPM0, 1u<8u);
    			TIMER_ON=0;
    			TPM_StopTimer(TPM0);
    			TPM0 -> CNT=0;

    			if((SensorN_REG==0)&& (SensorS_REG==1)){
    				SensorN_REG=1;
    				SensorS_REG=1;
    				estados=7;
    			}
    			else if((SensorS_REG==0)&&(SensorN_REG==1)){
    				SensorN_REG=1;
    			    SensorS_REG=1;
    				estados=10;
    			}
    			else if((SensorS_REG==0)&&(SensorN_REG==0)){
    				SensorN_REG=1;
    			    SensorS_REG=1;
    				estados=13;
    			}
    			else{
    				estados=2;
    			}

    		}
    		break;

    	case 2:
    		if (TIMER_ON==0){
    			TIMER_ON = 1;
    			TPM_SetTimerPeriod (TPM0, FSM[2] .TIMER_MOD);
    			TPM_StartTimer (TPM0, kTPM_SystemClock);
    		}
    		if (TIMER_FLAG){
    			TPM_ClearStatusFlags (TPM0, 1u<8u);
    			TIMER_ON=0;
    			TPM_StopTimer(TPM0);
    			TPM0 -> CNT=0;
    			estados=3;
    		}
    		break;

    	case 3:
    		if (TIMER_ON==0){
    			TIMER_ON = 1;
    		    TPM_SetTimerPeriod (TPM0, FSM[3] .TIMER_MOD);
    		    TPM_StartTimer (TPM0, kTPM_SystemClock);
    		}
    		if (TIMER_FLAG){
    			TPM_ClearStatusFlags (TPM0, 1u<8u);
    			TIMER_ON=0;
    		    TPM_StopTimer(TPM0);
    		    TPM0 -> CNT=0;
    		    estados=4;
    		}
    		break;

    	case 4:
    		if (TIMER_ON==0){
    		    		    TIMER_ON = 1;
    		    		    TPM_SetTimerPeriod (TPM0, FSM[4] .TIMER_MOD);
    		    		    TPM_StartTimer (TPM0, kTPM_SystemClock);
    		    		}
    		    		if (TIMER_FLAG){
    		    			TPM_ClearStatusFlags (TPM0, 1u<8u);
    		    		    TIMER_ON=0;
    		    		    TPM_StopTimer(TPM0);
    		    		    TPM0 -> CNT=0;

    		    		    if((SensorE_REG==0)&&(SensorO_REG==1)){
    		    		    	SensorE_REG=1;
    		    		    	SensorO_REG=1;
    		    		    	estados=15;
    		    		    }
    		    		    else if ((SensorO_REG==0)&&(SensorE_REG==1)){
    		    		    	SensorE_REG=1;
    		    		    	SensorO_REG=1;
    		    		    	estados=18;
    		    		    }
    		    		    else if ((SensorO_REG==0)&&(SensorE_REG==0)){
    		    		        SensorE_REG=1;
    		    		        SensorO_REG=1;
    		    		        estados=21;
    		    		    }
    		    		    else{
    		    		    	estados=5;
    		    		    }
    		    		 }
    		    		 break;
    	case 5:
    		if (TIMER_ON==0){
    			TIMER_ON = 1;
    		    TPM_SetTimerPeriod (TPM0, FSM[5] .TIMER_MOD);
    		    TPM_StartTimer (TPM0, kTPM_SystemClock);
    		}
    		if (TIMER_FLAG){
    		    TPM_ClearStatusFlags (TPM0, 1u<8u);
    		    TIMER_ON=0;
    		    TPM_StopTimer(TPM0);
    		    TPM0 -> CNT=0;
    		    estados=6;
    		}
    		break;

    	case 6:
    	    if (TIMER_ON==0){
    	    	TIMER_ON = 1;
    	    	TPM_SetTimerPeriod (TPM0, FSM[6] .TIMER_MOD);
    	    	TPM_StartTimer (TPM0, kTPM_SystemClock);
    	    }
    	    if (TIMER_FLAG){
    	    	TPM_ClearStatusFlags (TPM0, 1u<8u);
    	    	TIMER_ON=0;
    	    	TPM_StopTimer(TPM0);
    	    	TPM0 -> CNT=0;
    	    	estados=1;
    	    }
    	    break;

    	case 7:
    		if (TIMER_ON==0){
    	    	TIMER_ON = 1;
    	    	TPM_SetTimerPeriod (TPM0, FSM[7] .TIMER_MOD);
    	    	TPM_StartTimer (TPM0, kTPM_SystemClock);
    		}
    		if (TIMER_FLAG){
    	    	TPM_ClearStatusFlags (TPM0, 1u<8u);
    	    	TIMER_ON=0;
    	    	TPM_StopTimer(TPM0);
    	    	TPM0 -> CNT=0;
    	    	estados=8;
    	    }
    		break;

    	case 8:
    		if (TIMER_ON==0){
    	    	TIMER_ON = 1;
    	    	TPM_SetTimerPeriod (TPM0, FSM[8] .TIMER_MOD);
    	    	TPM_StartTimer (TPM0, kTPM_SystemClock);
    	    }
    		if (TIMER_FLAG){
    	    	TPM_ClearStatusFlags (TPM0, 1u<8u);
    	    	TIMER_ON=0;
    	    	TPM_StopTimer(TPM0);
    	    	TPM0 -> CNT=0;
    	    	estados=9;
    	    }
    		break;

    	case 9:
    	    if (TIMER_ON==0){
    	    	TIMER_ON = 1;
    	    	TPM_SetTimerPeriod (TPM0, FSM[9] .TIMER_MOD);
    	    	TPM_StartTimer (TPM0, kTPM_SystemClock);
    	    }
    	    if (TIMER_FLAG){
    	    	TPM_ClearStatusFlags (TPM0, 1u<8u);
    	    	TIMER_ON=0;
    	    	TPM_StopTimer(TPM0);
    	    	TPM0 -> CNT=0;
    	    	estados=4;
    	    }
    	    break;

    	 case 10:
    	     if (TIMER_ON==0){
    	    	 TIMER_ON = 1;
    	    	 TPM_SetTimerPeriod (TPM0, FSM[10] .TIMER_MOD);
    	    	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	    	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	    	 TIMER_ON=0;
    	    	 TPM_StopTimer(TPM0);
    	    	 TPM0 -> CNT=0;
    	    	 estados=11;
    	     }
    	     break;

    	 case 11:
    		 if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[11] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    		 if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=12;
    	     }
    		 break;

    	 case 12:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[12] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=4;
    	     }
    	     break;

    	 case 13:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[13] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=14;
    	    }
    	    break;

    	 case 14:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[14] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=3;
    	    }
    	    break;


    	 case 15:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[15] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=16;
    	     }
    	     break;


    	 case 16:
    		 if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[16] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=17;
    	     }
    	     break;

    	 case 17:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[17] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=1;
    	     }
    	     break;


    	 case 18:
    		 if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[18] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=19;
    	     }
    	     break;


    	 case 19:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[19] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=20;
    	     }
    	     break;

    	 case 20:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[20] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=1;
    	     }
    	     break;


    	 case 21:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[21] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=22;
    	     }
    	     break;


    	 case 22:
    	     if (TIMER_ON==0){
    	     	 TIMER_ON = 1;
    	     	 TPM_SetTimerPeriod (TPM0, FSM[22] .TIMER_MOD);
    	     	 TPM_StartTimer (TPM0, kTPM_SystemClock);
    	     }
    	     if (TIMER_FLAG){
    	     	 TPM_ClearStatusFlags (TPM0, 1u<8u);
    	     	 TIMER_ON=0;
    	     	 TPM_StopTimer(TPM0);
    	     	 TPM0 -> CNT=0;
    	     	 estados=6;
    	     }
    	     break;
    	}
    }
    return 0 ;
}
