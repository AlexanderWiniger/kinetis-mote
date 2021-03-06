/**
 * \file
 *         A brief description of what this file is.
 * \author
 *         Alexander Winiger <alexander.winiger@stud.hslu.ch>
 */

#ifndef PLATFORM_CONF_H_
#define PLATFORM_CONF_H_

/*
 * Definitions below are dictated by the hardware and not really
 * changeable!
 */
#define PLATFORM_CONF_HAS_LEDS          1
#define PLATFORM_CONF_HAS_RADIO         1
#define PLATFORM_CONF_HAS_EEPROM        0
#define PLATFORM_CONF_HAS_ACCELEROMETER 1
#define PLATFORM_CONF_HAS_USB           1
#define PLATFORM_CONF_ENABLE_LOW_POWER  0
#define PLATFORM_CONF_ENABLE_HEARTBEAT  1

#define HAVE_STDINT_H
#include "kl25zdef.h"

#ifdef __CYGWIN__
int strcasecmp(const char*, const char*);
int strncasecmp(const char*, const char*, size_t);
char* strdup(const char*);
#else /* __CYGWIN__ */
#define HAVE_SNPRINTF
#define snprintf    _snprintf
#define strcasecmp  _stricmp
#define strncasecmp _strnicmp
#define strdup      _strdup
#endif /* __CYGWIN__ */

/* Compiler configurations */
#define CCIF
#define CLIF

#ifndef BV
#define BV(x) (1<<(x))
#endif

// needed for CC2520 to work
#define splhigh() 0
#define splx(arg)

/* Platform typedefs */
typedef uint32_t clock_time_t;
typedef uint32_t uip_stats_t;

/* CPU target speed in Hz */
#define F_CPU CLK0_FREQ_HZ

/* Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND       256UL
#define RTIMER_CONF_USE_LPTMR   PLATFORM_CONF_ENABLE_LOW_POWER
#if RTIMER_CONF_USE_LPTMR
#define RTIMER_CONF_SECOND      1024U
#else
#define RTIMER_CONF_SECOND      (4096U*8)
#endif

// UART Definitions
// Clock Source
#define UART0_IRCLK_SRC       0x03
#define UART0_OSCER_SRC       0x02
#define UART0_PLLFLL_SRC      0x01
#define UART0_CLK_DIS         0x00

/* Console source configuration */
#define CONSOLE_ARCH_CONF_USB   (1 & PLATFORM_CONF_HAS_USB)
#define UART0_CONF_RX_WITH_DMA  1
#define UART0_CONF_CLOCK_SRC    UART0_PLLFLL_SRC
#define SLIP_ARCH_CONF_USB      (0 & PLATFORM_CONF_HAS_USB)

/* LED ports */
#define LEDS_CONF_RED           (1<<18)
#define LEDS_CONF_GREEN	        (1<<19)
#define LEDS_CONF_BLUE          (1<<1)

#define RED_LED_OFF             GPIOB_PSOR = LEDS_CONF_RED
#define RED_LED_ON              GPIOB_PCOR = LEDS_CONF_RED
#define GREEN_LED_OFF           GPIOB_PSOR = LEDS_CONF_GREEN
#define GREEN_LED_ON            GPIOB_PCOR = LEDS_CONF_GREEN
#define BLUE_LED_OFF            GPIOD_PSOR = LEDS_CONF_BLUE
#define BLUE_LED_ON             GPIOD_PCOR = LEDS_CONF_BLUE

/*
 * SPI bus configuration for the frdm-kl25z
 */
/* SPI input/output registers. */
#define SPI_TXBUF SPI_D_REG(SPI1_BASE_PTR)
#define SPI_RXBUF SPI_D_REG(SPI1_BASE_PTR)

/* SPI bus - CC2520 pin configuration. */
#define CC2520_CONF_SYMBOL_LOOP_COUNT   2604
#define CC2520_CONF_AUTOACK             0

#define SPI_WAITFOREOTx() while (!(SPI1_S & SPI_S_SPTEF_MASK)){ __asm("NOP"); }         \
                          while (!(SPI1_S & SPI_S_SPRF_MASK)) { __asm("NOP"); }         \
                          (void)SPI_RXBUF;
#define SPI_WAITFOREORx() while (!(SPI1_S & SPI_S_SPTEF_MASK)){ __asm("NOP"); }         \
                          while (!(SPI1_S & SPI_S_SPRF_MASK)) { __asm("NOP"); }
#define SPI_WAITFORTxREADY() while (!(SPI1_S & SPI_S_SPTEF_MASK)){ __asm("NOP"); }

/*
 * Enables/disables CC2520 access to the SPI bus (not the bus).
 * (Chip Select)
 */
#define CC2520_SPI_ENABLE()     CC_CSN_ClrVal()
#define CC2520_SPI_DISABLE()    CC_CSN_SetVal()

/* Pin status.CC2520 */
#define CC2520_FIFO_IS_1        CC_FIFO_GetVal()
#define CC2520_FIFOP_IS_1       (GPIO_PDIR_REG(FPTD_BASE_PTR) & 0x01)
#define CC2520_CCA_IS_1	        CC_CCA_GetVal()
#define CC2520_SFD_IS_1	        CC_SFD_GetVal()

/* The CC2520 reset pin. */
#define SET_RESET_INACTIVE()    CC_RESET_SetVal()
#define SET_RESET_ACTIVE()      CC_RESET_ClrVal()

/* CC2520 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()       CC_VREG_EN_SetVal()
#define SET_VREG_INACTIVE()     CC_VREG_EN_ClrVal()

/* CC2520 rising edge trigger for external interrupt 0 (FIFOP). */
#define CC2520_FIFOP_INT_INIT()

/* FIFOP on external interrupt C4. */
#define CC2520_ENABLE_FIFOP_INT()       (PORTD_PCR0 |= (uint32_t) (PORT_PCR_IRQC(0x09) | PORT_PCR_ISF_MASK ))
#define CC2520_DISABLE_FIFOP_INT()      (PORTD_PCR0 = (uint32_t) ((PORTD_PCR0 & ~(0x0F)) | PORT_PCR_ISF_MASK))
#define CC2520_CLEAR_FIFOP_INT()

#endif /* PLATFORM_CONF_H_ */
