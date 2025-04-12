#define LASER_CONTROL_PIN 9

const char* morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

const int DOT_DURATION = 300;
const int DASH_DURATION = 900;
const int SYMBOL_GAP = 300;     // gap between dot/dash in same letter
const int LETTER_GAP = 900;     // gap between letters
const int WORD_GAP = 1800;      // gap between words

void setup() {
  pinMode(LASER_CONTROL_PIN, OUTPUT);
  digitalWrite(LASER_CONTROL_PIN, LOW); 
  Serial.begin(9600);
}

void sendMorse(String message) {
  message.toUpperCase();

  for (int i = 0; i < message.length(); i++) {
    char c = message[i];
    
    if (c >= 'A' && c <= 'Z') {
      String code = morseCode[c - 'A'];
      Serial.print(c);
      Serial.print(" : ");
      Serial.println(code);
      
      for (int j = 0; j < code.length(); j++) {
        digitalWrite(LASER_CONTROL_PIN, HIGH);
        delay(code[j] == '.' ? DOT_DURATION : DASH_DURATION);
        digitalWrite(LASER_CONTROL_PIN, LOW);

        delay(SYMBOL_GAP);  // gap between symbols
      }
      delay(LETTER_GAP - SYMBOL_GAP); // extra delay to complete letter gap
    } 
    else if (c == ' ') {
      delay(WORD_GAP);
    }
  }

  delay(2000); // final pause after sending
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    message.trim();
    Serial.println("Sending: " + message);
    sendMorse(message);
  }
}
