#include <Time.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

// all of the button pins
int buzzer = 4;
int buttonPin = 2;
int echoPin = 8;
int trigPin = 9;
int ledPin = 3;

// declares other variables
int buttonState = 0;
float duration_calibrated = 0;
float distance_calibrated = 0;
float duration_new, distance_new;
float alarmTimeStandIn, original_time, new_time;
int buzzerState = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  setTime(8,59,50,1,1,20);
  Alarm.alarmRepeat(9,0,0,AlarmGoesOff);
}

void loop() {
  //Calibration
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
    digitalWrite(ledPin, HIGH);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration_calibrated = pulseIn(echoPin, HIGH);
    distance_calibrated = 0.017 * duration_calibrated;
    distance_calibrated = round(distance_calibrated);
    Serial.print("calibrated distance: ");
    Serial.print(distance_calibrated);
    Serial.println(" cm");
  } else if (buttonState == LOW){
    digitalWrite(ledPin, LOW);
  }
  Serial.println(hour());
  Serial.println(minute());
  Serial.println(second());
  delay(60);
  Alarm.delay(10);
}

void AlarmGoesOff()
{
  digitalWrite(buzzer, HIGH);
  buzzerState = HIGH;
  while (true){
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration_new = pulseIn(echoPin, HIGH);
    distance_new = 0.017*duration_new;
    if (distance_calibrated == 0){
        distance_calibrated = distance_new;
    }
    if ((distance_calibrated - round(distance_new)) > 5) {
        digitalWrite(buzzer, LOW);
        buzzerState = LOW;
        Serial.println("checking if in bed now");
        checkInBed();
        break;
      delay(10);
    }
  }
}

void checkInBed()
{
  original_time = now();
  while (true){
    if ((new_time - original_time) >= 0.25*60){
        break;
    }
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration_new = pulseIn(echoPin, HIGH);
    distance_new = 0.017*duration_new;
    if ((distance_calibrated - round(distance_new)) > 5) {
      if (buzzerState == LOW){
        digitalWrite(buzzer, HIGH);
        buzzerState = HIGH;
      } else {
        digitalWrite(buzzer, LOW);
        buzzerState = LOW;
      }
      new_time = now();
    
      delay(200);
    }
    Alarm.delay(100);
  }
  if (buzzerState == HIGH){
    digitalWrite(buzzer, LOW);
  } 
  
}

