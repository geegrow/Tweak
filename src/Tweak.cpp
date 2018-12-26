/*!
 * @file Tweak.cpp
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
#include "Tweak.h"

/**************************************************************************/
/*!
    @brief    Instantiate and configure Tweak, which uses TimerOne
*/
/**************************************************************************/
Tweak::Tweak() {
    Timer1.initialize(100);
    Timer1.attachInterrupt(timerHandler);
}

/**************************************************************************/
/*!
    @brief    Public interface for processTimerHandler function
*/
/**************************************************************************/
void Tweak::timerHandler(void) {
    me().processTimerHandler();
}

/**************************************************************************/
/*!
    @brief    Attach callback to a common function with Ms period
    @param    callbackFunction  Pointer to a function to be called after period estimates
    @param    milliseconds      Period (in Ms)
    @return   ID of callback in array
*/
/**************************************************************************/
int8_t Tweak::attachMsCallback(void (*callbackFunction)(), uint16_t milliseconds) {
    return me().attachCallback(callbackFunction, milliseconds, false);
}

/**************************************************************************/
/*!
    @brief    Attach callback to a common function with Us period
    @param    callbackFunction  Pointer to a function to be called after period estimates
    @param    milliseconds      Period (in Us)
    @return   ID of callback in array
*/
/**************************************************************************/
int8_t Tweak::attachUsCallback(void (*callbackFunction)(), uint16_t microseconds) {
    int8_t id = me().attachCallback(callbackFunction, microseconds, true);

    if (id >= 0) {
        me().usCallbackCount++;
    }

    return id;
}

/**************************************************************************/
/*!
    @brief    Attach callback to a member function with Ms period
    @param    obj           Pointer to a object of the class. The class MUST be
                            inherited from TweakCallbackOwnerClass and MUST include
                            function tweakHandler, which will be called after
                            timer estimation
    @param    milliseconds  Period (in Ms)
    @return   ID of callback in array
*/
/**************************************************************************/
int8_t Tweak::attachMsMemberCallback(TweakCallbackOwnerClass* obj, uint16_t milliseconds) {
    return me().attachMemberCallback(obj, milliseconds, false);
}

/**************************************************************************/
/*!
    @brief    Attach callback to a member function with Us period
    @param    obj           Pointer to a object of the class. The class MUST be
                            inherited from TweakCallbackOwnerClass and MUST include
                            function tweakHandler, which will be called after
                            timer estimation
    @param    milliseconds  Period (in Us)
    @return   ID of callback in array
*/
/**************************************************************************/
int8_t Tweak::attachUsMemberCallback(TweakCallbackOwnerClass* obj, uint16_t microseconds) {
    int8_t id = me().attachMemberCallback(obj, microseconds, true);

    if (id >= 0) {
        me().usCallbackCount++;
    }

    return id;
}

/**************************************************************************/
/*!
    @brief    Delay which doesn't "freeze" device
    @param    delay_ms    Time of delay in Ms
*/
/**************************************************************************/
void Tweak::delay(uint16_t delay_ms) {
    me().delayLimit = delay_ms;

    while(me().delayCounter < me().delayLimit) {
        __asm__ __volatile__ (" nop\n");
    }

    me().delayCounter = 0;
    me().delayLimit = 0;
}

/**************************************************************************/
/*!
    @brief    Set size of array which stores callbacks
    @param    size    Desired size of array
    @note     Default size of array is defined by DEFAULT_CALLBACK_ARRAY_SIZE
*/
/**************************************************************************/
void Tweak::setCallbackArraySize(uint8_t size){
    if (size > 127){
        Serial.println(F("Max array size is 127"));
        return;
    }
    me().callbackArraySize = size;
    if (!me().callbackArray)
        me().callbackArray = new TweakCallbackItem[size];
}

/**************************************************************************/
/*!
    @brief    Set active/deactive status of selected array
    @param    id         ID of callback in array, which is needed to change status
    @param    setActive  Set selected callback active or not (true/false)
*/
/**************************************************************************/
void Tweak::SetCallbackActive(uint8_t id, bool setActive){
    if (Tweak::me().callbackArray[id].isActive != setActive)
        Tweak::me().callbackArray[id].isActive = setActive;
}

/**************************************************************************/
/*!
    @brief    "Step" ticks and calls handler
*/
/**************************************************************************/
void Tweak::processTimerHandler(void) {
    if (this->lock == true)
        return;

    if (this->usCallbackCount > 0)
        this->walkCallback(true);

    this->usCounter++;
    if (this->usCounter == 10) {
        this->usCounter = 0;
        this->walkCallback(false);
        if (this->delayLimit != 0
            && this->delayCounter < this->delayLimit
        ) {
            this->delayCounter++;
        }
    }
}

/**************************************************************************/
/*!
    @brief    Attach common function or lambda to callback item
    @param    callbackFunction  Pointer to a function to be called after period estimates
    @param    milliseconds      Period (in Us)
    @param    isUsScale         Selects which min tick size to use for this callback
    @return   ID of callback in array
*/
/**************************************************************************/
int8_t Tweak::attachCallback(void (*callbackFunction)(), uint16_t period, bool isUsScale) {
    if (!this->callbackArray){
        Tweak::me().setCallbackArraySize(DEFAULT_CALLBACK_ARRAY_SIZE);
        Serial.print(F("Tweak: callback array size is set to default "));
        Serial.println(this->callbackArraySize, DEC);
    }

    if (this->callbackArraySize == 127)
        return -1;

    TweakCallbackItem item;
    item.obj = nullptr;
    item.method = callbackFunction;
    item.id = this->spawnNewId();
    item.counter = 0;
    item.limit = period;
    item.isUsScale = isUsScale;
    item.isActive = true;

    if (item.id < 0)
        return -1;

    return this->pushCallbackItem(&item);
}

/**************************************************************************/
/*!
    @brief    Attach member function to callback item
    @param    obj           Pointer to a object of the class. The class MUST be
                            inherited from TweakCallbackOwnerClass and MUST include
                            function tweakHandler, which will be called after
                            timer estimation
    @param    milliseconds  Period (in Us)
    @param    isUsScale     Selects which min tick size to use for this callback
    @return   ID of callback in array
*/
/**************************************************************************/
int8_t Tweak::attachMemberCallback(TweakCallbackOwnerClass* obj, uint16_t period, bool isUsScale) {
    if (!this->callbackArray){
        Tweak::me().setCallbackArraySize(DEFAULT_CALLBACK_ARRAY_SIZE);
        Serial.print(F("Tweak: callback array size is set to default "));
        Serial.println(this->callbackArraySize, DEC);
    }

    if (this->callbackArraySize == 127)
        return -1;

    TweakCallbackItem item;
    item.obj = obj;
    item.method = nullptr;
    item.id = this->spawnNewId();
    item.counter = 0;
    item.limit = period;
    item.isUsScale = isUsScale;
    item.isActive = true;

    if (item.id < 0)
        return -1;

    return this->pushCallbackItem(&item);
}

/**************************************************************************/
/*!
    @brief    Store callback item in array and get ID
    @param    item    Callback item to be stored
    @return   ID of callback in array
*/
/**************************************************************************/
int8_t Tweak::pushCallbackItem(TweakCallbackItem *item) {
    this->lock = true;
    if (item->id > this->callbackArraySize){
        this->lock = false;
        return -1;
    }

    this->callbackArray[item->id] = *item;
    this->lock = false;
    return item->id;
}

/**************************************************************************/
/*!
    @brief    Check every callback in array if it needs to be called or not
    @param    isUsScale    Selects which min tick size to use for this callback
*/
/**************************************************************************/
void Tweak::walkCallback(bool isUsScale) {
    for(uint8_t i = 0; i < this->callbackArraySize; i++) {
        if (this->callbackArray[i].isActive) {
            if (this->callbackArray[i].isUsScale == isUsScale) {
                this->callbackArray[i].counter += isUsScale ? TICK_SIZE : 1;

                if (this->callbackArray[i].limit != 0
                    && this->callbackArray[i].counter >= this->callbackArray[i].limit
                ) {
                    this->callbackArray[i].counter = 0;

                    if (this->callbackArray[i].obj != nullptr) {
                        this->callbackArray[i].obj->tweakHandler();
                    } else if (this->callbackArray[i].method != nullptr) {
                        (this->callbackArray[i].method)();
                    }
                }
            }
        }
    }
}

/**************************************************************************/
/*!
    @brief    Get new ID number for callback item in array
*/
/**************************************************************************/
int8_t Tweak::spawnNewId() {
    int8_t id = this->callbackArrayNextID;

    if (id >= this->callbackArraySize){
        return -1;
    }

    this->callbackArrayNextID++;
    return id;
}
