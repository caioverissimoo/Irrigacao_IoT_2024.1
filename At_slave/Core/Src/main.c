/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_lorawan.h"
#include "stm32wlxx_hal.h"
#include "core_cm4.h"
#include "stm32wlxx_hal_pwr.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LoRa.h"
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    uint8_t* Buff;
    uint8_t BuffSize;
} lora_AppData_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
//TIM_HandleTypeDef htim1;
/* USER CODE BEGIN PV */
uint32_t pMillis;
uint32_t cMillis;
float tCelsius = 0;
float tFahrenheit = 0;
float RH = 0;
int SENSOR_ID;
uint8_t RH1, RH2, TC1, TC2, SUM, CHECK;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void GPIO_Input_Init(void);
void Check_GPIO_Input(void);
uint8_t DHT22_Start(void);
uint8_t DHT22_Read(void);
void microDelay(uint16_t delay);
void SendSensorData(uint8_t sensorId, int temperature, int humidity);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void microDelay(uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim1, 0);
  while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
}

uint8_t DHT22_Start(void)
{
  uint8_t Response = 0;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  GPIO_InitStructPrivate.Pin = GPIO_PIN_11;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate); // set the pin as output
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);       // pull the pin low
  microDelay(1300);                                  // wait for 1300us
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);       // pull the pin high
  microDelay(30);                                    // wait for 30us
  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructPrivate); // set the pin as input
  microDelay(40);
  if (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)))
  {
    microDelay(80);
    if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11))) Response = 1;
  }
  pMillis = HAL_GetTick();
  cMillis = HAL_GetTick();
  while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) && pMillis + 2 > cMillis)
  {
    cMillis = HAL_GetTick();
  }
  return Response;
}

uint8_t DHT22_Read(void)
{
  uint8_t a, b = 0;
  for (a = 0; a < 8; a++)
  {
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) && pMillis + 2 > cMillis)
    { // wait for the pin to go high
      cMillis = HAL_GetTick();
    }
    microDelay(40); // wait for 40 us
    if (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11))) // if the pin is low
      b &= ~(1 << (7 - a));
    else
      b |= (1 << (7 - a));
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) && pMillis + 2 > cMillis)
    { // wait for the pin to go low
      cMillis = HAL_GetTick();
    }
  }
  return b;
}

void SendSensorData(uint8_t sensorId, int temperature, int humidity)
{
    uint8_t data[5];
    data[0] = sensorId;                      // Identificador do sensor
    data[1] = (uint8_t)(temperature >> 8);   // Parte alta da temperatura
    data[2] = (uint8_t)(temperature & 0xFF); // Parte baixa da temperatura
    data[3] = (uint8_t)(humidity >> 8);      // Parte alta da umidade
    data[4] = (uint8_t)(humidity & 0xFF);    // Parte baixa da umidade

    // Adicione aqui o código para enviar dados via LoRaWAN usando funções diretas
    // Este é um exemplo genérico, ajuste conforme a sua biblioteca ou hardware específico

    // Suponha que você tem uma função chamada LoRa_Send para enviar dados
    if (LoRa_Send(data, sizeof(data)) != 0) {
        printf("Erro ao enviar dados via LoRaWAN.\n");
    } else {
        printf("Dados enviados com sucesso via LoRaWAN.\n");
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_LoRaWAN_Init();
  MX_TIM1_Init();
  GPIO_Input_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    SENSOR_ID = DHT22_Start();
    RH1 = DHT22_Read(); // First 8bits of humidity
    RH2 = DHT22_Read(); // Last 8 bits of humidity
    TC1 = DHT22_Read(); // First 8 bits of Celsius
    TC2 = DHT22_Read(); // Last 8 bits of Celsius
    SUM = DHT22_Read(); // Check sum
    CHECK = RH1 + RH2 + TC1 + TC2;

    if (CHECK == SUM)
    {
      tCelsius = ((TC1 << 8) | TC2) / 10.0;
      tFahrenheit = tCelsius * 9 / 5 + 32;
      RH = ((RH1 << 8) | RH2) / 10.0;
      SendSensorData(SENSOR_ID, (int)(tCelsius * 10), (int)(RH * 10)); // Ajuste de escala conforme necessário
    }
    HAL_Delay(2000);
    Check_GPIO_Input();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  /* USER CODE BEGIN SystemClock_Config */

  /* USER CODE END SystemClock_Config */
}

/* USER CODE BEGIN 4 */
void GPIO_Input_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  //HAL_PWREx_EnableVddIO2();
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Check_GPIO_Input(void)
{
  uint8_t gpio_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
  printf("GPIO state: %d\n", gpio_state);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_Error_Handler */
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
/* USER CODE END Header_Error_Handler */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

