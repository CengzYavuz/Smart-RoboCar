#include <AFMotor.h>

typedef void (*ActionFunction)();     // What the car should do
typedef bool (*ConditionFunction)();  // When to do it

struct Event {
  ActionFunction action;
  ConditionFunction condition;
  unsigned long duration;      // Optional: how long to do the action (ms)
  unsigned long startTime;
  bool isFinished;
};


const int MAX_EVENTS = 25;
Event eventQueue[MAX_EVENTS];
int eventCount = 0;
int currentEventPointer = 0;

bool CorrectFlag = true;
// Ultrasonik sensör pinleri
#define TRIG_PIN A1
#define ECHO_PIN A0


// Motor tanımları
AF_DCMotor leftFront(4);   // M1
AF_DCMotor leftBack(1);    // M2
AF_DCMotor rightFront(3);  // M3
AF_DCMotor rightBack(2);   // M4



void addEvent(ActionFunction a, ConditionFunction c, unsigned long dur = 0) {
  if (eventCount >= MAX_EVENTS) return;
  eventQueue[eventCount++] = {a, c, dur, 0, false};
}
Event* getNextEvent() {
  if (currentEventPointer >= eventCount) return nullptr;
  return &eventQueue[currentEventPointer];
}




float calculateDistanceCM(int step = 1) {
  float totalDistance = 0;

  for(int i = 0; i < step; i++){

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
                            // Ölçüm başlat
    long duration = pulseIn(ECHO_PIN, HIGH,23300); // Süreyi ölç
    float tempDistance = duration * 0.034 / 2;

    if(tempDistance > 2 && tempDistance < 400){
      totalDistance += tempDistance;
    }else{
      i--;
    }
    delay(60);  // Mesafeyi cm cinsinden hesapla

  }

  totalDistance = totalDistance/step;
  return totalDistance;

}
void AdjustMiddle(float setPoint){
  float distance = calculateDistanceCM(2);
  float error = fabs(setPoint - distance); // Ne kadar yakınız?
  Serial.println(distance);
  // Eğer çok yaklaştıysa, dur
  if (error < 2.0) {
    stopMotors();
    CorrectFlag = false;
    return;
  }

  // Hedefe ne kadar yakınsa, o kadar yavaş git
  int speed = 0;
  if (error > 20) {
    speed = 255;
  } else if (error > 10) {
    speed = 200;
  } else {
    speed = 150;
  }

  // Hedefin önünde misin yoksa gerisinde mi?
  if (distance > setPoint) {
    setForward(speed); // Geri git
  } else {
    setBackward(speed);  // İleri git
  }
  delay(70);
  if(error < 30){
    stopMotors();

  }

}





// Hareket fonksiyonları
void setForward(int speed) {
  leftFront.run(FORWARD);
  leftBack.run(FORWARD);
  rightFront.run(FORWARD);
  rightBack.run(FORWARD);

  leftFront.setSpeed(speed);
  leftBack.setSpeed(speed);
  rightFront.setSpeed(speed);
  rightBack.setSpeed(speed);
}
void setBackward(int speed) {
  leftFront.run(BACKWARD);
  leftBack.run(BACKWARD);
  rightFront.run(BACKWARD);
  rightBack.run(BACKWARD);

  leftFront.setSpeed(speed);
  leftBack.setSpeed(speed);
  rightFront.setSpeed(speed);
  rightBack.setSpeed(speed);
}
void setLeft(int speed) {
  leftFront.run(BACKWARD);
  leftBack.run(BACKWARD);
  rightFront.run(FORWARD);
  rightBack.run(FORWARD);

  leftFront.setSpeed(speed);
  leftBack.setSpeed(speed);
  rightFront.setSpeed(speed);
  rightBack.setSpeed(speed);
}
void setRight(int speed) {
  leftFront.run(FORWARD);
  leftBack.run(FORWARD);
  rightFront.run(BACKWARD);
  rightBack.run(BACKWARD);

  leftFront.setSpeed(speed);
  leftBack.setSpeed(speed);
  rightFront.setSpeed(speed);
  rightBack.setSpeed(speed);
}

void stopMotors() {
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
  rightFront.run(RELEASE);
  rightBack.run(RELEASE);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Setup completed.");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //EVENTS
  CorrectFlag = true;

  addEvent(
  []() { setForward(255); },                    // Action
  []() { return true; }, // Condition
  2500                                          // No time limit
);
 
  addEvent(
  []() { AdjustMiddle(26); },                    // Action
  []() { return CorrectFlag; }, // Condition
  0                                          // No time limit
);
  addEvent(
  []() { setLeft(255); },                    // Action
  []() { return true; }, // Condition
  330                                        // No time limit
);
  addEvent(
  []() { setForward(255); },                    // Action
  []() { return true; }, // Condition
  2000                                          // No time limit
);
  addEvent(
  []() { AdjustMiddle(80); },                    // Action
  []() { return CorrectFlag; }, // Condition
  0                                          // No time limit
);
  addEvent(
  []() { setLeft(255); },                    // Action
  []() { return true; }, // Condition
  330                                        // No time limit
);
 addEvent(
  []() { setForward(255); },                    // Action
  []() { return true; }, // Condition
  750                                          // No time limit
);
  addEvent(
  []() { AdjustMiddle(32.5); },                    // Action
  []() { return CorrectFlag; }, // Condition
  0                                          // No time limit
);
  addEvent(
  []() { setRight(255); },                    // Action
  []() { return true; }, // Condition
  330                                          // No time limit
);
 addEvent(
  []() { setForward(255); },                    // Action
  []() { return true; }, // Condition
  700                                          // No time limit
);

  addEvent(
  []() { AdjustMiddle(30); },                    // Action
  []() { return CorrectFlag; }, // Condition
  0                                          // No time limit
);
  addEvent(
  []() { setLeft(255); },                    // Action
  []() { return true; }, // Condition
  330                                          // No time limit
);
 addEvent(
  []() { setForward(255); },                    // Action
  []() { return true; }, // Condition
  1500                                          // No time limit
);
addEvent(
  []() { AdjustMiddle(12); },                    // Action
  []() { return CorrectFlag; }, // Condition
  0                                          // No time limit
);
  addEvent(
  []() { stopMotors(); },                    // Action
  []() { return true; }, // Condition
  1                                          // No time limit
);



}

void loop() {
  Serial.println(currentEventPointer);
  Event* current = getNextEvent();
  if (current == nullptr) return;  // All events finished

  if (!current->isFinished) {
    if (current->condition()) {
      current->startTime = millis();
      current->isFinished = true;
    } else {
      return;  // Wait until condition is met
    }
  }

  // Run the action
  current->action();

  // Finish after duration (if duration is specified)
  if (current->duration > 0 && millis() - current->startTime >= current->duration) {
    current->isFinished = true;
    currentEventPointer++;
    CorrectFlag = true;
  }

  // OR finish when condition becomes false (for open-ended events)
  if (current->duration == 0 && !current->condition()) {
    current->isFinished = true;
    currentEventPointer++;
    CorrectFlag = true;
  }
}