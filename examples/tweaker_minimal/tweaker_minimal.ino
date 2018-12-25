
typedef void (*UserCallback)(void);

class TweakCallbackOwnerClass {
	public:
		virtual void tweakHandler() = 0;
};

struct TweakCallbackItem
{
    TweakCallbackOwnerClass* obj;
    UserCallback* method;
};


class Tweak {
	public:

		static Tweak& me() {
	        static Tweak  instance;
	        return instance;
	    }

		static int8_t attachMemberCallback(TweakCallbackOwnerClass* obj, uint16_t milliseconds) {
			return me().attachMemberCallback(obj, milliseconds, false);
		}

		static void timerHandler(void) {
			me().walkCallback();
		}


	private:

		Tweak();
	    Tweak(const Tweak&);
	    Tweak& operator=(Tweak&);

		int8_t attachCallback(void (*callbackFunction)(), uint16_t period, bool isUsScale) {
			if (this->callbackArraySize == 127)
				return -1;

			TweakCallbackItem item;
			item.obj = nullptr;
			item.method = &callbackFunction;

			TweakCallbackItem *tmpCallbackArray = (TweakCallbackItem*) realloc(this->callbackArray, (sizeof * callbackArray) * (this->callbackArraySize + 1));

			if (tmpCallbackArray) {
				this->callbackArray = tmpCallbackArray;
				this->callbackArray[this->callbackArraySize] = *item;

				this->callbackArraySize++;
				this->lock = false;

				return this->callbackArraySize - 1;
			}
		}

		void walkCallback(bool isUsScale) {
			for(uint8_t i = 0; i < this->callbackArraySize; i++) {
				if (this->callbackArray[i].obj != nullptr) {
					this->callbackArray[i].obj->chronosHandler();
				} else if (this->callbackArray[i].method != nullptr) {
					(*this->callbackArray[i].method)();
			}
		}

		volatile uint8_t callbackArraySize = 0;
		TweakCallbackItem *callbackArray;
};


/***************************************************************/


void setup() {
  Tweak::attachCallback(myTimerHandler2, 5000);
}

void loop() {
	for (uint8_t i = 0; i < 1000; i++) {
		Tweak::timerHandler();
	}
}

void myTimerHandler2(void) {
  volatile static uint8_t i = 0;
  i++;
}
