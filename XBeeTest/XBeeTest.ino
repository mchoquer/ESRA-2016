void setup() {
  Serial.begin(9600);

}

void loop() {
  while (1) {
    char a = 'a';
    for (int i = 0; i < 26; i++) {
      a++;
      Serial.print(a);
      delay(500);
    }
  }

}
