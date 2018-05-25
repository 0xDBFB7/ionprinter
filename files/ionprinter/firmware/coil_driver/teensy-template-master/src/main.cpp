#include "WProgram.h"

void zero_all(){
		digitalWrite(3,0);
		digitalWrite(4,0);
		digitalWrite(9,0);
		digitalWrite(10,0);
}

extern "C" int main(void)
{
	pinMode(3,OUTPUT);
	pinMode(4,OUTPUT);
	pinMode(9,OUTPUT);
	pinMode(10,OUTPUT);
//	analogWrite(3,127);
//	analogWrite(10,127);
	while (1) {
		zero_all();
		digitalWrite(3,1);
		digitalWrite(4,0);
		digitalWrite(9,0);
		digitalWrite(10,1);
		delay(1);
		zero_all();
		digitalWrite(3,0);
		digitalWrite(4,1);
		digitalWrite(9,1);
		digitalWrite(10,0);
		delay(1);

	}

}

