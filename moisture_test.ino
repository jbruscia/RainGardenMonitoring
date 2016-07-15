void setup() {
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  double sensorValue1 = analogRead(A0);
  delay(50);
  double sensorValue2 = analogRead(A1);
  
  
  double voltage1 = (sensorValue1/1023)*3;
  double vwc;
  if(voltage1 <= 1.1)
  {
    vwc = 10*voltage1-1;
    }
  else if(voltage1 <= 1.3)
  {
    vwc = 25*voltage1-17.5;
    }
  else if(voltage1 <= 1.82)
  {
    vwc = 48.08*voltage1-47.5;
    }
  else if(voltage1 <= 2.2)
  {
    vwc = 26.32*voltage1-7.89;
    }
 else vwc = 0;
  
  Serial.print("sensor1: ");
  Serial.print(sensorValue1);
  Serial.print(" voltage: ");
  Serial.print(voltage1);
  Serial.print(" vwc: ");
  Serial.println(vwc);

  double voltage2 = (sensorValue2/1023)*3;
  double vwc2;
  if(voltage2 <= 1.1)
  {
    vwc2 = 10*voltage2-1;
  }
  else if(voltage2 <= 1.3)
  {
    vwc2 = 25*voltage2-17.5;
    }
  else if(voltage2 <= 1.82)
  {
    vwc2 = 48.08*voltage2-47.5;
    }
  else if(voltage2 <= 2.2)
  {
    vwc2 = 26.32*voltage2-7.89;
    }
 else vwc2 = 0;
  
  Serial.print("sensor2: ");
  Serial.print(sensorValue2);
  Serial.print(" voltage: ");
  Serial.print(voltage2);
  Serial.print(" vwc: ");
  Serial.println(vwc2);
  Serial.println();

  

  delay(1500);
}
