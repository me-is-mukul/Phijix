
#define LASER_CONTROL_PIN 9


const char* morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void setup() {
  pinMode(LASER_CONTROL_PIN, OUTPUT);
  digitalWrite(LASER_CONTROL_PIN, LOW); 
  Serial.begin(9600);
}

void sendMorse(String message) {
  for (int i = 0; i < message.length(); i++) {
    char c = message[i];
    if (c >= 'A' && c <= 'Z') {
      String code = morseCode[c - 'A'];
      for (int j = 0; j < code.length(); j++) {
        digitalWrite(LASER_CONTROL_PIN, HIGH);
        if (code[j] == '.') {
          delay(200);
        } else {
          delay(600);
        }
        digitalWrite(LASER_CONTROL_PIN, LOW);
        delay(200);
      }
      delay(600);
    } else if (c == ' ') {
      delay(1400);
    }
  }
  delay(2000);
}

void loop() {
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    message.trim();
    message.toUpperCase();
    Serial.println("Sending: " + message);
    sendMorse(message);

  }
}

