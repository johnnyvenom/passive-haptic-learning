unsigned long testTimer=0;
int testInterval = 500;


/******************************************
test motors
/
/
/
/
*******************************************/

void test_motors()
{
  if (millis() > testTimer+testInterval)
  {   
    pins_numb = (pins_numb + 1) % 6; //WHICH ACTUATOR
    
    attacks[pins_numb] = 2000; //ENVELOPE PARAMETERS
    s_t[pins_numb] = 0;
    s_i[pins_numb] = 255;
    d_t[pins_numb] = 0;
        
     onset[pins_numb] = millis(); //ENVELOPE START TIME
     //actuatorStatus(pins_numb, 1); //FLAG
     motor_shiftOut (pins_numb, 1);   
     
//        Serial.print("actuator #: ");
//        Serial.println(pins_numb);
//        Serial.print(": ");
//        Serial.print(a_t[pins_numb]);
//        Serial.print(", ");
//        Serial.print(s_t[pins_numb]);
//        Serial.print(", ");
//        Serial.print(s_i[pins_numb]);
//        Serial.print(", ");
//        Serial.print(d_t[pins_numb]);
//        Serial.println(". ");
        
    testTimer=millis();
    
    commLEDflag = 1;
  } 
}
