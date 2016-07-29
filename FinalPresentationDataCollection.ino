//This code records the percent moisture from 11 moisture sensors and
//saves them to an SD card, along with the date and time of the reading.


#include <DS3231.h>
#include <avr/sleep.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

DS3231 rtc(SDA,SCL);


//The data log file
#define FILE_NAME "DataLog.txt"
 
//Data header  (these lines get written to the beginning"
#define LOGGERNAME "Presentation Data"
#define DATA_HEADER "SampleTime, PercentMoisture"

long UnixTime;
int intTime;
int lastData = 1;
int mostRecentTime;
int wakePin = 10;  //rain gauge pin
int sleepStatus = 0; //variable to store request for sleep
int count = 0; //counter
int analogNum = 0;
int change;
float sensorVal;
float raingauge;
float voltage;
float vwc;

void wakeUpNow()  //interrupt is handled here after waking up
{
  //execute code here before returning to loop function
  //timers and codes using timer don't work here
  //basically just used to wake up microcontroller
}

void setupLogFile()
{
  //Initialise the SD card
  if (!SD.begin(12))
  {
    Serial1.println("Error: SD card failed to initialise or is missing.");
    //Hang
    //while (true); 
  }
  
  //Check if the file already exists
  bool oldFile = SD.exists(FILE_NAME);  
  
  //Open the file in write mode
  File logFile = SD.open(FILE_NAME, FILE_WRITE);
  
  //Add header information if the file did not already exist
  if (!oldFile)
  {
    logFile.println(LOGGERNAME);
    logFile.println(DATA_HEADER);
  }
  
  //Close the file to save it
  logFile.close();  
}

void logData(String rec)
{
  //Re-open the file
  File logFile = SD.open(FILE_NAME, FILE_WRITE);
  
  //Write the CSV data
  logFile.println(rec);
  
  //Close the file to save it
  logFile.close();  
}
 


void setup() {
  Serial.begin(9600);
  pinMode(wakePin, INPUT_PULLUP);
  attachInterrupt(2, wakeUpNow, CHANGE); //interrupt 0 is attached to digital pin 2
  setupLogFile();
  rtc.begin();
  sleepNow();
  Wire.begin();

  rtc.setDate(29, 07, 2016); //set date and time
  rtc.setTime(11, 43, 00);

  raingauge = 0.01; //when system wakes up, 0.01" of rain have fallen
}

void sleepNow() //put microcontroller to sleep here
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable();

  attachInterrupt(2, wakeUpNow, CHANGE); //reattach interrupt here
  sleep_mode(); //where device is put to sleep

  Serial.println("I am awake bitch");
  delay(100);
  sleep_disable(); //first thing done after waking up
  detachInterrupt(2); //disable interrupt so that wakeUpNow
                      //will not be executed during normal running

}

String createDataRecord()
{
  //Create a String type data record in csv format
  //SampleTime, Moisture
  String data = "Time: ";
  data += rtc.getUnixTime(rtc.getTime());
  data += ", Moistures: ";
  
  for (analogNum = 0; analogNum <= 7; analogNum++){
   if (analogNum == 6){
    analogNum = 7;
   }
  sensorVal = analogRead(analogNum); //read soil moisture sensor
  voltage =(sensorVal/1023)*3; //conversion to voltage from analog
  
  
  // following if statement is a calibration for percent moisture from manufacturer
  if(voltage <= 1.1)
  {
    vwc = 10*voltage;
    }
  else if(voltage <= 1.3)
  {
    vwc = 25*voltage-17.5;
    }
  else if(voltage <= 1.82)
  {
    vwc = 48.08*voltage-47.5;
    }
  else if(voltage <= 2.2)
  {
    vwc = 26.32*voltage-7.89;
    }
 else vwc = 0;
  data += vwc;     //adds the battery voltage to the data string
  data += ", ";
  }
  data += "Rain: ";
  data += raingauge;
  return data;
}

void loop() {
  change = digitalRead(wakePin); //read rain gauge pin
  if (change == 0 && lastData != 0){
    raingauge = raingauge + 0.01;  //if bucket has tipped, increase rain amount
    Serial.println(raingauge);
  }

  lastData = change; //helping debounce the tipping bucket

  UnixTime = rtc.getUnixTime(rtc.getTime());  //read Unix time
  intTime = (int)UnixTime;
  if (intTime%5 == 0 && (mostRecentTime != intTime))  //do this every 5 seconds for 30 minutes
  {
     String dataRec = createDataRecord();  //collect data into a string
  
    //Save the data record to the log file
    logData(dataRec); 
    
    mostRecentTime = intTime;  //change Unix time
    
    count++; //count up to 30 minutes before going to sleep
  }


  if (count >= 5000) {
      Serial.println("Timer: Entering Sleep mode"); //goes to sleep after 30 minutes
      delay(100);     // this delay is needed, the sleep
                      //function will provoke a Serial error otherwise!!
      count = 0;
      sleepNow();     // sleep function called here
  }
 }

