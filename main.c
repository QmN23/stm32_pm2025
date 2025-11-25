#include "ssd1306.h"
#include "stm32f10x.h"

void Peripherals_Init(void) {
    // Включение тактирования
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_SPI1EN;

    // Настройка SPI1 пинов: PA5=SCK, PA7=MOSI
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5 |
                    GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5 |  // PA5: Alternate Push-Pull, 50MHz
                   GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7);  // PA7: Alternate Push-Pull, 50MHz

    // Настройка управляющих пинов: PA1=DC, PA4=CS, PA2=RES (Output, Push-Pull, 2MHz)
    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1 |
                    GPIO_CRL_CNF4 | GPIO_CRL_MODE4 |
                    GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
    GPIOA->CRL |= (GPIO_CRL_MODE1_0 |   // PA1: Output, 2MHz
                   GPIO_CRL_MODE4_0 |   // PA4: Output, 2MHz
                   GPIO_CRL_MODE2_0);   // PA2: Output, 2MHz

    // Установка начальных состояний
    GPIOA->BSRR = GPIO_BSRR_BS4;  // CS = 1
    GPIOA->BSRR = GPIO_BSRR_BS2;  // RES = 1

    // Настройка SPI1
    SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_CPHA |  // Полярность и фаза
                SPI_CR1_MSTR |                 // Режим мастера
                SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | // Baudrate = fPCLK/256
                SPI_CR1_SSM | SPI_CR1_SSI;     // Программное управление CS

    SPI1->CR1 |= SPI_CR1_SPE; // Включение SPI
}

int main(void) {
    // SystemInit() вызывается автоматически до входа в main()

    // Инициализация нашей периферии
    Peripherals_Init();

    // Инициализация дисплея
    SSD1306_Init();

    // Отображение тестового изображения
    SSD1306_TestPattern();

    while(1) {
        // Основной цикл
    }
}