#include "mbed.h"
#include "LCDTaonoi.h"
#include <string>

Serial pc(SERIAL_TX,SERIAL_RX);
LCDTaonoi lcd(I2C_SDA, I2C_SCL);

int main()
{
    pc.baud(115200);
    lcd.clear();
    pc.printf("====================\nHello World\n====================\n");
    string str[] = {"***** WELCOME! *****"," AUTO PANCAKE MAKER ","","   PRESS TO START   "};
    for(int i=0; i<4; i++) {
        lcd.setCursor(0,i);
        lcd.print(str[i].c_str());
    }

    while(1) {
        wait(0.05);
    }
}