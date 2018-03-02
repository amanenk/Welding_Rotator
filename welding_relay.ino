void turnOnRelay(){  
  Serial.println("turning on relay");
  isWelding = true;
  digitalWrite(RELAY_OUT,HIGH);
}

void turnOffRelay(){    
  Serial.println("turning off relay");  
  isWelding = false;
  digitalWrite(RELAY_OUT,LOW);  
}

boolean isRelayOn(){    
//  Serial.println("getting state off relay");
  digitalRead(RELAY_OUT);  
}

