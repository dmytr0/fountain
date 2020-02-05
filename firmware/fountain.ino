#define RESET_CLOCK 0 				// сброс часов на время загрузки прошивки (для модуля с несъёмной батарейкой). Не забудь поставить 0 и прошить ещё раз!
#define BRIGHT_THRESHOLD 200 		// величина сигнала освещения, ниже которой включится подсветка (0-1023)
#define PUMP_ON_HOUR 7 				// час включения насоса
#define PUMP_OFF_HOUR 22 			// час выключения насоса
#define NUM_LEDS 16					// колличество светодиодов(секций) в адресной ленте
#define BRIGHTNESS 255				// яркость ленты (0-255)
#define EFFECT_SPEED 5				// скорость еффектов

#define PHOTO A3 					// пин фоторезистора
#define PUMP 6 						// пин насоса
#define LED 13 						// пин LED подсветки


#include "RTClib.h"
RTC_DS3231 rtc;
#include <GyverTimer.h>
GTimer_ms clockTimer(1000);
GTimer_ms timeChecker(20000);
GTimer_ms ledTimer(EFFECT_SPEED);

#include "FastLED.h"
CRGB leds[NUM_LEDS];

byte counter;
int8_t year, month, day, hrs, mins, secs;
boolean pumpOn = false;
boolean ledOn = false;

void setup()
{
	Serial.begin(115200);
	pinMode(PUMP, OUTPUT);
	pinMode(LED, OUTPUT);
	boolean status = true;
	if (rtc.begin())
	{
		Serial.println("RTC OK");
	} else {
		Serial.println("RTC ERROR");
		status = false;
	}
	// Настройка RTC
	if (RESET_CLOCK || rtc.lostPower()) {
		DateTime currentTime = DateTime(F(__DATE__), F(__TIME__));
		rtc.adjust(currentTime);
	}
	// Время
	now = rtc.now();
	secs = now.second();
	mins = now.minute();
	hrs = now.hour();
	day = now.day();
	month = now.month();
	year = now.year();


	FastLED.addLeds<WS2812, LED, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  	FastLED.setBrightness(BRIGHTNESS);
  	
}

void loop()
{
	if (clockTimer.isReady()) clockTick();
	if (ledTimer.isReady()) ledEffect();
	if (timeChecker.isReady()) {
		checkPumpTime();
		checkLed();
	}
}

void clockTick() {
	secs++;
	if (secs > 59)
	{
		// каждую минуту
		secs = 0;
		mins++;
	}
	if (mins > 59)
	{
		// каждый час
		now = rtc.now();
		secs = now.second();
		mins = now.minute();
		hrs = now.hour();
		day = now.day();
		month = now.month();
		year = now.year();
		if (hrs > 23)
		{
			hrs = 0;
		}
	}
}

void checkPumpTime() {
	if (hour >= PUMP_ON_HOUR && hour < PUMP_OFF_HOUR) {
		pumpTurn(true);
	} else {
		pumpTurn(false);
	}
}

void checkLed() {
	int illumination = analogRead(PHOTO);
	if (illumination < BRIGHT_THRESHOLD) {
		ledTurn(true);
	} else {
		ledTurn(false);
	}
}

void ledTurn(boolean turnOn) {
	if(turnOn && !ledOn) {
		ledOn = turnOn;
		ledTimer.reset();
		ledTimer.start();
		Serial.println("Led was turned on");
	}

	if(!turnOn && ledOn) {
		ledOn = turnOn;
		ledTimer.stop();
		ledBlackColor();
		Serial.println("Led was turned off");
	}
}

void pumpTurn(boolean turnOn) {
	if (turnOn && !pumpOn) {
		pumpOn = turnOn;
		digitalWrite(PUMP, HIGH);
		Serial.println("Pump was turned on");
	}

	if (!turnOn && pumpOn) {
		pumpOn = turnOn;
		digitalWrite(PUMP, LOW);
		Serial.println("Pump was turned off");
	}
}



// Пример эффекта отсюда https://alexgyver.ru/ws2812_guide/
void ledEffect() {
	for (int i = 0; i < NUM_LEDS; i++ ) {         	// от 0 до первой трети
    	leds[i] = CHSV(counter + i * 2, 255, 255);  // HSV. Увеличивать HUE (цвет)
    	// умножение i уменьшает шаг радуги
  	}
  	counter++;        								// counter меняется от 0 до 255 (тип данных byte)
  	FastLED.show();
}

// Имитация выключенной ленты, можно переделать на отключение питания через транзистор или реле
void ledBlackColor() {
	for (int i = 0; i < NUM_LEDS; i++ ) {         	
    	leds[i] = CHSV(0, 0, 0);  
  	}
}
