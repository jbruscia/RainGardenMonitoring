#include <avr/sleep.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Sodaq_DS3231.h"


//The data log file
#define FILE_NAME "DataLog.txt"
 
//Data header  (these lines get written to the beginning"
#define LOGGERNAME "Mayfly microSD Card Tester"
#define DATA_HEADER "SampleNumber, vwc"

int samplenum = 1;
int wakePin = 10;  //rain gauge pin
int sleepStatus = 0; //variable to store request for sleep
int count = 0; //counter
float sensorVal1;
float voltage1;
float vwc;

void wakeUpNow()  //interrupt is handled here after waking up
{
  //execute code here before returning to loop function
  //timers and codes using timer don't work here
  //basically just used to wake up microcontroller
}

void setup() {
  Serial1.begin(9600);
  pinMode(wakePin, INPUT);
  attachInterrupt(2, wakeUpNow, CHANGE); //interrupt 0 is attached to digital pin 2
  setupLogFile();
  rtc.begin();
}

void sleepNow() //put microcontroller to sleep here
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable();

  attachInterrupt(2, wakeUpNow, CHANGE); //reattach interrupt here
  sleep_mode(); //where device is put to sleep

  sleep_disable(); //first thing done after waking up
  detachInterrupt(2); //disable interrupt so that wakeUpNow
                                              //will not be executed during normal running
}

void loop() {
  //count to 10 seconds before going to sleep
  Serial1.print("Awake for ");
  Serial1.print(count);
  Serial1.println("sec");
  count++; 

  
  sensorVal1 = analogRead(A0); //read soil moisture sensor
  voltage1 =(sensorVal1/1023)*3; //conversion to voltage from analog

  // following if statement is a calibration for percent moisture from manufacturer
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
  
  //Serial1.print("sensor1: ");
  //Serial1.print(sensorVal1);
  //Serial1.print(" voltage: ");
  //Serial1.print(voltage1);
  Serial1.print(" vwc: ");
  Serial1.println(vwc);
  delay(1000);   // waits for a second
  
  String dataRec = createDataRecord();
  
  //Save the data record to the log file
  logData(dataRec);

  if (count >= 10) {
      Serial1.println("Timer: Entering Sleep mode");
      delay(100);     // this delay is needed, the sleep
                      //function will provoke a Serial error otherwise!!
      count = 0;
      sleepNow();     // sleep function called here
  }
}

void setupLogFile()
{
  //Initialise the SD card
  if (!SD.begin(12))
  {
    Serial1.println("Error: SD card failed to initialise or is missing.");
    //Hang
  //  while (true); 
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
 
String createDataRecord()
{
  //Create a String type data record in csv format
  //SampleNumber, Battery
  String data = "";
  data += samplenum;           //creates a string called "data", put in the sample number 
  data += ",";                 //adds a comma between values
  data += vwc;     //adds the battery voltage to the data string
  samplenum++;   //increment the sample number
  return data;
}
