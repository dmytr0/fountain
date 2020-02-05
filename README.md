# fountain
*Arduino fountain manager*

## Настройки

    #define RESET_CLOCK 0             // сброс часов на время загрузки прошивки (для модуля с несъёмной батарейкой). Не забудь поставить 0 и прошить ещё раз!
    #define BRIGHT_THRESHOLD 200      // величина сигнала освещения, ниже которой включится подсветка (0-1023)
    #define PUMP_ON_HOUR 7            // час включения насоса
    #define PUMP_OFF_HOUR 22          // час выключения насоса
    #define NUM_LEDS 16               // колличество светодиодов(секций) в адресной ленте
    #define BRIGHTNESS 255            // яркость ленты (0-255)
    #define EFFECT_SPEED 5            // скорость еффектов


## Пины

	#define PHOTO A3                // пин фоторезистора
	#define PUMP 6                  // пин насоса
	#define LED 13                  // пин LED подсветки
