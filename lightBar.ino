const int leds = 4;
const int buttons = 2;
const int ledPins[] = {5, 6, 9, 10};
// potentiometer = A5;
const int buttonPins[] = {12, 11};



int brightness[leds];
int buttonState[buttons];
int lastButtonState[buttons];
int zoom = 0;
int zoomRate = 2;
int mode = 0;


void setup()
{
  for(int i = 0; i < leds; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
  for(int i = 0; i < buttons; i++)
  {
    pinMode(buttonPins[i], INPUT);
  }

  // init buttons
  for(int i = 0; i < buttons; i++)
  {
    buttonState[i] = 0;
    lastButtonState[i] = 0;
  }
  Serial.begin(9600);
}

int buttonRead(int button, int maxValue, int value)
{
  maxValue--;
  buttonState[button] = digitalRead(buttonPins[button]);
  if (buttonState[button] != lastButtonState[button])
  {
    if (buttonState[button] == HIGH)
    {
      if(value == maxValue)
      {
        value = 0;
      }
      else
      {
        value++;
      }
    }
  }
  lastButtonState[button] = buttonState[button];
  return value;
}

void loop()
{
  int sensorValue = analogRead(A0);
  int potValue = analogRead(A5);
  
  zoom = buttonRead(0, 3, zoom);
  mode = buttonRead(1, 2, mode);
  
  switch(mode)
  {
    case 0: sensorValue = 1024 - sensorValue;
      break;
  }

  if(zoom == 0)
  {
    //sensorValue = map(sensorValue, 0, 1024, 0, 1024 * zoomRate * (zoom + 1));
    potValue = map(potValue, 0, 1024, 0, 0);
  }
  else
  {
    sensorValue = map(sensorValue, 0, 1024, 0, 1024 * zoomRate * (zoom));
    potValue = map(potValue, 0, 1024, -0.5 * (1024 * zoomRate * (zoom)), 0.5 * (1024 * zoomRate * (zoom)));
  }
  
  

  for(int i = 0; i < leds; i++)
  {
    analogWrite(ledPins[i], constrain(sensorValue - (i * 256) + potValue, 0, 255));
    Serial.print(i);
    Serial.print(":");
    Serial.print(constrain(sensorValue - (i * 256) + potValue, 0, 255));
    Serial.print(" ");
  }
  
  Serial.print(potValue);
  Serial.print(" ");
  Serial.println(sensorValue);
  delay(10);
}

