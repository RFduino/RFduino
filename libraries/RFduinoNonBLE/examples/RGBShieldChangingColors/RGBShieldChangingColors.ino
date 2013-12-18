int led_red = 2;
int led_green = 3;
int led_blue = 4;
int led_yellow = 5;
int i = 0;
int mode = 0;

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_yellow, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  
  analogWrite(led_red, i);
  analogWrite(led_green, (255 - i));
  analogWrite(led_blue, i);
  analogWrite(led_yellow, (255 - i));

  if (mode == 0)
  {
    if (++i >= 255)
    {
      mode = 1;
    }
  }
  else
  {
    if (--i == 0)
    {
      mode = 0;
    }
  }
  delay(25);
}

