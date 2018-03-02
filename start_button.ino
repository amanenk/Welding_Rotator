void setupStart() {
  // register callbacks
  start.registerCallbacks(start_pressedCallback, start_releasedCallback, start_pressedDurationCallback);
  start.setup(START_BUTTON, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
}

void start_pressedCallback()
{
  // handle pressed state
  Serial.println("start pressed");
  stepper.setCurrentPosition(0);  
  Destination = int((360 + getOverlaping()) * reducrorKoef / degPerStep);  

  //  speed in steps per second  
//  stepper.setAcceleration(100);
  stepper.setMaxSpeed(getSpeedInSteps());
  stepper.setAcceleration(1000);  
  stepper.enableOutputs();
  isRunning = true;
  isStopped = false;
  if (digitalRead(DIRRECTION_SWITCH)) {
    Serial.println("dirrection is positive");
    stepper.moveTo(Destination);
  } else {
    Serial.println("dirrection is negative");
    Destination = -Destination;
    stepper.moveTo(Destination);
  }
  turnOnRelay();
}

void start_releasedCallback()
{
  // handle released state
  //  digitalWrite(pinLED, LOW); // turn the LED off
  //  Serial.println("LOW");
}

void start_pressedDurationCallback(unsigned long duration)
{
  // handle still pressed state
  //  Serial.print("HIGH still pressed (");
  //  Serial.print(duration);
  //  Serial.println("ms)");
}
