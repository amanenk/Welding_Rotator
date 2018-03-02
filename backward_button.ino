void setupBackwardButton() {
  // register callbacks
  backwardButton.registerCallbacks(BackwardButton_pressedCallback, BackwardButton_releasedCallback, BackwardButton_pressedDurationCallback);
  backwardButton.setup(BACKWARD_BUTTON, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
}


void BackwardButton_pressedCallback()
{
  // handle pressed state
  Serial.println("backward pressed");
  //  stepper.setMaxSpeed(getSpeedInSteps());
  stepper.setSpeed(-getSpeedInSteps());
  stepper.enableOutputs();
  turnOffRelay();
  isRunning = false;
  isStopped = false;
  isforward = false;

  isforwardBackwardpressed = true;
}

void BackwardButton_releasedCallback()
{
  // handle released state
  //  digitalWrite(pinLED, LOW); // turn the LED off
  //  Serial.println("LOW");
  isStopped = true;
  isforwardBackwardpressed = false;
}

void BackwardButton_pressedDurationCallback(unsigned long duration)
{
  if (duration > 300) {
    stepper.runSpeed();
    if (duration % 1000 == 0) {
      // handle still pressed state
      Serial.print("backward still pressed (");
      Serial.print(duration);
      Serial.println("ms)");
    }
  }
}
