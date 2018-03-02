void setupStopButton() {
  // register callbacks
  stopButton.registerCallbacks(StopButton_pressedCallback, StopButton_releasedCallback, StopButton_pressedDurationCallback);
  stopButton.setup(STOP_BUTTON, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
}

void StopButton_pressedCallback()
{
  // handle pressed state
  Serial.println("stop pressed");
  isRunning = false;
  stepper.enableOutputs();
  turnOffRelay();
  isStopped = true;
  stepper.stop();
}

void StopButton_releasedCallback()
{
  // handle released state
  //  digitalWrite(pinLED, LOW); // turn the LED off
  //  Serial.println("LOW");
}

void StopButton_pressedDurationCallback(unsigned long duration)
{
  // handle still pressed state
  //  Serial.print("HIGH still pressed (");
  //  Serial.print(duration);
  //  Serial.println("ms)");
  if (duration > 1000) {
    stepper.disableOutputs();
  }
}
