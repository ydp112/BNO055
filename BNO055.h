#include "mbed.h"

#define bno055_addr (0x28 << 1)

#define BNO055_ID_ADDR                                           0x00
#define BNO055_EULER_H_LSB_ADDR                                  0x1A
#define BNO055_TEMP_ADDR                                         0x34
#define BNO055_OPR_MODE_ADDR                                     0x3D
#define BNO055_CALIB_STAT_ADDR                                   0x35
#define BNO055_SYS_STAT_ADDR                                     0x39
#define BNO055_SYS_ERR_ADDR                                      0x3A
#define BNO055_AXIS_MAP_CONFIG_ADDR                              0x41
#define BNO055_SYS_TRIGGER_ADDR                                  0x3F

typedef struct CalibStatus_t
{
    int mag;
    int acc;
    int gyr;
    int sys;
} CalibStatus;

typedef struct Euler_t
{
    float heading;
    float pitch;
    float roll;
} Euler;


/** Class for operating Bosch BNO055 sensor over I2C **/
class BNO055 
{ 
public: 

/** Create BNO055 instance **/
    BNO055(PinName SDA, PinName SCL); 
    
void writeReg(int regAddr, char value);

char readReg(int regAddr);

CalibStatus readCalibrationStatus();

bool bno055Healthy();


Euler getEulerAngles();

    private:
    
        I2C i2c;
        char rx,tx[2],address;  //I2C variables
        char rawdata[22]; //Temporary array for input data values
        char op_mode;
        char pwr_mode;
        float accel_scale,rate_scale,angle_scale;
        int temp_scale;
        
void readchar(char location){
    tx[0] = location;
    i2c.write(address,tx,1,true);
    i2c.read(address,&rx,1,false);
}

void writechar(char location, char value){
    tx[0] = location;
    tx[1] = value;
    i2c.write(address,tx,2);
}

void setpage(char value){
    writechar(0x07,value);
}
    };