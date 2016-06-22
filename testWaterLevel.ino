void setup() {
  Serial.begin(9600);
}

void loop() {
  double sensorValue = analogRead(A0); //read sensor
  //double depth = map(sensorValue, 120, 630, 0, 21);  //map sensor into depth measurement
  double depth = ((sensorValue-120)/510)*21;
  depth = constrain(depth, 0, 21);

  Serial.print("sensor: ");
  Serial.print(sensorValue);
  Serial.print(" depth: ");
  Serial.println(depth);

  delay(50);
}
