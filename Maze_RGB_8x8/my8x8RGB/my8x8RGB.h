
#ifndef my8x8RGB_H
#define my8x8RGB_H


void INITRGB(char in[3]); //whitebalance define
void color_choose(int oasis[8][8][4]);
void Showscreen(); //display
void screen_color(uint8_t R,uint8_t G,uint8_t B); // all dot on screen
void screen_off();//all led brightness=0
void RGB_led(uint8_t Colum,uint8_t Row,char  color,uint8_t brightness); ////on LED each point | color = 'R','G','B' | **brightness 0-255 => rgb_point(1,1,'R',255);
void rgb_point(uint8_t Colum,uint8_t Row,uint8_t R,uint8_t G,uint8_t B); // 16m_colorful *yellow,purple,pink. etc.
#endif // my8x8RGB_H

