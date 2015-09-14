  #include <IRremote.h>
  //#include <LiquidCrystal.h>
//LiquidCrystal lcd(19, 18, 17, 16, 15, 14);

//Outputs of the arduino card
int echo = 5;
int trigger = 6;
int leftwheel_forward = 7;
int rightwheel_forward = 8;
int rec_infrarouge = 9;
int leftwheel_backward = 10;
int rightwheel_backward = 11;

long duration = 0;
long distance = 0;
long automatic = 0;

// Value for each key on the IR remote
long ir_keypad_1 = 2534850111;
long ir_keypad_2 = 1033561079;
long ir_keypad_3 = 1635910171;
long ir_keypad_4 = 2351064443;
long ir_keypad_5 = 1217346747;
long ir_keypad_6 = 71952287;
long ir_keypad_7 = 851901943;
long ir_keypad_8 = 465573243;
long ir_keypad_9 = 1053031451;

IRrecv irrecv(rec_infrarouge);
decode_results results;

void setup() {
  pinMode(leftwheel_forward, OUTPUT);
  pinMode(rightwheel_forward, OUTPUT);
  pinMode(leftwheel_backward, OUTPUT);
  pinMode(rightwheel_backward, OUTPUT);
  //lcd.begin(16, 2);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  //lcd.setCursor(0, 0);
  //lcd.print("www.funduino.de");
  //lcd.setCursor(0, 1);
  if(irrecv.decode(&results)) 
  {
	if (results.value == ir_keypad_2)
		forward(1000);
    else if(results.value == ir_keypad_4)
		turn_left(500);
    else if(results.value == ir_keypad_6)
		turn_right(500);
    else if(results.value == ir_keypad_8)
		backward(1000);
    else if(results.value == ir_keypad_5)
    {
      automatic = (automatic == 0) ? 1 : 0;
      Serial.println(results.value, DEC);
    }
	else if (results.value == ir_keypad_9) // Demi Tour droite
		turn_right(1000);
	else if (results.value == ir_keypad_7) // Demi Tour gauche
		turn_left(1000);
    else
    {
      Serial.println(results.value, DEC);
    }
    irrecv.resume();
  }
  else if(automatic == 1)
  {
    if (calculate_distance() > 50)
    {
		forward();
		Serial.print(distance);
		Serial.println("cm");
    }
    else
    {
		digitalWrite(leftwheel_forward, LOW);
		digitalWrite(leftwheel_backward, HIGH);
		delay(2000); // Essayer de contourner obstacle
		digitalWrite(leftwheel_backward, LOW);
		digitalWrite(rightwheel_forward, LOW);
		Serial.print(distance);
		Serial.println("cm");
    }
  }
  else
  {
	  stop_all();
  }
}

void stop_all()
{
	digitalWrite(leftwheel_forward, LOW);
	digitalWrite(rightwheel_forward, LOW);
	digitalWrite(leftwheel_backward, LOW);
	digitalWrite(rightwheel_backward, LOW);
}

void forward(int time_of_running)
{
	digitalWrite(leftwheel_forward, HIGH);
	digitalWrite(rightwheel_forward, HIGH);
	delay(time_of_running);
	Serial.println(results.value, DEC);
}

void forward()
{
	digitalWrite(leftwheel_forward, HIGH);
	digitalWrite(rightwheel_forward, HIGH);
}

void backward(int time_of_running)
{
	digitalWrite(rightwheel_backward, HIGH);
	digitalWrite(leftwheel_backward, HIGH);
	delay(time_of_running);
	Serial.println(results.value, DEC);
}

void turn_left(int time_of_running)
{
	digitalWrite(rightwheel_forward, HIGH);
	digitalWrite(leftwheel_backward, HIGH);
	delay(time_of_running);
	Serial.println(results.value, DEC);
}

void turn_right(int time_of_running)
{
	digitalWrite(leftwheel_forward, HIGH);
	digitalWrite(rightwheel_backward, HIGH);
	delay(time_of_running);
	Serial.println(results.value, DEC);
}

long calculate_distance()
{
	digitalWrite(trigger, LOW);
	delay(5);
	digitalWrite(trigger, HIGH);
	delay(10);
	digitalWrite(trigger, LOW);
	duration = pulseIn(echo, HIGH);
	return ((duration / 2) / 29.1);
}
