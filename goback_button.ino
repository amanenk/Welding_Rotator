void setupGobackButton() {
  // register callbacks
  gobackButton.registerCallbacks(GobackButton_pressedCallback, GobackButton_releasedCallback, GobackButton_pressedDurationCallback);
  gobackButton.setup(GOBACK_BUTTON, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
}

void GobackButton_pressedCallback()
{
  // handle pressed state
  Serial.println("goback pressed");    
  stepper.setMaxSpeed(getSpeedInSteps());
  stepper.setAcceleration(1000);  
  stepper.enableOutputs();
  isRunning = true;  
  isStopped = false;
  isGoingBack = true;
  turnOffRelay();
  stepper.moveTo(0);
}

void GobackButton_releasedCallback()
{
  // handle released state
  //  digitalWrite(pinLED, LOW); // turn the LED off
  //  Serial.println("LOW");
}

void GobackButton_pressedDurationCallback(unsigned long duration)
{
  // handle still pressed state
  //  Serial.print("HIGH still pressed (");
  //  Serial.print(duration);
  //  Serial.println("ms)");
}
