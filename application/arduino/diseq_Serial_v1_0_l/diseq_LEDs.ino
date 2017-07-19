void pwrLedctrl() //toggles pwrLED on and off - not used
{
  if (millis() > pwrTimer + (pwrInterval/2)) digitalWrite(pwrLED, HIGH);
  else if (millis() > pwrTimer + (pwrInterval)) {
    digitalWrite(pwrLED, LOW);
    pwrTimer = millis();
  }
}

void commLEDctrl() //turns commLED on when SPI communication occurs
{
  if (commLEDflag == 1) 
  {
    digitalWrite(commLED, HIGH);
    commTimer = millis();
    commLEDflag = 0;
  }
 else if (millis() > commTimer + (commInterval)) 
 {
   digitalWrite(commLED, LOW);
 }
}

//blinks pwr led for every error message
void errorLEDctrl()
{
  if (errorFlag == 1) {
    digitalWrite(pwrLED, LOW);
    errorTimer = millis();
    errorFlag = 2;
  }
  
  if (errorFlag==2) {
    if (millis() > (errorTimer + errorInterval)) {
      digitalWrite(pwrLED, HIGH);
      errorFlag=0;
    }
  }
}
