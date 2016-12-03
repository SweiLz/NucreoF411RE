#ifndef LCDTAONOI_H
#define LCDTAONOI_H
#include "mbed.h"
#include <string>

class LCDTaonoi
{
public:
	LCDTaonoi(PinName _SDA, PinName _SCL , int _addr = 0x4E );
	void Command (char value, int Mode = 0);
	void print (string Data);
	int  address(int column, int row);
	void setCursor (int column, int row);
	void clear();
//void Command (const char* value, int Mode )
//	~LCDTaonoi();
private:
	const int addr ;
	I2C lcd;

};

#endif