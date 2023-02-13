/*
Relay - Pin 8
Contact water sensor - Pin 10
Float sensor - Pin 16 (via 10K resistor)
Button - Pin 9 (via 10K resistor)
*/

int pumpRelayPin = 8;
int highWaterLevelPin = 10;
int lowWaterLevelPin = 16;
int manualTriggerPin = 9;
bool runPump = false;
bool manualDisable = false;

void setup() {
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(highWaterLevelPin, INPUT_PULLUP);
  pinMode(lowWaterLevelPin, INPUT_PULLUP);
  pinMode(manualTriggerPin, INPUT_PULLUP);
}

void loop() {
  if (runPump == true) {
    // If we need to start the pump, first make sure the tank isn't full and the pump
    // isn't already running. Start the pump and delay 1 second
    if (digitalRead(highWaterLevelPin) != HIGH && digitalRead(pumpRelayPin) == LOW) {
      digitalWrite(pumpRelayPin, HIGH);
      delay(1000);
    }
  } else if (digitalRead(pumpRelayPin) == HIGH) {
    // If we don't need to start the pump, stop the pump (assuming it's running)
    digitalWrite(pumpRelayPin, LOW);
    delay(1000);
  }

  // When the contact water sensor detects water, it goes HIGH. If this happens, the
  // tank is full. Stop the pump immediately to avoid overflow.
  if (digitalRead(highWaterLevelPin) == HIGH) {
   digitalWrite(pumpRelayPin, LOW);
   runPump = false;
   delay(1000);
  }

  // If the float sensor goes LOW, the tank is low and should be filled. Start the
  // pump (unless we manually stopped it via the button).
  if (digitalRead(lowWaterLevelPin) == LOW && manualDisable == false) {
    runPump = true;
  }

  // The button should toggle the pump state. If we disable the pump via the button,
  // it should not start again due to the low water sensor.
  if (digitalRead(manualTriggerPin) == LOW) {
    if (runPump == false) {
      // If we push the button and the pump is not running, disable the manual disable
      // and set the runPump flag.
      runPump = true;
      manualDisable = false;
    } else {
      // If we push the button and the pump is running, stop it immediately and set
      // the manual disable flag
      digitalWrite(pumpRelayPin, LOW);
      runPump = false;
      manualDisable = true;
      delay(1000);
    }
  }
}
