
#include "mbed.h"
#include "BNO055.h"

BNO055::BNO055(PinName SDA, PinName SCL) : i2c(SDA, SCL){
    address = bno055_addr;
    }

/**
 * Function to write to a single 8-bit register
 */
void BNO055:: writeReg(int regAddr, char value)
{
    char wbuf[2];
    wbuf[0] = regAddr;
    wbuf[1] = value;
    i2c.write(bno055_addr, wbuf, 2, false);  
}

/**
 * Function to read from a single 8-bit register
 */
char BNO055:: readReg(int regAddr)
{
    char rwbuf = regAddr;
    i2c.write(bno055_addr, &rwbuf, 1, false);
    i2c.read(bno055_addr, &rwbuf, 1, false);
    return rwbuf;
}

/**
 * Returns the calibration status of each component
 */
CalibStatus BNO055:: readCalibrationStatus()
{
    CalibStatus status;
    int regVal = readReg(BNO055_CALIB_STAT_ADDR);
        
    status.mag = regVal & 0x03;
    status.acc = (regVal >> 2) & 0x03;
    status.gyr = (regVal >> 4) & 0x03;
    status.sys = (regVal >> 6) & 0x03;
    
    return status;
}


/**
 * Checks that there are no errors on the accelerometer
 */
bool BNO055:: bno055Healthy()
{
    int sys_error = readReg(BNO055_SYS_ERR_ADDR);
    wait(0.001);
    int sys_stat = readReg(BNO055_SYS_STAT_ADDR);
    wait(0.001);
    
    if(sys_error == 0 && sys_stat == 5)
        return true;
    else
        return false;
}

/**
 * Reads the Euler angles, zeroed out
 */
Euler BNO055::getEulerAngles()
{
    char buf[16];
    Euler e;
    
    // Read in the Euler angles
    buf[0] = BNO055_EULER_H_LSB_ADDR;
    i2c.write(bno055_addr, buf, 1, false);
    i2c.read(bno055_addr, buf, 6, false);
    
    short int euler_head = buf[0] + (buf[1] << 8);
    short int euler_roll = buf[2] + (buf[3] << 8);
    short int euler_pitch = buf[4] + (buf[5] << 8);
    
    e.heading = ((float)euler_head) / 16.0;
    e.roll = ((float)euler_roll) / 16.0;
    e.pitch = ((float)euler_pitch) / 16.0;
    
    return e;
}
