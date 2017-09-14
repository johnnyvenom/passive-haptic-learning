/******************************************
/process actuators
/
/
/
*******************************************/

void process_actuators()
{
  for(i=0;i<6;i++)
  {
    //if (motorState[i] > 0) cur_position[i] = millis() - onset[i];
      cur_position[i] = millis() - onset[i];

      if (motorState[i] > 0 ) 
        {
        if (cur_position[i] <  attacks[i]) {//attack
           cur_pwm[i] = (cur_position[i]/attacks[i]) * s_i[i];//calc attack pwm
          // Serial.print("attack"); 
        }
        
        else if (cur_position[i] < attacks[i] + s_t[i]) {//sustain
          cur_pwm[i] = s_i[i];
          //Serial.println(cur_pwm[i]);
        }
        
        else if (cur_position[i] < attacks[i] + s_t[i] + d_t[i]) { //decay phase
          cur_pwm[i] = s_i[i] - (((cur_position[i]-attacks[i]-s_t[i])/d_t[i]) * s_i[i]);
          //Serial.println(cur_pwm[i]);
        }
        
        else if (cur_position[i] < (attacks[i] + s_t[i] + d_t[i]+ brakeInterval)) { //braking
          cur_pwm[i] = 0;
          if (motorState[i] != 2) 
          {
          motor_shiftOut(i, 2);
//          Serial.print("motor ");
//          Serial.print(i);
//          Serial.println(" brake");
          }
        }
        
        if (cur_position[i] > ( attacks[i] +s_t[i] + d_t[i] + brakeInterval)) // not active
        {
         // Serial.print(i);
          if (motorState[i] != 0) 
          {
           cur_pwm[i] = 0;
           motor_shiftOut (i, 0);
//          Serial.print("motor ");
//          Serial.print(i);
//          Serial.println("stop");
          }
        }
    }
  }
}

/******************************************
///motor shiftOut
/
/
/
*******************************************/

void motor_shiftOut(int motor, int state)
{
  motorState[motor] = state;
  
//  Serial.print("motor ");
//  Serial.print(motor);
//  Serial.print(", ");
//  Serial.println(state);
  
  byte motorSRtemp = 1;
    
  switch(state) 
    {
        case 0: //turns motor off
          motorSRtemp = 1 << motorPinB[motor];
          motorSRtemp = ~motorSRtemp; //bitwise NOT
          motorSRstate = motorSRstate & motorSRtemp; //de-activate pinB
        break;
        
        case 1: //turns motor on
          motorSRtemp = 1 << motorPinB[motor];
          motorSRtemp = ~motorSRtemp;
          motorSRstate = motorSRstate & motorSRtemp; //de-activate pinB
          
        break;

      case 2: //turns motor braking on
        motorSRtemp = 1 << motorPinB[motor]; //sets bit 'motor' to 1
        motorSRstate = motorSRstate | motorSRtemp;// activate pinB
        break;

    }
    //check pinA and turn on if any motors are in states 1 and 2 (on and braking)
    pinAflag=0;
    for (ii=0;ii<6;ii++) 
    {
      if (motorState[ii]>0)pinAflag=1; //setpinAflag to 1 if any motors are in state 1 or 2
    }
    
    if (pinAflag>0) 
    {
      motorSRstate = motorSRstate | 1;
     // Serial.println("true");
    }
    
    else motorSRstate = 0;
   // Serial.print ("Pin A Status:");
    //Serial.println(pinAflag);
   if (motorSRstate != prev_motorSRstate)
   {
   //set shift register
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, motorSRstate);
    digitalWrite(latchPin, HIGH);
    if (enable_SRprint) Serial.println(motorSRstate, BIN);
    prev_motorSRstate = motorSRstate;
   }

}

/******************************************
/set actuators
/
/
/
*******************************************/

void set_actuators()
{
//  if (millis() > PWMwriteTimer + PWMwriteInterval)
//  {
    for (int aa = 0; aa < 6; aa++) 
    {
      if(cur_pwm[aa] != prev_pwm[aa]) {
        Palatis::SoftPWM.set(pins[aa],cur_pwm[aa]);
        prev_pwm[aa]=cur_pwm[aa];
        
     if (enable_PWMprint){ 
      Serial.print(aa);
      Serial.print(": ");
      Serial.println(cur_pwm[aa]);
       }
      }
    }
//    PWMwriteTimer=millis();
//    
//  }
}
