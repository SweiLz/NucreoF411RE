#include "mbed.h"
#include "SDFileSystem.h"
#include "LCDTaonoi.h"
#include "QEI.h"
#include <string>
#include <vector>


Serial pc(SERIAL_TX, SERIAL_RX);
Serial device(D8,D2);
SDFileSystem sd(D11, D12, D13, D10,"sd");
LCDTaonoi lcd(I2C_SDA, I2C_SCL);
QEI encoder(A0, A1);

Timer timer;
Ticker TimetickTemp;
Ticker TimetickDir;
Ticker TimetickLCD;

void getTemperature();
DigitalInOut TempSensor(D4);
DigitalOut TempStatus(D3);


DigitalIn btn(A2);
void getDir();

int SelectMenu();
void ListFileNames();
void displayData();

void ProcessFile(string filename);
void ProcessCmd(string cmd);
string stringData(char ch,string cmd);
int FullSize=0,NowSize=0;

vector<string> strDisplay;
vector<string> filenames;
vector<string>::iterator it;


int item_index,list_index,last_index=1;
int direction,select=0,select2=0,select3=0;
float percent = 0;
string filePrint="";
int run=0;
int temperature=0;

int main()
{
    
    pc.baud(115200);
    device.baud(115200);
    
    
    TimetickTemp.attach(&getTemperature, 0.5);
    TimetickDir.attach(&getDir, 0.1);
    TimetickLCD.attach(&displayData, 0.5);
    encoder.reset();
   /* while(1)
    {
        if(device.readable())
        {
            pc.putc(device.getc());
        }
        if(pc.readable())
        {
            device.putc(pc.getc());
        }
    }*/
    ListFileNames();

    ///////////////////////////////// START ///////////////////////////
    pc.printf("====================\nHello World\n====================\n");
    string str[] = {"***** WELCOME! *****"," AUTO PANCAKE MAKER ","","   PRESS TO START   "};
    for(int i=0; i<4; i++) {
        lcd.setCursor(0,i);
        lcd.print(str[i].c_str());
    }
    while(btn.read());
    while(!btn.read());
    //////////////////////////////////////////////////////////////////



    ///////////////////////// MAIN MENU //////////////////////////////
modemenu:
    strDisplay.clear();
    strDisplay.push_back("PRINT FILE");
    //strDisplay.push_back("Mode 2");
    //strDisplay.push_back("Mode 3");
    //strDisplay.push_back("Mode 4");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("*** SELECT  MODE ***");
    select=SelectMenu();

    switch(select) {
        case 1:
printmenu:
            ////////////////// Print File Name List /////////////////////
            strDisplay.clear();
            for(int i=0; i<filenames.size(); i++) {
                strDisplay.push_back(filenames[i].c_str());
            }
            strDisplay.push_back("<< BACK ");

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("**** PRINT FILE ****");
            select=SelectMenu();

            if(select==strDisplay.size()) {
                goto modemenu;
            } else {
backgroundmenu:
                ///////////////////////////// Background Menu ////////////////
                strDisplay.clear();
                strDisplay.push_back("SQUARE");
                strDisplay.push_back("CIRCLE");
                strDisplay.push_back("ELLIPS");
                strDisplay.push_back("<< BACK ");
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("**** BACKGROUND ****");
                select2=SelectMenu();

                if(select2==strDisplay.size()) {
                    goto printmenu;
                } else {
                    ////////////////////////////////////// Size /////////////////////////
                    strDisplay.clear();
                    strDisplay.push_back("FIT");
                    strDisplay.push_back("NORMAL");
                    strDisplay.push_back("LARGE");
                    strDisplay.push_back("<< BACK ");

                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("******* SIZE *******");
                    select3=SelectMenu();
                    if(select3==4) {
                        goto backgroundmenu;
                    }
                }
            }
            break;
    }


    //pc.printf("%s\n",strDisplay[select3-1].c_str());


    pc.printf("Print File....\n");

    lcd.clear();
    run=1;
    ProcessFile(filenames[select-1].c_str());
    //ProcessFile(filenames[1].c_str());
    //pc.printf("Goodbye World\n");
    run=2;
    device.printf("$%d\r",select2);
    while(1) {
        if(device.readable()) {
            char ch = device.getc();
            pc.putc(ch);
            if(ch=='$') {
                
                break;
            }
        }
    }
    pc.printf("Finish\n");
    run=0;
    lcd.clear();
    pc.printf("Goodbye World\n");
}


void ProcessFile(string filename)
{
    filePrint = filename;
    NowSize=0;
    percent=0;
    bool comment = false;
    string cmd="";
    string readPath = "/sd/FileToPrint/" + filename;

    FILE *fInput = fopen(readPath.c_str(), "r");

    if (fInput != NULL) {
        fseek( fInput, 0, SEEK_END );
        FullSize = ftell(fInput);
        rewind(fInput);
        //pc.printf("@PC Name :%s\n",filename.c_str());
        //pc.printf("@PC Size :%d\n",FullSize);

        int8_t ch = fgetc(fInput);
        while(ch != EOF) {
            if(ch != '\n') {
                if (ch == '(') comment = true;
                if (!comment) cmd.push_back(ch);
                if (ch == ')') comment = false;
            } else {
                wait_us(100);
                if(cmd.size()>1) {
                    if(!cmd.find("G0 ")||!cmd.find("G1 ")||!cmd.find("G2 ")||!cmd.find("G3 ")) {
                        percent = float(NowSize)/float(FullSize)*100;
                        ProcessCmd(cmd);
                    }
                }
                cmd.clear();
            }
            ch = fgetc(fInput);
            NowSize++;
        }
        percent = 100;
        filePrint.clear();
    }
    fclose(fInput);
}

void ProcessCmd(string cmd)
{
    string tempCmd="",sendCmd="";

    if(!cmd.find("G0 ") || !cmd.find("G1 "))sendCmd = "G0 ";
    else if(!cmd.find("G2 "))sendCmd = "G2 ";
    else if(!cmd.find("G3 "))sendCmd = "G3 ";

    if(cmd.find('X',0)!= std::string::npos)sendCmd.append(stringData('X',cmd));
    if(cmd.find('Y',0)!= std::string::npos)sendCmd.append(stringData('Y',cmd));
    if(cmd.find('Z',0)!= std::string::npos)sendCmd.append(stringData('Z',cmd));
    if(cmd.find('I',0)!= std::string::npos)sendCmd.append(stringData('I',cmd));
    if(cmd.find('J',0)!= std::string::npos)sendCmd.append(stringData('J',cmd));

    device.printf("%s\r",sendCmd.c_str());
    pc.printf("Device : %s\n\r",sendCmd.c_str());
    wait(1.0);
    /*while(1) {
        if(device.readable()) {
            if(device.getc()=='#') {
                break;
            }
        }
    }*/
}

string stringData(char ch,string cmd)
{
    size_t pos = cmd.find(ch,0);
    string tempCmd,temp = cmd.substr(pos+1,8);
    float number = atof(temp.c_str());
    if(ch=='Z')number = number>0?1.0:0.0;
    sprintf(&tempCmd[0],"%c%.3f ",ch,number);
    return tempCmd.c_str();
}

void ListFileNames()
{
    string path = "/sd/FileToPrint";
    struct dirent *dirp;
    DIR *dp = opendir(path.c_str());
    while((dirp = readdir(dp)) != NULL) {
        filenames.push_back(string(dirp->d_name));
    }
    closedir(dp);
}

int SelectMenu()
{
    item_index = 1;
    list_index = 0;
    last_index = 3;
    lcd.setCursor(0,item_index);
    lcd.print("->");
    while(1) {
        item_index+=direction;
        if(item_index>3) {
            item_index=3;
            list_index++;
            for(int i=1; i<4; i++) {
                lcd.setCursor(3,i);
                lcd.print("               ");
            }
        }
        if(item_index>strDisplay.size()) {
            item_index=strDisplay.size();
        }
        if(item_index<1) {
            item_index=1;
            list_index--;
            for(int i=1; i<4; i++) {
                lcd.setCursor(3,i);
                lcd.print("               ");
            }
        }
        if(list_index<0)list_index=0;
        if(list_index>strDisplay.size()-3)list_index=strDisplay.size()-3;

        lcd.setCursor(0,last_index);
        lcd.print("  ");
        lcd.setCursor(0,item_index);
        lcd.print("->");
        last_index = item_index;

        if(strDisplay.size()<=3) {
            for(int i=0; i<strDisplay.size(); i++) {
                lcd.setCursor(3,i+1);
                lcd.print(strDisplay[i].c_str());
            }
        }   else {
            lcd.setCursor(3,1);
            lcd.print(strDisplay[list_index].c_str());
            lcd.setCursor(3,2);
            lcd.print(strDisplay[list_index+1].c_str());
            lcd.setCursor(3,3);
            lcd.print(strDisplay[list_index+2].c_str());
        }

        if(!btn.read()) {
            //pc.printf("%d\n",item_index+list_index);
            while(!btn.read());
            return item_index+list_index;
        }
        wait(0.1);
    }
}

string tempz;
void displayData()
{
    
    if(run==1) {
        
        tempz.clear();
        lcd.clear();
        
        lcd.setCursor(0,0);
        sprintf(&tempz[0],"PROCESS     :% 3.1f",percent);
        lcd.print(tempz);
        lcd.print("%");
        
        tempz.clear();
        lcd.setCursor(0,1);
        sprintf(&tempz[0],"TEMPERATURE : %.1f C",float(temperature)/10.0f);
        lcd.print(tempz);
        
        lcd.setCursor(0,3);
        lcd.print("FILE: ");
        if(filePrint.size()>0)
            lcd.print(&filePrint[0]);
        else
            lcd.print("              ");
    }
    if(run==2) {


        lcd.clear();

        lcd.setCursor(0,0);
        lcd.print("  WAITING TO FILL   ");

        tempz.clear();
        lcd.setCursor(0,1);
        sprintf(&tempz[0],"TEMPERATURE : %.1f C",float(temperature)/10.0f);
        lcd.print(tempz);

        lcd.setCursor(0,3);
        lcd.print(" AUTO PANCAKE MAKER ");
    }
}



void getDir()
{
    direction= encoder.getDirection();
}

void getTemperature()
{
    int start,end;
    timer.start();

    TempSensor.output();
    TempSensor.write(0);
    wait_ms(10);
    TempSensor.write(1);
    wait_us(40);
    TempSensor.input();

    while (!TempSensor.read());
    start = timer.read_us();
    while (TempSensor.read());
    end = timer.read_us();

    if (end - start> 60) {
        for (int i = 0; i < 32; i++) {
            while (!TempSensor.read());
            start = timer.read_us();
            while (TempSensor.read());
            end = timer.read_us();

            if (end - start > 50) {
                temperature <<= 1;
                temperature |= 1;
            } else {
                temperature <<= 1;
            }
        }
        temperature &= 0xFFFF;
    }
    timer.reset();
    //pc.printf("Temperature : %d\n",temperature);
}
