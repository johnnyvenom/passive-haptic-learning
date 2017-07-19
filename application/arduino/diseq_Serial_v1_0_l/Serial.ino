
/*
serial messages to enable serial print:
'p' enables SPI
'a' enables accel
'm' enables PWM
's' enables SR
'o' disables all 
*/

//void check_serial()
//{
//  if (Serial.available() > 0)
//  {
//    int in_byte = Serial.read();
//    if (in_byte == 'p') enable_SPIprint = 1;
//    else if (in_byte == 'a') enable_accelprint = 1;
//    else if (in_byte == 'm') enable_PWMprint = 1;
//    else if (in_byte == 's') enable_SRprint = 1;
//    else if (in_byte == 'i') enable_printrawSPI = 1;
//    
//    else if (in_byte == 49) trigger_1motor(0);
//    else if (in_byte == 50) trigger_1motor(1);
//    else if (in_byte == 51) trigger_1motor(2);
//    else if (in_byte == 52) trigger_1motor(3);
//    else if (in_byte == 53) trigger_1motor(4);
//    else if (in_byte == 54) trigger_1motor(5);
//   
//    else if (in_byte == '7') trigger_sequence(0);
//    else if (in_byte == '8') trigger_sequence(1);
//    else if (in_byte == '9') trigger_sequence(2);
//    
//    else if (in_byte == 'o') 
//    {
//      enable_SPIprint = 0;
//      enable_PWMprint = 0;
//      enable_accelprint = 0;
//      enable_SRprint = 0;
//      enable_printrawSPI = 0;
//    }
//  } 
//}


void serialEvent()
{
  // if there's any serial available, read it:
 while(Serial.available()>3)
 {

    adj_index = Serial.parseInt();
    act_index = adj_index - 10;
    Serial.print("Index = ");
    Serial.println(act_index);
    // look for the next valid integer in the incoming serial stream:
    attacks[act_index] = Serial.parseInt();
    Serial.print("Attack = ");
    Serial.println(attacks[act_index]);
    s_t[act_index] = Serial.parseInt();
    Serial.print("Sustain = ");
    Serial.println(s_t[act_index]);
    s_i[act_index] = Serial.parseInt();
    Serial.print("Amplitude = ");
    Serial.println(s_i[act_index]);
    d_t[act_index] = Serial.parseInt();
    Serial.print("Decay = ");
    Serial.println(d_t[act_index]);
    onset[act_index] = millis();
    //act_flag[act_index] = 1;
    motor_shiftOut (act_index, 1);
 }
}


void trigger_1motor(int pins_numb2) {
     
      if (enable_SPIprint) Serial.println(pins_numb2); //print
      
      //ENVELOPE PARAMETERS
      attacks[pins_numb2] = 1000;
      s_t[pins_numb2] = 5000;
      s_i[pins_numb2] = 255;
      d_t[pins_numb2] = 0;

      onset[pins_numb2] = millis(); //ENVELOPE START TIME
      //actuatorStatus(pins_numb, 1); //FLAG - happens in motorshiftout now
      motor_shiftOut (pins_numb2, 1);  
}

void trigger_sequence(int seq_type){
  
  int temp_attack, temp_sustain, temp_amplitude, temp_decay;
  long sequence_timer = 0;
  int sequence_interval = 0;
  
  switch (seq_type) {
    case 0:
      temp_attack = 0;
      temp_sustain = 250;
      temp_amplitude = 255;
      temp_decay = 0;
      sequence_interval = 250;
      break;
    
    case 1:
      temp_attack = 0;
      temp_sustain = 0;
      temp_amplitude = 150;
      temp_decay = 1000;
      sequence_interval = 1000;
      break;
    
    case 2:
      temp_attack = 500;
      temp_sustain = 0;
      temp_amplitude = 255;
      temp_decay = 500;
      sequence_interval = 1000;
      break;
      
  }

  for (int ss=0;ss<6;ss++) {
      attacks[ss] = temp_attack;
      s_t[ss] = temp_sustain;
      s_i[ss] = temp_amplitude;
      d_t[ss] = temp_decay;
      
      onset[ss] = millis(); //ENVELOPE START TIME
      //actuatorStatus(pins_numb, 1); //FLAG - happens in motorshiftout now
      motor_shiftOut (ss, 1);  
  }
}
