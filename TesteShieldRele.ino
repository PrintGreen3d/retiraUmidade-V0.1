
const int Rele1 = 4;
const int Rele2 = 5;
const int Rele3 = 6;
const int Rele4 = 7;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Rele1, OUTPUT);
  pinMode(Rele2, OUTPUT);
  pinMode(Rele3, OUTPUT);
  pinMode(Rele4, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //digitalWrite(Rele1, HIGH);
  //digitalWrite(Rele2, HIGH);
  //digitalWrite(Rele3, HIGH);
  digitalWrite(Rele4, HIGH);
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(Rele1, LOW);
  digitalWrite(Rele2, LOW);
  //digitalWrite(Rele3, LOW);
  digitalWrite(Rele4, LOW);
  
  delay(1000);                       // wait for a second
}
