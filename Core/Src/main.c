/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Parcial Sistemas Embebidos (A prueba de cambios de pines y Ánodo Común)
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

// Función auxiliar para el display de 7 segmentos
void mostrar_7seg(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g);

// Prototipos de las funciones del parcial
void funcion_conteo_0_9(uint16_t retardo);
void funcion_palabra(uint16_t retardo);
void luces_der_izq(uint32_t velocidad);
void luces_centro_extremos(uint32_t velocidad);

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
    // A prueba de balas: usa buzzerPin_GPIO_Port generado por CubeMX
    HAL_GPIO_WritePin(buzzerPin_GPIO_Port, buzzerPin_Pin, GPIO_PIN_SET);
    delay_us_dwt(dela);
    HAL_GPIO_WritePin(buzzerPin_GPIO_Port, buzzerPin_Pin, GPIO_PIN_RESET);
    delay_us_dwt(dela);
  }
}

void reproducir_secuencia(const uint16_t notas[], const uint16_t duraciones[], uint16_t total)
{
  for (uint16_t i = 0; i < total; i++)
  {
    Sound_play(notas[i], duraciones[i]);
    HAL_Delay(duraciones[i]); // Pausa equivalente entre notas
  }
}

// =========================================================================
// FUNCIÓN AUXILIAR PARA EL DISPLAY DE 7 SEGMENTOS
// =========================================================================
// Usa _GPIO_Port para cada segmento. Ya no importa si están en PA o PB.
void mostrar_7seg(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g)
{
  HAL_GPIO_WritePin(A7s_GPIO_Port, A7s_Pin, a ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(B7s_GPIO_Port, B7s_Pin, b ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(C7s_GPIO_Port, C7s_Pin, c ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(D7s_GPIO_Port, D7s_Pin, d ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(E7s_GPIO_Port, E7s_Pin, e ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(F7s_GPIO_Port, F7s_Pin, f ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(G7s_GPIO_Port, G7s_Pin, g ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// =========================================================================
// 1. FUNCION DE CONTEO (0 al 9) - ÁNODO COMÚN (0 enciende, 1 apaga)
// =========================================================================
void funcion_conteo_0_9(uint16_t retardo) {
  // 0
  mostrar_7seg(0, 0, 0, 0, 0, 0, 1);
  HAL_Delay(retardo);
  
  // 1
  mostrar_7seg(1, 0, 0, 1, 1, 1, 1);
  HAL_Delay(retardo);
  
  // 2
  mostrar_7seg(0, 0, 1, 0, 0, 1, 0);
  HAL_Delay(retardo);
  
  // 3
  mostrar_7seg(0, 0, 0, 0, 1, 1, 0);
  HAL_Delay(retardo);
  
  // 4
  mostrar_7seg(1, 0, 0, 1, 1, 0, 0);
  HAL_Delay(retardo);
  
  // 5
  mostrar_7seg(0, 1, 0, 0, 1, 0, 0);
  HAL_Delay(retardo);
  
  // 6
  mostrar_7seg(0, 1, 0, 0, 0, 0, 0);
  HAL_Delay(retardo);
  
  // 7
  mostrar_7seg(0, 0, 0, 1, 1, 1, 1);
  HAL_Delay(retardo);
  
  // 8
  mostrar_7seg(0, 0, 0, 0, 0, 0, 0);
  HAL_Delay(retardo);
  
  // 9
  mostrar_7seg(0, 0, 0, 0, 1, 0, 0);
  HAL_Delay(retardo);
  
  // Apagar todo al finalizar (en ánodo común se apaga con 1)
  mostrar_7seg(1, 1, 1, 1, 1, 1, 1);
}

// =========================================================================
// 2. FUNCION DE PALABRA (SISTEMAS) - ÁNODO COMÚN
// =========================================================================
void funcion_palabra(uint16_t retardo) {
  // Letra S 
  mostrar_7seg(0, 1, 0, 0, 1, 0, 0);
  HAL_Delay(retardo);

  // Letra I 
  mostrar_7seg(1, 0, 0, 1, 1, 1, 1);
  HAL_Delay(retardo);

  // Letra S 
  mostrar_7seg(0, 1, 0, 0, 1, 0, 0);
  HAL_Delay(retardo);

  // Letra T 
  mostrar_7seg(1, 1, 1, 0, 0, 0, 0);
  HAL_Delay(retardo);

  // Letra E 
  mostrar_7seg(0, 1, 1, 0, 0, 0, 0);
  HAL_Delay(retardo);

  // Letra M (Aproximación con A, D, G)
  mostrar_7seg(0, 1, 1, 0, 1, 1, 0);
  HAL_Delay(retardo);

  // Letra A 
  mostrar_7seg(0, 0, 0, 1, 0, 0, 0);
  HAL_Delay(retardo);

  // Letra S 
  mostrar_7seg(0, 1, 0, 0, 1, 0, 0);
  HAL_Delay(retardo);
  
  // Apagar todo al finalizar
  mostrar_7seg(1, 1, 1, 1, 1, 1, 1);
}

// =========================================================================
// 3. JUEGO DE LUCES (Derecha a Izquierda)
// =========================================================================
void luces_der_izq(uint32_t velocidad) {
  // Apagar todos los LEDs primero a prueba de balas (usando _GPIO_Port)
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_RESET);

  // Encender y apagar uno a uno desde led5 a led1
  HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_RESET); 
  
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET); 
  
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET); 
  
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET); 
  
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET); 
}

// =========================================================================
// 4. JUEGO DE LUCES (Centro a Extremos)
// =========================================================================
void luces_centro_extremos(uint32_t velocidad) {
  // Apagar todos los LEDs al inicio (uno por uno para que funcione siempre)
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_RESET);

  // Centro (led3)
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led3_GPIO_Port, led3_Pin, GPIO_PIN_RESET); 
  
  // Medios (led2 y led4)
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(led4_GPIO_Port, led4_Pin, GPIO_PIN_RESET); 
  
  // Extremos (led1 y led5)
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_SET);
  HAL_Delay(velocidad);
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, GPIO_PIN_RESET); 
  HAL_GPIO_WritePin(led5_GPIO_Port, led5_Pin, GPIO_PIN_RESET); 
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
  /* USER CODE BEGIN 2 */
  delay_us_dwt_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // -------------------------------------------------------------
    // EJECUCIÓN SECUENCIAL DE LAS REQUERIMIENTOS DEL PARCIAL
    // -------------------------------------------------------------
    
    // 1. Conteo 0 al 9 en el display de 7 segmentos (1 segundo por número)
    funcion_conteo_0_9(1000);
    HAL_Delay(1000);

    // 2. Palabra SISTEMAS en el display (500 ms por letra)
    funcion_palabra(500);
    HAL_Delay(1000);

    // 3. Juego de luces Derecha a Izquierda
    luces_der_izq(200);
    HAL_Delay(1000);

    // 4. Juego de luces Centro a Extremos
    luces_centro_extremos(300);
    HAL_Delay(1000);

    // 5. Melodía MAMI (Karol G)
    cancion_2_MAMIII();
    HAL_Delay(1000);

    // 6. Melodía Take On Me / Careless Whisper
    cancion_3_CarelessWhisper();
    HAL_Delay(1000);

    // 7. Melodía La Gata bajo la lluvia
    cancion_1_LaGataBajoLaLluvia();
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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, G7s_Pin|F7s_Pin|A7s_Pin|B7s_Pin
                          |led1_Pin|led2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, led3_Pin|led4_Pin|led5_Pin|buzzerPin_Pin
                          |C7s_Pin|D7s_Pin|E7s_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : G7s_Pin F7s_Pin A7s_Pin B7s_Pin
                           led1_Pin led2_Pin */
  GPIO_InitStruct.Pin = G7s_Pin|F7s_Pin|A7s_Pin|B7s_Pin
                          |led1_Pin|led2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : led3_Pin led4_Pin led5_Pin buzzerPin_Pin
                           C7s_Pin D7s_Pin E7s_Pin */
  GPIO_InitStruct.Pin = led3_Pin|led4_Pin|led5_Pin|buzzerPin_Pin
                          |C7s_Pin|D7s_Pin|E7s_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
