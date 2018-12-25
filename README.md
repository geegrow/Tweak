# Tweak

This library is based on TimerOne and allows to add more than one user defined
functions, lambda functions and also class members methods as callbacks with Timer1.

It may be very useful if you want to call (by Timer1) functions or classes which
are placed in different parts of your code.

Attention! This library uses dynamic array for storing of created callbacks,
so you must control memory usage yourself.
Be noticed that you can only add new callbacks but not delete them. You can just
activate/deactivate already created callbacks.

For more information you can visit https://github.com/geegrow/Tweak

--------------------------------------------------------------------

"___________________________________" <--   This is a place for very motivating
                                            and enthusiastic slogan!

Please visit our store https://geegrow.ru and check out for some cool stuff!



<!-- START COMPATIBILITY TABLE -->

## Compatibility

MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
Atmega328 @ 16MHz  |              |              |     X       |
Atmega328 @ 12MHz  |              |              |     X       |
Atmega32u4 @ 16MHz |      X       |              |             |
ESP8266            |              |              |     X       |
Atmega2560 @ 16MHz |              |              |     X       |
ATSAM3X8E          |              |              |     X       |
ATSAM21D           |              |              |     X       |
Intel Curie @ 32MHz|              |              |     X       |
STM32F2            |              |              |     X       |

  * ATmega328 @ 16MHz : Arduino UNO
  * ATmega328 @ 12MHz : Adafruit Pro Trinket 3V
  * ATmega32u4 @ 16MHz : Arduino Leonardo, Arduino Micro, Arduino Yun, Geegrow DaVinci
  * ESP8266 :
  * ATmega2560 @ 16MHz : Arduino Mega
  * ATSAM3X8E : Arduino Due
  * ATSAM21D : Arduino Zero, M0 Pro

<!-- END COMPATIBILITY TABLE -->
