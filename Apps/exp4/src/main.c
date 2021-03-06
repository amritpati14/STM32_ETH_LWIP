/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tcpip.h"
#include "httpserver-netconn.h"
//#include "serial_debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/*--------------- LCD Messages ---------------*/
//#define MESSAGE1   "     STM32F4x7      "
#define MESSAGE1   "     hello world    "
#define MESSAGE2   "  STM32F-4 Series   "
#define MESSAGE3   "Basic WebServer Demo"

/*--------------- Tasks Priority -------------*/
#define DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 1 )
#define BT_TASK_PRIO    ( tskIDLE_PRIORITY + 1 )

static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\r\n";

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void LCD_LED_Init(void);
void ToggleGreen(void * pvParameters);
void vTaskFunction( void *pvParameters );

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured to
       168 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
//add a long delay wait for DP83848 finish reset
unsigned int i,j;
for(i=0;i<50;i++)
{
    for(j=0;j < 65500;j++);
}
#ifdef SERIAL_DEBUG
  DebugComPort_Init();
#endif

  /* Initialize wakeup button */
   //STM_EVAL_PBInit( BUTTON_WAKEUP, BUTTON_MODE_GPIO );

  /*Initialize LCD and Leds */
  LCD_LED_Init();
  printf("hello world!\r\n");
  /* configure Ethernet (GPIOs, clocks, MAC, DMA) */
   //ETH_BSP_Config();
  printf("ETH_BSP_Config\r\n");
  /* Initilaize the LwIP stack */
   //LwIP_Init();

  /* Initialize webserver demo */
   //http_server_netconn_init();
  printf("http_server_netconn_init\r\n");
//#ifdef USE_DHCP
//  /* Start DHCPClient */
//  xTaskCreate(LwIP_DHCP_task, "DHCPClient", configMINIMAL_STACK_SIZE * 2, NULL,DHCP_TASK_PRIO, NULL);
//#endif
  printf("DHCPClient\r\n");
  /* Start toogleLed4 task : Toggle LED4  every 250ms */
  xTaskCreate(ToggleGreen, "LED4", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
   xTaskCreate( vTaskFunction, "Task1", 1000, pcTextForTask1,
         1,
         NULL );
   xTaskCreate( vTaskFunction, "Task2", 1000, pcTextForTask2,
         2, NULL );

  /* Start scheduler */
  vTaskStartScheduler();
  /* We should never get here as control is now taken by the scheduler */
  for( ;; );

}

/**
 *
 */
void vTaskFunction(void * pvParameters)
{
   char *pcTaskName;
   const TickType_t xDelay250ms = pdMS_TO_TICKS( 250 );
   pcTaskName = (char*) pvParameters;

   for (;;)
   {
      printf( pcTaskName );
      vTaskDelay( xDelay250ms );
   }
}

/**
  * @brief  Toggle Led4 task
  * @param  pvParameters not used
  * @retval None
  */
void ToggleGreen(void * pvParameters)
{
  const TickType_t xDelay250ms = pdMS_TO_TICKS( 250 );
  for( ;; )
  {
    /* Toggle LED4 each 250ms */
    STM_EVAL_LEDToggle(LED1);
    //STM_EVAL_LEDToggle(LED2);
    //STM_EVAL_LEDToggle(LED3);
    //STM_EVAL_LEDToggle(LED4);
    vTaskDelay( xDelay250ms );
  }
}

/**
  * @brief  Initializes the STM324xG-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
void LCD_LED_Init(void)
{
#ifdef USE_LCD
  /* Initialize the STM324xG-EVAL's LCD */
  STM324xG_LCD_Init();
#endif

  /* Initialize STM324xG-EVAL's LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  //STM_EVAL_LEDInit(LED3);
  //STM_EVAL_LEDInit(LED4);

#ifdef USE_LCD
  /* Clear the LCD */
  LCD_Clear(Black);

  /* Set the LCD Back Color */
  LCD_SetBackColor(Black);

  /* Set the LCD Text Color */
  LCD_SetTextColor(White);

  /* Display message on the LCD*/
  LCD_DisplayStringLine(Line0, (uint8_t*)MESSAGE1);
  LCD_DisplayStringLine(Line1, (uint8_t*)MESSAGE2);
  LCD_DisplayStringLine(Line2, (uint8_t*)MESSAGE3);
#endif
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
