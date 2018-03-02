void setupForwardButton() {
  // register callbacks
  forwardButton.registerCallbacks(ForwardButton_pressedCallback, ForwardButton_releasedCallback, ForwardButton_pressedDurationCallback);
  forwardButton.setup(FORWARD_BUTTON, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
}


void ForwardButton_pressedCallback()
{
  // handle pressed state
  Serial.println("forward pressed");
//  stepper.setMaxSpeed(getSpeedInSteps()); 
  stepper.setSpeed(getSpeedInSteps());  
  stepper.enableOutputs();    
  turnOffRelay();
  isRunning = false;
  isStopped = false;
  isforward = true;  
  isforwardBackwardpressed = true;
}

void ForwardButton_releasedCallback()
{
  // handle released state
  //  digitalWrite(pinLED, LOW); // turn the LED off
  //  Serial.println("LOW");
  isStopped = true;  
  isforwardBackwardpressed = false;
}

void ForwardButton_pressedDurationCallback(unsigned long duration)
{
  if (duration > 300) {
    stepper.runSpeed();
    if (duration % 1000 == 0) {
      // handle still pressed state
      Serial.print("forward still pressed (");
      Serial.print(duration);
      Serial.println("ms)");
    }
  }
}
