#include <AccelStepper.h>
#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   30   // [ms]
#define POT_DELAY_IN 0
#define POT_SPEED_IN 1
#define START_BUTTON 2
#define STOP_BUTTON 3
#define GOBACK_BUTTON 4
#define DIRRECTION_SWITCH 5
#define FORWARD_BUTTON 6
#define BACKWARD_BUTTON 7
#define STEPPER_DIRECTION 8
#define STEPPER_STEP 9
#define STEPPER_ENABLE 10
#define RELAY_OUT 11
#define LED_1 12
#define LED_2 13
#define BLINK_INTERVAL 200
//46/15

AccelStepper stepper(AccelStepper::DRIVER, STEPPER_STEP, STEPPER_DIRECTION);
static InputDebounce start; // not enabled yet, setup has to be called later
static InputDebounce stopButton; // not enabled yet, setup has to be called later
static InputDebounce gobackButton; // not enabled yet, setup has to be called later
static InputDebounce forwardButton; // not enabled yet, setup has to be called later
static InputDebounce backwardButton; // not enabled yet, setup has to be called later

static const double reducrorKoef = 46 / 15;
static const double degPerStep = 0.45;
static const double min_Speed = 0.5;
static const double max_Speed = 5;
static const double min_Overlapping = 0;
static const double max_Overlapping = 15;
int SpeedInSteps = 0;
static double Destination = 0;
int last_position = 0;
bool isWelding = false;
bool isRunning = false;
unsigned long previousMillis = 0;        // will store last time LED was updated
int ledState = LOW;             // ledState used to set the LED
bool isStopped = true;
bool isforward = false;
bool isforwardBackwardpressed = false;
bool isGoingBack = false;

void setup()
{
  Serial.begin(115200);
  pinMode(DIRRECTION_SWITCH, INPUT);
  pinMode(RELAY_OUT, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  stepper.setEnablePin(STEPPER_ENABLE);
  stepper.setMaxSpeed(10);
  stepper.setPinsInverted (false, false, true);
  stepper.enableOutputs();
  setupStart();
  setupBackwardButton();
  setupForwardButton();
  setupGobackButton();
  setupStopButton();
  turnOffRelay();
}

void loop()
{
  unsigned long currentMillis = millis();
  // poll button state
  start.process(currentMillis); // callbacks called in context of this function
  stopButton.process(currentMillis); // callbacks called in context of this function
  gobackButton.process(currentMillis); // callbacks called in context of this function
  forwardButton.process(currentMillis); // callbacks called in context of this function
  backwardButton.process(currentMillis); // callbacks called in context of this function

  delay(1); // [ms]
  handleStepper();
  handleLED(currentMillis);

}

void handleStepper() {
  if (isRunning)
    stepper.run();

  //  if (stepper.currentPosition() != 0 &&  millis() % 1000 == 0  &&  stepper.isRunning() && last_position != stepper.currentPosition()) {
  //    Serial.print("currentPosition = ");
  //    Serial.print(stepper.currentPosition());
  //    Serial.print("/");
  //    Serial.println(stepper.distanceToGo());
  //    last_position = stepper.currentPosition();
  //  }
  if (stepper.distanceToGo() == 0 && isWelding) {
    Serial.println("stepper in position");
    turnOffRelay();
    isStopped = true;
    //    last_position = stepper.currentPosition();
  }
  if (stepper.distanceToGo() == 0 && isGoingBack) {
    Serial.println("stepper in 0 position");
    isStopped = true;
    isGoingBack = false;
    //    last_position = stepper.currentPosition();
  }
}

void handleLED(unsigned long currentMillis) {
  if (currentMillis - previousMillis >= BLINK_INTERVAL) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    if (!isforwardBackwardpressed) {
      if (!digitalRead(DIRRECTION_SWITCH)) {
        if (!isStopped) {
          digitalWrite(LED_1, ledState);
        } else {
          digitalWrite(LED_1, HIGH);
        }
        digitalWrite(LED_2, LOW);
      } else {
        digitalWrite(LED_1, LOW);
        if (!isStopped) {
          digitalWrite(LED_2, ledState);
        } else {
          digitalWrite(LED_2, HIGH);
        }
      }
    } else {
      if (isforward) {
        if (!isStopped) {
          digitalWrite(LED_1, ledState);
        } else {
          digitalWrite(LED_1, HIGH);
        }
        digitalWrite(LED_2, LOW);
      } else {
        digitalWrite(LED_1, LOW);
        if (!isStopped) {
          digitalWrite(LED_2, ledState);
        } else {
          digitalWrite(LED_2, HIGH);
        }
      }
    }
  }
}

double getOverlaping() {
  Serial.print("analogRead(POT_DELAY_IN = ");
//  delay(500);
  int analog = analogRead(POT_DELAY_IN);
  Serial.println(analog);
  double Overlapping = min_Overlapping + ((analog * (max_Overlapping - min_Overlapping) / 1024));
  Serial.print("Overlapping = ");
  Serial.println(Overlapping);
  return Overlapping;
}

double getSpeedInSteps() {
  Serial.print("analogRead(POT_SPEED_IN = ");  
//  delay(500);
  int analog = analogRead(POT_SPEED_IN);
  Serial.println(analog);
  double move_speed = min_Speed + (analog * (max_Speed - min_Speed) / 1024); // turns per minute
  float speed_in_steps = move_speed * 360 / degPerStep / 60 * reducrorKoef;
  Serial.print("speed_in_steps = ");
  Serial.println(speed_in_steps);
  return speed_in_steps;
}


