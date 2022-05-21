/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
void debugPrintln(UART_HandleTypeDef *uart_handle, char _out[]);
void demoDataTransfer();
void checkAddress(UART_HandleTypeDef * huart, I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint32_t Num_Trials, uint32_t Timeout);
void writeToI2C(uint16_t EEPROM_DEVICE_ADDR, uint8_t madd, uint8_t Data, uint8_t Result);
void readFromI2C(uint16_t EEPROM_DEVICE_ADDR, uint8_t madd, uint8_t Data, uint8_t Result);
void lightSensingInit();

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	//General purpose Function to send a char array over
	//the UART and to automatically send a new line
	//character after it
	void debugPrintln(UART_HandleTypeDef *uart_handle, char _out[]){
		HAL_UART_Transmit(uart_handle, (uint8_t *) _out, strlen(_out), 60);
		char newline[2] = "\r\n";
		HAL_UART_Transmit(uart_handle, (uint8_t *) newline, 2, 10);
	}

	//Demo sending data over UART to laptop
	void demoDataTransfer(){
	debugPrintln(&huart1, "Hello, this is STMF0 Discovery board"); // print full line
	//Flash Blue LED
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
	HAL_Delay(1000);
	}

	void checkAddress(UART_HandleTypeDef * huart, I2C_HandleTypeDef * hi2c, uint16_t DevAddress, uint32_t Num_Trials, uint32_t Timeout){
		uint8_t i = 0, I2Creturn;
		uint8_t strbuf[40] = { 0 };

		for (i = 1; i < 128; i++) {
			I2Creturn = HAL_I2C_IsDeviceReady(&hi2c,DevAddress, Num_Trials, Timeout);
			if (I2Creturn != HAL_OK){ //No ACK Received At That Address
				debugPrintln(&huart, "No ACK recieved");
			}

			else if (I2Creturn == HAL_OK) { //ACK Received At That Address
				sprintf(strbuf, "ACK received on address: 0x%X", i);
				debugPrintln(&huart, strbuf);
			}

			HAL_Delay(100);
		}

		debugPrintln(&huart, "#######Scanning done######");
	}

	void writeToI2C(uint16_t EEPROM_DEVICE_ADDR, uint8_t madd, uint8_t Data, uint8_t Result) {
		uint8_t *sData = &Data;
		uint8_t *rData = &Result;
		memset(str, 0, sizeof(str));
		sprintf(str, "Writing 0x%X to EEPROM address 0x%X", Data, madd);
		debugPrintln(&huart2, str);
		I2CReturn = HAL_I2C_Mem_Write(&hi2c1, EEPROM_DEVICE_ADDR, madd, 2, sData, 1, HAL_MAX_DELAY);
		if (I2CReturn != HAL_OK) {
		debugPrintln(&huart2, "Write to address FAILED");
		}

	}

	void readFromI2C(uint16_t EEPROM_DEVICE_ADDR, uint8_t madd, uint8_t Data, uint8_t Result) {
		uint8_t *sData = &Data;
		uint8_t *rData = &Result;
		memset(str, 0, sizeof(str));
		sprintf(str, "Reading from EEPROM address 0x%X ", madd);
		debugPrintln(&huart2, str);
		I2CReturn = HAL_I2C_Mem_Read(&hi2c1, EEPROM_DEVICE_ADDR, madd, 2, rData, 1, HAL_MAX_DELAY);
		if (I2CReturn != HAL_OK) {
		debugPrintln(&huart2, "Read from address FAILED");
	}

    void lightSensingInit(){
        // Test case: Set GPIO pin high.
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

        // Get ADC values for 2 pins that the LDRS are connected to.
        HAL_ADC_Start(&hadc1); //Start HAL_ADC for first ADC pin.
        HAL_ADC_Start(&hadc2); //Start HAL_ADC for second ADC pin.
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); //Poll ADC pin 1 for conversion (of the first LDR).
        raw = HAL_ADC_GetValue(&hadc1); //Get digital value
        HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY); //Poll ADC pin 2 for conversion (of the second LDR).
        raw2 = HAL_ADC_GetValue(&hadc2); //Get digital value.

        // Test: Set GPIO pin low
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
        if (raw>raw2){ //if LDR1 has more light on it than LDR2:
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //Turn LDR2 off.
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); //Turn LDR1 on.
        }
        else if (raw2>raw){ //else, if LDR2 has more light on it than LDR1:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); //Turn LDR1 off.
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //Turn LDR2 on.
        }
        // Convert to string and print
        sprintf(msg, "%hu\r\n", raw);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        sprintf(msg, "%hu\r\n", raw2);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        // Pretend we have to do something else for a while
        HAL_Delay(1);
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
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  checkAddress(&huart1, &hi2c1,(uint16_t) (i << 1), 3, 5);

  char str[60] = { 0 }; //Useful buffer for printing to UART
  uint8_t I2CReturn = 0; //Status var to indicate if HAL_I2C operation has succeeded (1) or failed (0);
  uint8_t i, j, Loop = 0; //Loop counters

  //Setup variables for reading and writing
  uint16_t EEPROM_DEVICE_ADDR = 0x55 << 1; //Address of EEPROM device on I2C bus
  uint16_t madd = 0x00; //Memory address variable containing a starting memory address for a location of memory in the EEPROM
  uint8_t Data = 0x10;//Data variable containing sStarting value to write to memory, could be any 8bit value
  uint8_t *sData = &Data; //Pointer to sending Data variable
  uint8_t Result = 0x00; //Variable to stored value read back from memory in
  uint8_t *rData = &Result; //Pointer to result data variable
  //Say hello over UART
  debugPrintln(&huart1, "Hello, this is STMF0 Discovery board: ");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      //LDR TESTING FUNCTION CALL:
      lightSensingInit(); //This toggles the connected LEDs depending on the light on the LDRS on the HAT.

    /* USER CODE END WHILE */
	  //Demo sending data over UART to laptop
	  //uncomment out the following line and comment all other loop code out to test sending data over UART to laptop
	  //demoDataTransfer();

	  //Print and increment a loop counter for visual tracking/debugging purposes only
	  memset(str, 0, sizeof(str)); //Reset str to zeros
	  sprintf(str, "\rLoop count %d\n", Loop);//Format string to include the loop counter variable
	  debugPrintln(&huart2, str);
	  Loop = Loop + 1;

	  //Write and read back 5 different values to 5 different memory locations
	  for (i = 0; i < 5; i++) {
		  writeToI2C(EEPROM_DEVICE_ADDR, madd, Data, Result);
		  readFromI2C(EEPROM_DEVICE_ADDR, madd, Data, Result);
		  //PRINT READ VALUE
		  memset(str, 0, sizeof(str));
		  sprintf(str, "Received data: 0x%X \n", Result);
		  debugPrintln(&huart2, str);
		  //Increment address and data values and clear Result holder
		  madd = madd + 1;
		  Data = Data + 1;
		  Result = 0x00;
		  HAL_Delay(1000); //Pause
		  } //End for loop of reading and writing 5 addresses

	  //Read back last 5 values again:
	  madd = madd - 5;
	  for (j = 0; j < 5; j++) {
		  I2CReturn = HAL_I2C_Mem_Read(&hi2c1, EEPROM_DEVICE_ADDR, madd, 2, rData, 1, HAL_MAX_DELAY);
	  if (I2CReturn != HAL_OK) {
		  debugPrintln(&huart2, "Read from address FAILED");
	  }

	  //PRINT READ VALUE
	  memset(str, 0, sizeof(str));
	  sprintf(str, "Address 0x%X contains: 0x%X ", madd, Result);
	  debugPrintln(&huart2, str);
	  madd = madd + 1;
	  }

	  debugPrintln(&huart2, "-------------"); //Draw a line under a series of 5 before next while loop begins
	  //Flash Blue LED as visual check code is running
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
	  HAL_Delay(DELAY);

	  }

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_15;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x2000090E;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
