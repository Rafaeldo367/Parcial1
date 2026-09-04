/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Programa principal con 3 canciones para Buzzer Pasivo
  ******************************************************************************
  * Canción 1: La Gata Bajo la Lluvia
  * Canción 2: MAMIII (Karol G & Becky G)
  * Canción 3: Careless Whisper (George Michael)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// --- DICCIONARIO ÚNICO DE FRECUENCIAS (en Hz) ---
#define Sil   5

// Octava 1 y 2 (Notas Graves)
#define C_    (261/2)
#define Cs_   (277/2)
#define D_    (293/2)
#define Ds_   (311/2)
#define E_    (329/2)
#define F_    (349/2)
#define Fs_   (369/2)
#define G_    (391/2)
#define Gs_   (415/2)
#define A_    (440/2)
#define As_   (466/2)
#define B_    (493/2)

// Octava 3 (Notas Medias)
#define C     261
#define Cs    277
#define D     293
#define Ds    311
#define E     329
#define F     349
#define Fs    369
#define G     391
#define Gs    415
#define A     440
#define As    466
#define B     493

// Octava 4 (Notas Altas)
#define C2    524
#define Cs2   555
#define D2    588
#define Ds2   623
#define E2    660
#define F2    699
#define Fs2   740
#define G2    784
#define Gs2   831
#define A_2   880
#define As2   933
#define B2    988

// Octava 5 (Notas Muy Altas)
#define C3    1047
#define Cs3   (555*2)
#define D3    (588*2)
#define Ds3   (623*2)
#define E3    (660*2)
#define F3    (699*2)
#define Fs3   (740*2)
#define G3    (784*2)
#define Gs3   (831*2)
#define A_3   (880*2)
#define As3   (933*2)
#define B3    (988*2)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t pasos;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void delay_us_dwt_init(void);
void delay_us_dwt(uint32_t reta);
void Sound_play(uint32_t frec, uint32_t dura);
void reproducir_secuencia(const uint16_t notas[], const uint16_t duraciones[], uint16_t total);

void cancion_1_LaGataBajoLaLluvia(void);
void cancion_2_MAMIII(void);
void cancion_3_CarelessWhisper(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// --- FUNCIONES DEL BUZZER ---
void delay_us_dwt_init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  pasos = (HAL_RCC_GetSysClockFreq() / 1000000);
}

void delay_us_dwt(uint32_t reta)
{
  DWT->CYCCNT = 0;
  while (DWT->CYCCNT <= pasos * reta);
}

void Sound_play(uint32_t frec, uint32_t dura)
{
  if (frec <= 5) {
    HAL_Delay(dura);
    return;
  }
  
  uint32_t dela = 500000 / frec;
  uint32_t repe = frec * dura / 1000;

  while (repe--)
  {
    HAL_GPIO_WritePin(parlante_GPIO_Port, parlante_Pin, GPIO_PIN_SET);
    delay_us_dwt(dela);
    HAL_GPIO_WritePin(parlante_GPIO_Port, parlante_Pin, GPIO_PIN_RESET);
    delay_us_dwt(dela);
  }
}

// Reproductor genérico de secuencias
void reproducir_secuencia(const uint16_t notas[], const uint16_t duraciones[], uint16_t total)
{
  for (uint16_t i = 0; i < total; i++)
  {
    Sound_play(notas[i], duraciones[i]);
    HAL_Delay(duraciones[i]); // Pausa equivalente entre notas
  }
}

// =========================================================================
// CANCIÓN 1: LA GATA BAJO LA LLUVIA
// =========================================================================
void cancion_1_LaGataBajoLaLluvia(void)
{
  #define TEMPO_GATA (88 * 2)
  #define N_GATA     (60000 / TEMPO_GATA)
  #define S_GATA     (N_GATA / 4)
  #define C_GATA     (2 * S_GATA)
  #define B_GATA     (N_GATA * 2)

  static const uint16_t n[] = {
    E, B, Sil, E, B, A, B, A, B, A, E, Cs, Gs, Sil, Cs, Gs, Fs, Gs, Fs, Gs, Fs, Cs, A_, E, Sil, E,
    E, D, Cs, D, Cs, D, Fs, A, Cs2, B, Cs2, B, Cs2, B, E, B, Sil, E, B, A, B, A, B, A, E, Cs, Gs,
    Sil, Cs, Gs, Fs, Gs, Fs, Gs, Fs, Cs, A_, E, Sil, E, E, D, Cs, D, Cs, D, Fs, A, Cs2, B, Cs2, B,
    Cs2, B, Sil, Cs2, B, Cs2, B, Fs, A, Cs2, Cs2, Sil, B, A, B, A, E, Gs, B, B, Sil, A, Gs, A, Fs,
    Gs, A, Gs, Fs, A, Gs, Sil, Sil, Gs, A, B, B, Gs, Sil, Sil, Gs, A, B, D2, Cs2, Sil, Sil, Fs, Gs,
    A, Cs2, B, A
  };

  static const uint16_t d[] = {
    C_GATA, B_GATA, N_GATA + C_GATA, C_GATA, S_GATA + C_GATA, S_GATA + C_GATA, S_GATA, S_GATA,
    S_GATA + C_GATA, S_GATA + C_GATA, S_GATA, S_GATA, B_GATA, N_GATA + C_GATA, C_GATA,
    S_GATA + C_GATA, S_GATA + C_GATA, S_GATA, S_GATA, S_GATA + C_GATA, S_GATA + C_GATA, S_GATA, S_GATA,
    B_GATA, N_GATA + C_GATA, S_GATA, S_GATA + C_GATA, C_GATA, C_GATA, S_GATA, C_GATA + S_GATA, C_GATA,
    C_GATA, S_GATA, C_GATA + S_GATA, C_GATA, C_GATA, S_GATA, C_GATA + S_GATA, C_GATA, C_GATA, B_GATA,
    B_GATA, C_GATA, S_GATA + C_GATA, S_GATA + C_GATA, S_GATA, S_GATA, S_GATA + C_GATA, S_GATA + C_GATA,
    S_GATA, S_GATA, B_GATA, N_GATA + C_GATA, C_GATA, S_GATA + C_GATA, S_GATA + C_GATA, S_GATA, S_GATA,
    S_GATA + C_GATA, S_GATA + C_GATA, S_GATA, S_GATA, B_GATA, N_GATA + C_GATA, S_GATA, S_GATA + C_GATA,
    C_GATA, C_GATA, S_GATA, C_GATA + S_GATA, C_GATA, C_GATA, S_GATA, C_GATA + S_GATA, C_GATA, C_GATA,
    S_GATA, C_GATA + S_GATA, C_GATA + S_GATA, S_GATA, S_GATA, N_GATA + C_GATA, C_GATA, C_GATA, N_GATA,
    S_GATA, S_GATA, B_GATA, N_GATA + C_GATA, S_GATA, S_GATA, N_GATA + C_GATA, C_GATA, C_GATA, N_GATA,
    S_GATA, S_GATA, B_GATA, N_GATA + C_GATA, S_GATA, S_GATA, N_GATA + C_GATA, C_GATA, C_GATA, N_GATA,
    S_GATA, S_GATA, C_GATA + S_GATA, N_GATA, N_GATA, S_GATA, N_GATA / 3, N_GATA / 3, N_GATA / 3,
    C_GATA + S_GATA, N_GATA, N_GATA, S_GATA, N_GATA / 3, N_GATA / 3, N_GATA / 3, C_GATA + S_GATA,
    N_GATA, B_GATA, S_GATA, C_GATA, C_GATA, C_GATA, B_GATA, C_GATA, B_GATA
  };

  reproducir_secuencia(n, d, sizeof(n) / sizeof(n[0]));
}

// =========================================================================
// CANCIÓN 2: MAMIII (KAROL G)
// =========================================================================
void cancion_2_MAMIII(void)
{
  #define S_MAM   80
  #define C_MAM   (2 * S_MAM)
  #define N_MAM   (C_MAM * 2)
  #define B_MAM   (N_MAM * 2)

  static const uint16_t n[] = {
    G, G, G2, G2, G2, Fs2, B,  G, G, G2, G2, G2, Fs2, C2,  C2, C2, G2, G2, G2, Fs2, C2, C2,
    C2, C2, G2, G2, G2, Fs2, B, A, B,  G, G2, G2, G2, Fs2, B, A, B,  D2, D2, D2, D2, D2, D2, D2,
    E2, D2, C2, C2, G2, G2, G2, C2, C2, C2, C2, C2, G2, Fs2, E2, Fs2, E2
  };

  static const uint16_t d[] = {
    C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, N_MAM,  C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, N_MAM,
    C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, C_MAM,  C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, C_MAM,
    C_MAM + S_MAM, S_MAM, C_MAM,  C_MAM, C_MAM, C_MAM, C_MAM, C_MAM, C_MAM + S_MAM, S_MAM, N_MAM + S_MAM,
    S_MAM, S_MAM, S_MAM, S_MAM, S_MAM, S_MAM, C_MAM, C_MAM, S_MAM,  C_MAM, C_MAM,  C_MAM, C_MAM, C_MAM,
    C_MAM, C_MAM, C_MAM, C_MAM, C_MAM,  C_MAM, C_MAM, C_MAM, N_MAM, B_MAM
  };

  reproducir_secuencia(n, d, sizeof(n) / sizeof(n[0]));
}

// =========================================================================
// CANCIÓN 3: CARELESS WHISPER
// =========================================================================
void cancion_3_CarelessWhisper(void)
{
  #define S_CW   (60000 / (77 * 4 * 2))
  #define C_CW   (2 * S_CW)
  #define N_CW   (C_CW * 2)
  #define B_CW   (N_CW * 2)
  #define TTT_CW (S_CW / 6)

  static const uint16_t n[] = {
    Cs2,
    // Intro Repetición 1
    D2, E2, Fs2, G2, A_2, B2, Cs3, B2, Fs2, D2, Cs3, B2, Fs2, D2, A_2, G2, D2, B, A_2, G2, D2, G2,
    Fs2, D2, B, G, Sil, Fs, G, A, B, Cs2, D2, E2, Fs2,
    // Intro Repetición 2
    D2, E2, Fs2, G2, A_2, B2, Cs3, B2, Fs2, D2, Cs3, B2, Fs2, D2, A_2, G2, D2, B, A_2, G2, D2, G2,
    Fs2, D2, B, G, Sil, Fs, G, A, B, Cs2, D2, E2, Fs2
  };

  static const uint16_t d[] = {
    C_CW,
    // Repetición 1
    TTT_CW, TTT_CW, TTT_CW, TTT_CW, TTT_CW, TTT_CW, C_CW - 6 * TTT_CW, S_CW, C_CW, C_CW, C_CW + S_CW,
    S_CW, C_CW, C_CW + S_CW, C_CW, S_CW, C_CW, C_CW, C_CW + S_CW, S_CW, N_CW + S_CW, C_CW, S_CW, C_CW,
    C_CW, B_CW, S_CW, C_CW, C_CW, C_CW, C_CW, C_CW, C_CW, C_CW, C_CW,
    // Repetición 2
    TTT_CW, TTT_CW, TTT_CW, TTT_CW, TTT_CW, TTT_CW, C_CW - 6 * TTT_CW, S_CW, C_CW, C_CW, C_CW + S_CW,
    S_CW, C_CW, C_CW + S_CW, C_CW, S_CW, C_CW, C_CW, C_CW + S_CW, S_CW, N_CW + S_CW, C_CW, S_CW, C_CW,
    C_CW, B_CW, S_CW, C_CW, C_CW, C_CW, C_CW, C_CW, C_CW, C_CW, C_CW
  };

  reproducir_secuencia(n, d, sizeof(n) / sizeof(n[0]));
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  delay_us_dwt_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // Canción 1
    cancion_1_LaGataBajoLaLluvia();
    HAL_Delay(2000);

    // Canción 2
    cancion_2_MAMIII();
    HAL_Delay(2000);

    // Canción 3
    cancion_3_CarelessWhisper();
    HAL_Delay(2000);

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(parlante_GPIO_Port, parlante_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD1_Pin|LD2_Pin|LD3_Pin|LD4_Pin
                          |LD5_Pin|LD6_Pin|LD7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : parlante_Pin */
  GPIO_InitStruct.Pin = parlante_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(parlante_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD2_Pin LD3_Pin LD4_Pin
                           LD5_Pin LD6_Pin LD7_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD2_Pin|LD3_Pin|LD4_Pin
                          |LD5_Pin|LD6_Pin|LD7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
}
#endif /* USE_FULL_ASSERT */