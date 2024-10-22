int trigPin = 13;    // Trigger
int echoPin = 3;    // Echo
long duration;
float cm;

int led = 11;         // the PWM pin the LED is attached to
int brightness = 0;  // how bright the LED is



void setup() {
  //Serial Port begin
  Serial.begin (4800);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  Serial.print(duration);
  Serial.print("ms");
  Serial.println();
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  if (cm <= 12.0) {
   brightness = 0.0;
  digitalWrite(trigPin, HIGH);
  delay(1000);
   }
   else if (cm >= 42.0) {
     brightness = 255.0;
  digitalWrite(trigPin, HIGH);
  delay(1000);
    } 
    else {
     brightness = ((cm - 12.0) / (42.0 - 12.0)) * 255.0;
      digitalWrite(trigPin, LOW);

    }
  analogWrite(led, brightness);
  delay(100);
}
