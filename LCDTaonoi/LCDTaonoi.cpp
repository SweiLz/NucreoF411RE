#include "LCDTaonoi.h"



LCDTaonoi::LCDTaonoi(PinName _SDA, PinName _SCL , int _addr):lcd(_SDA,_SCL),addr(_addr)
{
    wait_ms(55);

    Command(0x01);//CLS
    wait_ms(15);
    
    //Begin
    Command(0x3);
    wait(0.005);
    Command(0x3);
    wait_us(100);
    Command(0x3);
    Command(0x2);
    Command(0x28); //Funtionset DL(4bit,8bit) = 0 N(2line,4line) = 1 F(5x8,5x10) = 0
    //Command(0x0);
    //Command(0xC);
    Command(0x0C); //Display on/off D(Display) = 1 C(Cursor) = 0 B(Blinking) = 0
    //Command(0x0);
    //Command(0x6);
    Command(0x06); //Entry mode set I/D(Decrement,Increment) = 1 S(No shift) = 0
    
    
}
void LCDTaonoi::clear()
{
    Command(0x01);
    wait_ms(5);
    setCursor(0,0);
}
void LCDTaonoi::Command (char value, int Mode )
{
    char _command[2];
    char Temp[2];
    char _value[2];
    

    Temp[0] = 0x0C;
    Temp[1] = 0x08;
    if(Mode == 0) {
        _command[0] = 0x0C;
        _command[1] = 0x08;
    } else {
        _command[0] = 0x0D;
        _command[1] = 0x09;
    }
    _value[0] = value & 0xF0;
    _value[0] = _value[0] | _command[0];
    lcd.write(addr,&_value[0],1);
    wait_us(40);
    _value[0] = value & 0xF0;
    _value[0] = _value[0] | _command[1];
    lcd.write(addr,&_value[0],1);
    wait_us(40);

    //lcd.write(addr,&Temp[0],1);
    //wait_us(40);


    _value[1] = value << 4;
    _value[1] = _value[1] | _command[0];
    lcd.write(addr,&_value[1],1);
    wait_us(40);
    _value[1] = value << 4;
    _value[1] = _value[1] | _command[1];
    lcd.write(addr,&_value[1],1);
    wait_us(40);
    lcd.write(addr,&Temp[0],1);
}

int LCDTaonoi::address(int column, int row)
{

    switch (row) {
        case 0:
            return 0x80 + column;
        case 1:
            return 0xc0 + column;
        case 2:
            return 0x94 + column;
        case 3:
            return 0xd4 + column;
    }
    return 0;
}

void LCDTaonoi::setCursor (int column, int row)
{
    int adr;
    adr = address(column,row);
    Command(adr);
    
}

void LCDTaonoi::print (string Data)
{
    int8_t i=0;
    while(Data[i]!='\0')
    {
        Command(Data[i++],1);
    }
}