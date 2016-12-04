/*****
        Algorithm based on MPU-9250_Snowda program. It has been modified by Josué Olmeda Castelló for imasD Tecnología.

        This algorithm calibrates and reads data from accelerometer, gyroscope, magnetometer and the
        internal temperature sensor. The lecture is made each time has a new mesured value (both gyro and accel data).
        A comunication with a computer is made using serial interface. The user can see the data measured with 1 second update rate.

        This algorithm uses the STM32L152 development board and the MPU-9250 9-axis InvenSense movement sensor. The communication
        between both devices is made through I2C serial interface.

        AD0 should be connected to GND.

                                                                04/05/2015
*****/

#include "mbed.h"
#include "MPU9250.h"
#include "Gamecontrol.h"
#include "my8x8RGB.h"
void timer();


Serial pc(USBTX, USBRX); // Huyperterminal default config: 9600 bauds, 8-bit data, 1 stop bit, no parity
Serial bell(PA_11,PA_12);
MPU9250 mpu9250;
Timer t;
//DigitalOut myled(LED1);

float sum = 0;
uint32_t sumCount = 0;
char buffer[14];
uint8_t dato_leido[2];
uint8_t whoami;
char showtimez[10];
int timez=0;
long int timetoplay=0;
long int lasttimetoplay=-1;
int main()
{
 
    setup_game();
    //___ Set up I2C: use fast (400 kHz) I2C ___
    i2c.frequency(400000);

    pc.printf("CPU SystemCoreClock is %d Hz\r\n", SystemCoreClock);

    t.start(); // Timer ON

    // Read the WHO_AM_I register, this is a good test of communication
    whoami = mpu9250.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

    pc.printf("I AM 0x%x\n\r", whoami);
    pc.printf("I SHOULD BE 0x71\n\r");
    if (I2Cstate != 0) // error on I2C
        pc.printf("I2C failure while reading WHO_AM_I register");

    if (whoami == 0x71) { // WHO_AM_I should always be 0x71
        pc.printf("MPU9250 WHO_AM_I is 0x%x\n\r", whoami);
        pc.printf("MPU9250 is online...\n\r");
        sprintf(buffer, "0x%x", whoami);
        wait(1);

        mpu9250.resetMPU9250(); // Reset registers to default in preparation for device calibration

        mpu9250.MPU9250SelfTest(SelfTest); // Start by performing self test and reporting values (accelerometer and gyroscope self test)
        pc.printf("x-axis self test: acceleration trim within : %f % of factory value\n\r", SelfTest[0]);
        pc.printf("y-axis self test: acceleration trim within : %f % of factory value\n\r", SelfTest[1]);
        pc.printf("z-axis self test: acceleration trim within : %f % of factory value\n\r", SelfTest[2]);
        pc.printf("x-axis self test: gyration trim within : %f % of factory value\n\r", SelfTest[3]);
        pc.printf("y-axis self test: gyration trim within : %f % of factory value\n\r", SelfTest[4]);
        pc.printf("z-axis self test: gyration trim within : %f % of factory value\n\r", SelfTest[5]);

        mpu9250.calibrateMPU9250(gyroBias, accelBias); // Calibrate gyro and accelerometer, load biases in bias registers
        pc.printf("x gyro bias = %f\n\r", gyroBias[0]);
        pc.printf("y gyro bias = %f\n\r", gyroBias[1]);
        pc.printf("z gyro bias = %f\n\r", gyroBias[2]);
        pc.printf("x accel bias = %f\n\r", accelBias[0]);
        pc.printf("y accel bias = %f\n\r", accelBias[1]);
        pc.printf("z accel bias = %f\n\r", accelBias[2]);
        wait(2);

        // Initialize device for active mode read of acclerometer, gyroscope, and temperature
        mpu9250.initMPU9250();
        pc.printf("MPU9250 initialized for active data mode....\n\r");

        // Initialize device for active mode read of magnetometer, 16 bit resolution, 100Hz.
        mpu9250.initAK8963(magCalibration);
        pc.printf("AK8963 initialized for active data mode....\n\r");
        pc.printf("Accelerometer full-scale range = %f  g\n\r", 2.0f*(float)(1<<Ascale));
        pc.printf("Gyroscope full-scale range = %f  deg/s\n\r", 250.0f*(float)(1<<Gscale));
        if(Mscale == 0) pc.printf("Magnetometer resolution = 14  bits\n\r");
        if(Mscale == 1) pc.printf("Magnetometer resolution = 16  bits\n\r");
        if(Mmode == 2) pc.printf("Magnetometer ODR = 8 Hz\n\r");
        if(Mmode == 6) pc.printf("Magnetometer ODR = 100 Hz\n\r");
        wait(1);
    }

    else { // Connection failure
        pc.printf("Could not connect to MPU9250: \n\r");
        pc.printf("%#x \n",  whoami);
        sprintf(buffer, "WHO_AM_I 0x%x", whoami);
        while(1) ; // Loop forever if communication doesn't happen
    }

    mpu9250.getAres(); // Get accelerometer sensitivity
    mpu9250.getGres(); // Get gyro sensitivity
    mpu9250.getMres(); // Get magnetometer sensitivity
    pc.printf("Accelerometer sensitivity is %f LSB/g \n\r", 1.0f/aRes);
    pc.printf("Gyroscope sensitivity is %f LSB/deg/s \n\r", 1.0f/gRes);
    pc.printf("Magnetometer sensitivity is %f LSB/G \n\r", 1.0f/mRes);
    magbias[0] = +470.;  // User environmental x-axis correction in milliGauss, should be automatically calculated
    magbias[1] = +120.;  // User environmental x-axis correction in milliGauss
    magbias[2] = +125.;  // User environmental x-axis correction in milliGauss

    while(1) {

        // If intPin goes high, all data registers have new data
        if(mpu9250.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {  // On interrupt, check if data ready interrupt

            mpu9250.readAccelData(accelCount);  // Read the x/y/z adc values
            // Now we'll calculate the accleration value into actual g's
            if (I2Cstate != 0) //error on I2C
                pc.printf("I2C error ocurred while reading accelerometer data. I2Cstate = %d \n\r", I2Cstate);
            else { // I2C read or write ok
                I2Cstate = 1;
                ax = (float)accelCount[0]*aRes - accelBias[0];  // get actual g value, this depends on scale being set
                ay = (float)accelCount[1]*aRes - accelBias[1];
                az = (float)accelCount[2]*aRes - accelBias[2];
            }

            mpu9250.readGyroData(gyroCount);  // Read the x/y/z adc values
            // Calculate the gyro value into actual degrees per second
            if (I2Cstate != 0) //error on I2C
                pc.printf("I2C error ocurred while reading gyrometer data. I2Cstate = %d \n\r", I2Cstate);
            else { // I2C read or write ok
                I2Cstate = 1;
                gx = (float)gyroCount[0]*gRes - gyroBias[0];  // get actual gyro value, this depends on scale being set
                gy = (float)gyroCount[1]*gRes - gyroBias[1];
                gz = (float)gyroCount[2]*gRes - gyroBias[2];
            }

            mpu9250.readMagData(magCount);  // Read the x/y/z adc values
            // Calculate the magnetometer values in milliGauss
            // Include factory calibration per data sheet and user environmental corrections
            if (I2Cstate != 0) //error on I2C
                pc.printf("I2C error ocurred while reading magnetometer data. I2Cstate = %d \n\r", I2Cstate);
            else { // I2C read or write ok
                I2Cstate = 1;
                mx = (float)magCount[0]*mRes*magCalibration[0] - magbias[0];  // get actual magnetometer value, this depends on scale being set
                my = (float)magCount[1]*mRes*magCalibration[1] - magbias[1];
                mz = (float)magCount[2]*mRes*magCalibration[2] - magbias[2];
            }

            mpu9250.getCompassOrientation(orientation);
        }

        Now = t.read_us();
        deltat = (float)((Now - lastUpdate)/1000000.0f) ; // set integration time by time elapsed since last filter update
        lastUpdate = Now;
        sum += deltat;
        sumCount++;

        // Pass gyro rate as rad/s
        // mpu9250.MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, mz);
        mpu9250.MahonyQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f, my, mx, mz);


        // Serial print and/or display at 1.5 s rate independent of data rates
        delt_t = t.read_ms();
        /////////////////   if (delt_t > 10) { // update LCD once per half-second independent of read rate

        //        pc.printf("ax = %f", 1000*ax);
        //        pc.printf(" ay = %f", 1000*ay);
        //        pc.printf(" az = %f  mg\n\r", 1000*az);
        //        pc.printf("gx = %f", gx);
        //        pc.printf(" gy = %f", gy);
        //        pc.printf(" gz = %f  deg/s\n\r", gz);
        //        pc.printf("mx = %f", mx);
        //        pc.printf(" my = %f", my);
        //        pc.printf(" mz = %f  mG\n\r", mz);


        tempCount = mpu9250.readTempData();  // Read the adc values
        if (I2Cstate != 0) //error on I2C
            pc.printf("I2C error ocurred while reading sensor temp. I2Cstate = %d \n\r", I2Cstate);
        else { // I2C read or write ok
            I2Cstate = 1;
            temperature = ((float) tempCount) / 333.87f + 21.0f; // Temperature in degrees Centigrade
            //            pc.printf(" temperature = %f  C\n\r", temperature);
        }
        //        pc.printf("q0 = %f\n\r", q[0]);
        //        pc.printf("q1 = %f\n\r", q[1]);
        //        pc.printf("q2 = %f\n\r", q[2]);
        //        pc.printf("q3 = %f\n\r", q[3]);

        //        pc.printf("Compass orientation: %f\n", orientation[0]);


        yaw   = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
        pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
        roll  = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
        pitch *= 180.0f / PI;
        yaw   *= 180.0f / PI;
        yaw   -= 13.8f; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
        roll  *= 180.0f / PI;


        //         pc.printf("Yaw, Pitch, Roll: %f %f %f\n\r", yaw, pitch, roll);
        //          pc.printf("Pitch, Roll: %f %f\n\r", pitch, roll);
        
        
        
        
        if((pitch>40&&pitch<80)||(pitch>-80&&pitch<-40)||(roll>40&&roll<80)||(roll>-80&&roll<-40))
        {
            if (t.read_ms()%100==0) {
            if(pitch>40&&pitch<80) {
              //  pc.printf("VV\n");
                moving('s');
            } 
             else if(pitch>-80&&pitch<-40) {
             //   pc.printf("^^\n");
                moving('w');
            } 
            else if(roll>40&&roll<80) {
             //   pc.printf("<<\n");
                moving('a');
            } 
            else if(roll>-80&&roll<-40) {
         //       pc.printf(">>\n");
                moving('d');
            }
            }
        
        }
        else if((pitch>10&&pitch<40)||(pitch>-40&&pitch<-10)||(roll>10&&roll<40)||(roll>-40&&roll<-10))
        {
            if (t.read_ms()%200==0) {
            if(pitch>10&&pitch<40) {
           //     pc.printf("V\n");
                moving('s');

            } else if(pitch>-40&&pitch<-10) {
           //     pc.printf("^\n");
                moving('w');
            }

            else if(roll>10&&roll<40) {

            //    pc.printf("<\n");
                moving('a');


            }  else if(roll>-40&&roll<-10) {
            //    pc.printf(">\n");
                moving('d');
            }
            }
        }
        else
        {
                pc.printf("No move\n");
                Showscreen();
            }
            gamestart();/*
        if(timetoplay%1000==0)
        {
        bell.printf("%d\n",timetoplay/1000);
        }
        timetoplay++;*/
        if((delt_t/1000)>(lasttimetoplay/1000)){
         bell.printf("%d\n",(delt_t/1000)+ timeformitem() );
         lasttimetoplay=delt_t;
         }
        
    /*    if(delt_t%1000==0)
        
        {timetoplay++;
    
         if((lasttimetoplay/10)!=(timetoplay/10))
           { bell.printf("%d\n",timetoplay/10);
        
        lasttimetoplay=timetoplay;}
        }

        

        myled= !myled;
        count = t.read_ms();

        if(count > 1<<21) {
            t.start(); // start the timer over again if ~30 minutes has passed
            count = 0;
            deltat= 0;
            lastUpdate = t.read_us();
        }*/
        sum = 0;
        sumCount = 0;
        //        }

    }
}
