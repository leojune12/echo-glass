
const int TRIG_PIN_1 = 3;
const int ECHO_PIN_1 = 2;
const int TRIG_PIN_2 = 5;
const int ECHO_PIN_2 = 4;
const int TRIG_PIN_3 = 7;
const int ECHO_PIN_3 = 6;
const int BUZZER_PIN = 8;
const int RED_LED_PIN = 9;
const int GREEN_LED_PIN = 10;
const int YELLOW_LED_PIN = 11;


float temp_In_C = 20.0;     // Can enter actual air temp here for maximum accuracy
float speed_Of_Sound;       // Calculated speed of sound based on air temp
float distance_Per_uSec;    // Distance sound travels in one microsecond
float max_distance = 150;   // In centimeter

float distance_1 = 0;
float distance_2 = 0;
float distance_3 = 0;

void setup() {
  pinMode(TRIG_PIN_1,OUTPUT);
  pinMode(ECHO_PIN_1,INPUT);
  pinMode(TRIG_PIN_2,OUTPUT);
  pinMode(ECHO_PIN_2,INPUT);
  pinMode(TRIG_PIN_3,OUTPUT);
  pinMode(ECHO_PIN_3,INPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(RED_LED_PIN,OUTPUT);
  pinMode(GREEN_LED_PIN,OUTPUT);
  pinMode(YELLOW_LED_PIN,OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  
  // Formula to calculate speed of sound in meters/sec based on temp
  speed_Of_Sound = 331.1 +(0.606 * temp_In_C);  
  // Calculate the distance that sound travels in one microsecond in centimeters
  distance_Per_uSec = speed_Of_Sound / 10000.0;
  Serial.begin(9600);
}

void loop() {
  long int t1 = millis();
  
  main_code();

  long int t2 = millis();
  Serial.print("Loop duration: ");
  Serial.print(t2-t1);
  Serial.println(" milliseconds");
}

void main_code() {
  distance_1 = read_sensor("1", TRIG_PIN_1, ECHO_PIN_1);
  distance_2 = read_sensor("2", TRIG_PIN_2, ECHO_PIN_2);
  distance_3 = read_sensor("3", TRIG_PIN_3, ECHO_PIN_3);

  if (distance_1 <= max_distance) {
    digitalWrite(RED_LED_PIN, HIGH);
  } else {
    digitalWrite(RED_LED_PIN, LOW);
  }

  if (distance_2 <= max_distance) {
    digitalWrite(YELLOW_LED_PIN, HIGH);
  } else {
    digitalWrite(YELLOW_LED_PIN, LOW);
  }

  if (distance_3 <= max_distance) {
    digitalWrite(GREEN_LED_PIN, HIGH);
  } else {
    digitalWrite(GREEN_LED_PIN, LOW);
  }

  float distance = distance_1;

  if (distance_2 < distance) {
    distance = distance_2;
  }
  
  if (distance_3 < distance) {
    distance = distance_3;
  }

  sound_buzzer(distance);
}

float read_sensor(String sensor_number, int TRIG_PIN, int ECHO_PIN) {
  float duration, distanceCm, final_distance=0;

  for (int i=0; i<2; i++) {
    digitalWrite(TRIG_PIN, HIGH);       // Set trigger pin HIGH 
    delayMicroseconds(50);              // Hold pin HIGH for 20 uSec
    digitalWrite(TRIG_PIN, LOW);        // Return trigger pin back to LOW again.
    duration = pulseIn(ECHO_PIN, HIGH);  // Measure time in uSec for echo to come back.
   
    // convert the time data into a distance in centimeters, inches and feet
    duration = duration / 2.0;  // Divide echo time by 2 to get time in one direction
    distanceCm = duration * distance_Per_uSec;

    if (distanceCm > final_distance) {
      if (distanceCm > max_distance) {
        final_distance = max_distance;
      } else {
        final_distance = distanceCm;
      }
    }

    delay(10);
  }
   
  if (final_distance <= 0){
    Serial.println("Sensor " + sensor_number +  " out of range");
  } else {
    Serial.print("Sensor " + sensor_number + ": ");
    Serial.print(distanceCm, 0);
    Serial.print("cm");
    Serial.println();
  }

  return distanceCm;
}

void sound_buzzer(float distance) {
  int max_mapped_distance = 4;
//  int mapped_distance = map(distance > max_distance ? max_distance : distance, 23, max_distance, 1, max_mapped_distance);
  
  if (distance <= max_distance) {
    if (distance < 50) {
      tone(BUZZER_PIN, 2000, 200);
    } else if (distance < 100) {
      tone(BUZZER_PIN, 2000, 40);
      delay(100);
    } else if (distance < 150) {
      tone(BUZZER_PIN, 2000, 40);
      delay(500);
    }
  }
}
