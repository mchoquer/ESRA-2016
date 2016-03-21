int switchState = 0;
const int switchPin = 12;
const int lightPin = 11;
long time = 0;
long debounce = 200;

void setup() {
  Serial.begin(9600);
  pinMode(switchPin, INPUT);
  pinMode(lightPin, OUTPUT);
}

void loop() {
  switchState = digitalRead(switchPin);
  if( switchState == HIGH && millis() - time > debounce)
    digitalWrite(lightPin, HIGH);
  else
    Serial.print(switchState);


}
