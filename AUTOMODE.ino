long autochangeInterval = 600000; //60000 = 1 minutes
long nextAutoChange = 600000; 

void CheckAutoMode()
{
  if(AUTOMODE)
  {
    
    if(millis() > nextAutoChange)
    {
      newProgram = true;
      nextAutoChange = millis()+autochangeInterval;
      }
    }
  }

  
void CheckForAutoModeOnStart()
{
  if(digitalRead(BUTTON_PIN) == LOW)
  {
    AUTOMODE = true;
    while (digitalRead(BUTTON_PIN) == LOW ) {;}//wait
    delay(100);
   }

}
  
