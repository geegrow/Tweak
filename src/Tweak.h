/*!
 * @file Tweak.h
 *
 * This is a library which allows to add more than one user defined functions,
 * lambda functions and also class members methods as callbacks with Timer1.
 * https://geegrow.ru
 *
 * @section author Author
 * Written by GeeGrow
 *
 * @section license License
 * BSD license, all text here must be included in any redistribution.
 *
 */
#ifndef TWEAK_H
#define TWEAK_H
#include "Arduino.h"

/* Uncomment this to use TimerFour instead of Timer1 (only on Mega2560) */
// #define USE_TIMER_FOUR

#ifndef USE_TIMER_FOUR
#include <TimerOne.h>
#else
#include <TimerFour.h>
#endif /* USE_TIMER_FOUR */

typedef void (*UserCallback)(void);

/**************************************************************************/
/*!
    @brief    Size (in Us) of smalles possible tick
*/
/**************************************************************************/
#define TICK_SIZE 100

/**************************************************************************/
/*!
    @brief    Size of array which stores user defined callbacks
*/
/**************************************************************************/
#define DEFAULT_CALLBACK_ARRAY_SIZE 8

/**************************************************************************/
/*!
    @brief    Class that must be inherited by a class, which includes the
              function to be callback'ed
*/
/**************************************************************************/
class TweakCallbackOwnerClass {
    public:
        virtual void tweakHandler() = 0;
};

/**************************************************************************/
/*!
    @brief    Struct which contains properties of user defined callback
*/
/**************************************************************************/
struct TweakCallbackItem
{
    TweakCallbackOwnerClass* obj = nullptr;
    UserCallback method = nullptr;
    uint16_t counter = 0;
    uint16_t limit = 0;
    uint8_t id = 0;
    bool isUsScale = false;
    bool isActive = false;
};

/**************************************************************************/
/*!
    @brief    Class that stores functions for using user defined callbacks
*/
/**************************************************************************/
class Tweak {
    public:

        static Tweak& me() {
            static Tweak  instance;
            return instance;
        }

        template < typename NonCaptureLambda >
        static int8_t attachMsLambda(NonCaptureLambda lambda, uint16_t milliseconds) {
            return me().attachCallback(lambda, milliseconds, false);
        }

        template < typename NonCaptureLambda >
        static int8_t attachUsLambda(NonCaptureLambda lambda, uint16_t microseconds) {
            uint8_t id = me().attachCallback(lambda, microseconds, true);

            if (id >= 0) {
                me().usCallbackCount++;
            }

            return id;
        }

        static int8_t attachMsCallback(void (*callbackFunction)(), uint16_t milliseconds);
        static int8_t attachUsCallback(void (*callbackFunction)(), uint16_t microseconds);
        static int8_t attachMsMemberCallback(TweakCallbackOwnerClass* obj, uint16_t milliseconds);
        static int8_t attachUsMemberCallback(TweakCallbackOwnerClass* obj, uint16_t microseconds);
        static void delay(uint16_t delay_ms);
        static void timerHandler();
        static void setCallbackArraySize(uint8_t size);
        static void setCallbackActive(uint8_t id, bool setActive);

    private:

        Tweak();
        Tweak(const Tweak&);
        Tweak& operator=(Tweak&);

        void processTimerHandler();
        int8_t attachLambda(void (*callbackFunction)(), uint16_t period, bool isUsScale);
        int8_t attachMemberCallback(TweakCallbackOwnerClass* obj, uint16_t period, bool isUsScale);
        int8_t attachCallback(void (*callbackFunction)(), uint16_t period, bool isUsScale);
        int8_t pushCallbackItem(TweakCallbackItem *item);
        void walkCallback(bool isUsScale);
        int8_t spawnNewId();

        volatile bool lock = false;
        volatile uint8_t usCounter = 0; // Actually it's not uS counter, it is tick counter where 1 tick = 100us
        volatile uint8_t usCallbackCount = 0;

        volatile uint16_t delayLimit = 0;
        volatile uint16_t delayCounter = 0;

        volatile uint8_t callbackArraySize = 0;
        volatile uint8_t callbackArrayNextID = 0;
        TweakCallbackItem *callbackArray = nullptr;
};

#endif /* TWEAK_H */
