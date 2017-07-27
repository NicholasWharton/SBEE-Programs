#define Switch1 P2_1 // Pin for the left switch on the Launchpad.
#define HAND P6_4 // Pin to connect the servo motor control wire (Yellow).

#define servo_period 20 // Servo control signal period, in milliseconds.
#define pulse_width_min 600 // Servo control signal pulse width for -90 deg.
#define pulse_width_max 2400 // Servo control signal pulse width for +90 deg.

#define control_delay 1000 // Amount of time that must pass between changing 
                           // hand states, in milliseconds.

volatile boolean hand_state = 0; // hand_state = 0 => Servo motor angle = -90 deg.
                                 // hand_state = 1 => Servo motor angle = +90 deg.
                                 
volatile long int lastTriggered = 0; // A variable to store the time, in milliseconds,
                                     // of the last hand_state change.

int period = servo_period; // Set the period of the PWM servo control signal.
int pulse = pulse_width_min; // Set the pulse width to the shortest pulse (hand_state = 0).


void setup()
{
  pinMode(RED_LED, OUTPUT); // An indicator of the hand's state.
  pinMode(HAND, OUTPUT); // The servo motor's control (Yellow) wire.
  
  // Set switch to a pullup input. 
                                  // Note: Use "INPUT" instead of "INPUT PULLUP" for the EMG input.
  pinMode(Switch1, INPUT_PULLUP); 
  attachInterrupt(Switch1, flex, FALLING); // Run the function named "flex" every time there is a "FALLING"
                                           // edge detected on the "Switch1" pin. I.e. run flex() whenever the
                                           // input pin's voltage changes from 3.3 V to 0 V.
}

void loop() 
{
  digitalWrite(HAND, HIGH); // Begin the servo motor control signal pulse.
  delayMicroseconds(pulse); // Delay the appropriate amount of time for the current hand_state.
  digitalWrite(HAND, LOW); // End the control signal pulse.
  delay(period); // Delay the rest of the period.
  
  if(hand_state == 0) // Check the current hand state, then set the appropriate pulse width for the next cycle.
  {
    pulse = pulse_width_min;
  } 
  else 
  {
    pulse = pulse_width_max;
  }
}

void flex() // Runs whenever there is a falling edge hardware interrupt on the attached pin.
{
  if((millis() - lastTriggered) > control_delay) // Check that enough time has passed since the last event.
  {
    lastTriggered = millis(); // Record the current time.
    if(hand_state == 0) // Check the hand state.
    {
      hand_state = 1; // Set the new hand state.
      digitalWrite(RED_LED, HIGH); // Set the indicator LED to represent hand_state = 1.
    } 
    else 
    {
      hand_state = 0;
      digitalWrite(RED_LED, LOW);
    } 
  }  
}

