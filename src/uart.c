#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"

#include "cmsis_os.h"

#include "uart.h"

/* USART2 instance is used. (TX on PA.02, RX on PA.03)
   (please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled 
    on HW board in order to support Virtual Com Port) */
#define USARTx_INSTANCE               USART2
#define USARTx_CLK_ENABLE()           LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define USARTx_IRQn                   USART2_IRQn
#define USARTx_IRQHandler             USART2_IRQHandler

#define USARTx_GPIO_CLK_ENABLE()      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)   /* Enable the peripheral clock of GPIOA */
#define USARTx_TX_PIN                 LL_GPIO_PIN_2
#define USARTx_TX_GPIO_PORT           GPIOA
#define USARTx_SET_TX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7)
#define USARTx_RX_PIN                 LL_GPIO_PIN_3
#define USARTx_RX_GPIO_PORT           GPIOA
#define USARTx_SET_RX_GPIO_AF()       LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7)
#define APB_Div 2

#define UART_BAUD_RATE 115200
// #define UART_BAUD_RATE 1843200

static osMessageQId _uart_tx_queue_id;
static osMessageQId _uart_rx_queue_id;

osMessageQDef(uartTXQueue, 256, char);
osMessageQDef(uartRXQueue, 64,  char);

static osMutexId _uart_write_mutex_id;
osMutexDef (uartWriteMutex);

static uint8_t _uart_is_init = 0;

void uart_init(void)
{
    if (!_uart_is_init)
    {
        // Disable stream buffers so I/O occurs immediately
        setvbuf(stdin,  NULL, _IONBF, 0); // should be a read-only stream
        setvbuf(stdout, NULL, _IONBF, 0); // disables wait for \n before printing
        setvbuf(stderr, NULL, _IONBF, 0); // should be already unbuffered
            
        /* (1) Enable GPIO clock and configures the USART pins *********************/

        /* Enable the peripheral clock of GPIO Port */
        USARTx_GPIO_CLK_ENABLE();

        /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
        LL_GPIO_SetPinMode(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_MODE_ALTERNATE);
        USARTx_SET_TX_GPIO_AF();
        LL_GPIO_SetPinSpeed(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
        LL_GPIO_SetPinOutputType(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetPinPull(USARTx_TX_GPIO_PORT, USARTx_TX_PIN, LL_GPIO_PULL_UP);

        /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
        LL_GPIO_SetPinMode(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_MODE_ALTERNATE);
        USARTx_SET_RX_GPIO_AF();
        LL_GPIO_SetPinSpeed(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
        LL_GPIO_SetPinOutputType(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetPinPull(USARTx_RX_GPIO_PORT, USARTx_RX_PIN, LL_GPIO_PULL_UP);

        /* (2) NVIC Configuration for USART interrupts */
        // NVIC_SetPriority(USARTx_IRQn, 0);
        NVIC_SetPriority(USARTx_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 6, 0));

        NVIC_EnableIRQ(USARTx_IRQn);

        /* (3) Enable USART peripheral clock and clock source ***********************/
        USARTx_CLK_ENABLE();

        /* (4) Configure USART functional parameters ********************************/
        
        /* Disable USART prior modifying configuration registers */
        /* Note: Commented as corresponding to Reset value */
        // LL_USART_Disable(USARTx_INSTANCE);

        /* TX/RX direction */
        LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);

        /* 8 data bit, 1 start bit, 1 stop bit, no parity */
        LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

        /* No Hardware Flow control */
        /* Reset value is LL_USART_HWCONTROL_NONE */
        // LL_USART_SetHWFlowCtrl(USARTx_INSTANCE, LL_USART_HWCONTROL_NONE);

        /* Oversampling by 16 */
        /* Reset value is LL_USART_OVERSAMPLING_16 */
        // LL_USART_SetOverSampling(USARTx_INSTANCE, LL_USART_OVERSAMPLING_16);

        /* Set Baudrate to UART_BAUD_RATE using APB frequency set to 100000000/APB_Div Hz */
        /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
        /* Ex :
            Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
        
            In this example, Peripheral Clock is expected to be equal to 100000000/APB_Div Hz => equal to SystemCoreClock/APB_Div
        */
        LL_USART_SetBaudRate(USARTx_INSTANCE, SystemCoreClock/APB_Div, LL_USART_OVERSAMPLING_16, UART_BAUD_RATE); 

        /* (5) Enable USART *********************************************************/
        LL_USART_Enable(USARTx_INSTANCE);

        /* Clear Overrun flag, in case characters have already been sent to USART */
        LL_USART_ClearFlag_ORE(USARTx_INSTANCE);

        /* Enable RXNE and Error interrupts */
        LL_USART_EnableIT_RXNE(USARTx_INSTANCE);
        LL_USART_EnableIT_ERROR(USARTx_INSTANCE);

        // Initialise transmit and receive message queues
        _uart_tx_queue_id = osMessageCreate(osMessageQ(uartTXQueue), NULL);
        _uart_rx_queue_id = osMessageCreate(osMessageQ(uartRXQueue), NULL);

        // Initialise write mutex
        _uart_write_mutex_id  = osMutexCreate(osMutex(uartWriteMutex));

        _uart_is_init = 1;
    }
}

// Make printf thread safe by wrapping with mutex
int __wrap_printf(char *fmt, ...)
{
    int result = 0;
    if (osMutexWait(_uart_write_mutex_id, osWaitForever) == osOK)
    {
        va_list args;
        va_start(args, fmt);
        result = vprintf(fmt, args);
        va_end(args);
        osMutexRelease(_uart_write_mutex_id);
    }
    return result;
}

int __io_putchar(int c)
{
    // Queue char for transmission, block if queue full
    osMessagePut(_uart_tx_queue_id, (char)c, osWaitForever);

    // Enable TXE interrupt
    LL_USART_EnableIT_TXE(USARTx_INSTANCE); 

    return c;
}

int __io_getchar(void)
{
    // Dequeue received char, block if queue empty
    osEvent evt = osMessageGet(_uart_rx_queue_id, osWaitForever);
    return evt.value.v;
}

int _write(int file, char *ptr, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return i;
}

int _read(int file, char *ptr, int len)
{
    // printf("len=%d, ", len);
    
    // BUG
    // using newlib-nano, setvbuf(stdin,  NULL, _IONBF, 0) has no effect, and we 
    // keep getting len == 1024.
    // Alternatively, if we remove the libc.a reference from the linker script, we
    // get len == 1 with newlib-nano.
    //
    // newlib-nano is apparently built using --disable-newlib-fvwrite-in-streamio, which
    // disables the setvbuf functions.
    //
    // see also 
    //  https://answers.launchpad.net/gcc-arm-embedded/+question/246038
    //  https://forum.43oh.com/topic/6989-newlib-problems-with-printf/

    len = 1; // workaround

    int i;

    for (i = 0; i < len; i++)
    {
        int c;
        if ((c = __io_getchar()) == EOF) break;
        *ptr++ = c;
    }

    return i;
}

void _uart_rxne_isr(void);
void _uart_txe_isr(void);
void _uart_error(void);

void USARTx_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_RXNE(USARTx_INSTANCE) && LL_USART_IsEnabledIT_RXNE(USARTx_INSTANCE))
    {
        _uart_rxne_isr();
    }

    if(LL_USART_IsEnabledIT_TXE(USARTx_INSTANCE) && LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE))
    {
        _uart_txe_isr();
    }

    if(LL_USART_IsEnabledIT_ERROR(USARTx_INSTANCE) /*&& LL_USART_IsActiveFlag_NE(USARTx_INSTANCE)*/)
    {
        _uart_error();
    }
}

void _uart_rxne_isr(void)
{
    uint8_t c = LL_USART_ReceiveData8(USARTx_INSTANCE);

    // Queue received char, drop if queue full
    osMessagePut(_uart_rx_queue_id, (char)c, 0);
}

void _uart_txe_isr(void)
{
    osEvent evt = osMessageGet(_uart_tx_queue_id, 0);
    switch (evt.status)
    {
    case osOK:                  // No message was received and no timeout specified
    case osEventTimeout:        // No message was received
        // Disable TXE interrupt so it doesn't immediately retrigger
        LL_USART_DisableIT_TXE(USARTx_INSTANCE);
        break;
    case osEventMessage:        // Message received
        LL_USART_TransmitData8(USARTx_INSTANCE, (uint8_t)evt.value.v);
        break;
    }
}

void _uart_error(void)
{
    __IO uint32_t sr_reg;

    /* Disable USARTx_IRQn */
    // NVIC_DisableIRQ(USARTx_IRQn);
    
    /* Error handling example :
      - Read USART SR register to identify flag that leads to IT raising
      - Perform corresponding error handling treatment according to flag
    */
    sr_reg = LL_USART_ReadReg(USARTx_INSTANCE, SR);

    if (sr_reg & LL_USART_SR_PE) 
    {
        printf("*** USART parity error! ***\n");
    }

    if (sr_reg & LL_USART_SR_FE) 
    {
        printf("*** USART frame error! ***\n");
    }

    if (sr_reg & LL_USART_SR_ORE) 
    {
        printf("*** USART overrun error! ***\n");
    }

    if (sr_reg & LL_USART_SR_NE) 
    {
        printf("*** USART noise error! ***\n");
    }
}

