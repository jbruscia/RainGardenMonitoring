#define rain_gauge_pin 2  //pin where magnetic reed switch is

void setup() {
  Serial.begin(9600);
  pinMode (rain_gauge_pin, INPUT);
  digitalWrite(rain_gauge_pin, HIGH);  //set up input in as low
  attachInterrupt(digitialPintoInterrupt(rain_gauge_pin), rainTrigger, FALLING);  //trigger interrupt when reed switch goes from high to low
  interrupts();
}


volatile unsigned long rain_last=0;


void rainTrigger()    //ISR that triggers while loop in main code function
{
  long thisTime=micros()-rain_last;
  rain_last=micros();
  if(thisTime>500)     //debounce time is 500us, change number to change debounce
  {
    volatile int trigger1 = 1;
  }
}

void mainCode();
{
  while(trigger1 = 1)  //while interrupt is triggered
  {

    // read water level sensor and print to serial monitor
     double sensorValue = analogRead(A0);
    //double depth = map(sensorValue, 120, 630, 0, 21);  //map sensor into depth measurement
     double depth = ((sensorValue-120)/510)*21;
    depth = constrain(depth, 0, 21);
  
    Serial.print("sensor: ");
     Serial.print(sensorValue);
    Serial.print(" depth: ");
    Serial.println(depth);

    delay(50);


    // read moisture sensors and print to serial monitor
    double sensorValue1 = analogRead(A1);
    double sensorValue2 = analogRead(A2);
  
  
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

    trigger1 = 0; //reset trigger to exit while loop
                  //trigger1 will go to 1 again if interrupt is still going
  }


  
}

