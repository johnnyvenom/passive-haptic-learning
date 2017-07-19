/******************************************
// Convert ints in gyro_val, accel_val, and mag_val into bytes
// and stores them in order into all_imu_data array.
// Params: none
// Returns: none

*******************************************/

void convertImuData() {
    all_imu_data[0] = topByte(gyro_val[0]);
    all_imu_data[1] = bottomByte(gyro_val[0]);
    all_imu_data[2] = topByte(gyro_val[1]);
    all_imu_data[3] = bottomByte(gyro_val[1]);
    all_imu_data[4] = topByte(gyro_val[2]);
    all_imu_data[5] = bottomByte(gyro_val[2]);
    all_imu_data[6] = topByte(accel_val[0]);
    all_imu_data[7] = bottomByte(accel_val[0]);
    all_imu_data[8] = topByte(accel_val[1]);
    all_imu_data[9] = bottomByte(accel_val[1]);
    all_imu_data[10] = topByte(accel_val[2]);
    all_imu_data[11] = bottomByte(accel_val[2]);
    all_imu_data[12] = topByte(mag_val[0]);
    all_imu_data[13] = bottomByte(mag_val[0]);
    all_imu_data[14] = topByte(mag_val[1]);
    all_imu_data[15] = bottomByte(mag_val[1]);
    all_imu_data[16] = topByte(mag_val[2]);
    all_imu_data[17] = bottomByte(mag_val[2]);
}
     
 /******************************************
/read sensors
/
/
/
*******************************************/     
      
//void readSensors()
//{
//  if (micros() > sensorTimer + sensorInterval)
//  {
////    dof.readGyro();
////    gyro_val[0] = (gyro_val[0]*IMU_scalar)+(dof.gx*(1-IMU_scalar));
////    gyro_val[1] = (gyro_val[1]*IMU_scalar)+(dof.gy*(1-IMU_scalar));
////    gyro_val[2] = (gyro_val[2]*IMU_scalar)+(dof.gz*(1-IMU_scalar));
//    
//    dof.readAccel();
//    accel_val[0] = (accel_val[0]*IMU_scalar)+(dof.ax*(1-IMU_scalar));
//    accel_val[1] = (accel_val[1]*IMU_scalar)+(dof.ay*(1-IMU_scalar));
//    accel_val[2] = (accel_val[2]*IMU_scalar)+(dof.az*(1-IMU_scalar));
//    
//    all_imu_data[6] = topByte(accel_val[0]);
//    all_imu_data[7] = bottomByte(accel_val[0]);
//    all_imu_data[8] = topByte(accel_val[1]);
//    all_imu_data[9] = bottomByte(accel_val[1]);
//    all_imu_data[10] = topByte(accel_val[2]);
//    all_imu_data[11] = bottomByte(accel_val[2]);
//    
////    dof.readMag();
////    mag_val[0] = (mag_val[0]*IMU_scalar)+(dof.mx*(1-IMU_scalar));
////    mag_val[1] = (mag_val[1]*IMU_scalar)+(dof.my*(1-IMU_scalar));
////    mag_val[2] = (mag_val[2]*IMU_scalar)+(dof.mz*(1-IMU_scalar));
//    
//    sensorTimer=micros();
//    if (sensorTimer > micros()+(sensorInterval)) sensorTimer=micros(); //if micros overflows set sensorTimer back to micros();
//  }
//}

void printSensors()
{
  if (millis()>printTimer+printInterval)
  {
    Serial.print("Magnetometer: ");
    for (int ff=0; ff<3; ff++) 
    {
      Serial.print (mag_val[ff]);
      Serial.print (", ");
    }
    Serial.print(":   ");
    
        Serial.print("Accelerometer: ");
    for ( int ff=0; ff<3; ff++) 
    {
      Serial.print (accel_val[ff]);
      Serial.print (", ");
    }
    Serial.print(":   ");
    
        Serial.print("Gyroscope: ");
    for ( int ff=0; ff<3; ff++) 
    {
      Serial.print (gyro_val[ff]);
      Serial.print (", ");
    }
    Serial.println(" ");
    
    printTimer=millis();
  }
}
