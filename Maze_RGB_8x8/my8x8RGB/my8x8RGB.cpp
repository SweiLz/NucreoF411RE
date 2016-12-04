#include "mbed.h"
#include "my8x8RGB.h"
/**************Define*************************/
DigitalOut RGB_RST(A2);
DigitalOut RGB_LAT(A1);
DigitalOut RGB_SB(A0);

/*DigitalIn nn0(D7);
DigitalIn nn1(D2);
DigitalIn nn2(D6);*/
SPI RGB_SPI(PB_15,PB_14,PB_13);
BusOut RGB_CP(D8,D9,D10,D11,D12,D13,D3,D4);
uint8_t RGBi= 0,RGBj= 0,RGBk= 0;
uint8_t RedWB=0,GreenWB=10,BlueWB=10; // percent | R,G,B calibrate change it if some color brighter than other
uint8_t display[8][8][4]= {0};
uint8_t ii,jj,kk;
/*******************************************/
void color_choose(int oasis[8][8][4])
{
    for(ii=0; ii<=7; ii++) {
        for(jj=0; jj<=7; jj++) {
            for(kk=0; kk<=7; kk++) {
                display[ii][jj][kk]=oasis[ii][jj][kk];
            }
        }
    }
}
void INITRGB(char in[3])
{
    int wd;
    RGB_RST =0;
    RGB_RST =1;
    RGB_SPI.frequency(20000000);
    RGB_SPI.format(8,1);
    RGB_LAT = 0;
    RGB_SB  =0;
    int l=0;

    for (int i = 0; i<8; i++) { // Whitebalance......**important
        for (int j = 0; j<3; j++) {
            for (int k = 0; k<6; k++) {
           //     wd+=((in[j]<<k)&0b00100000)?1:0;
                wd<<1;
                l++;
                if(l>=7) {
                    RGB_SPI.write(255);
                    l=0;
                }
            }
            //RGB_SPI.write(255);
        }
    }

    RGB_LAT = 1;
    RGB_LAT = 0;

    RGB_SPI.format(8,0);
}

void Showscreen()
{

    RGB_SB  =1;
    for( RGBi=0; RGBi<8; RGBi++) {
        for( RGBk=0; RGBk<8; RGBk++) {
            for( RGBj=0; RGBj<3; RGBj++) {
                RGB_SPI.write(display[RGBi][RGBk][RGBj]);
            }

        }
        RGB_CP = 0;
        wait_us(15);
        RGB_LAT = 1;
        RGB_LAT = 0;

        RGB_CP = (0x01<<RGBi);

    }
}
/*
void screen_color(uint8_t R,uint8_t G,uint8_t B)
{

    for(uint8_t Colum=0; Colum<8; Colum++) {
        for(uint8_t Row=0; Row<8; Row++) {
            display[Colum][Row][2]=R*RedWB/100;
            display[Colum][Row][1]=G;
            display[Colum][Row][0]=B*BlueWB/100;
        }
    }

}
void screen_off()
{

    for(uint8_t Colum=0; Colum<8; Colum++) {
        for(uint8_t Row=0; Row<8; Row++) {
            display[Colum][Row][2]=0;
            display[Colum][Row][1]=0;
            display[Colum][Row][0]=0;
        }
    }
    Showscreen();
}


void RGB_led(uint8_t Colum,uint8_t Row,char color,uint8_t brightness)
{
    uint8_t c;
    switch (color) {
        case 'R':
            c=2;
            break;
        case 'G':
            c=1;
            break;
        case 'B':
            c=0;
            break;
    }
    display[Colum][Row][c]=brightness;
}


void rgb_point(uint8_t Colum,uint8_t Row,uint8_t R,uint8_t G,uint8_t B)
{
    display[Colum][Row][2]=R*RedWB/100;
    display[Colum][Row][1]=G;
    display[Colum][Row][0]=B*BlueWB/100;
}

*/






































