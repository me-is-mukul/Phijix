const int photoPin = A0;
const int ledPin = 9;
const int threshold = 600;

bool lastState = false;
unsigned long lastChangeTime = 0;
unsigned long currentTime = 0;
unsigned long lastActivityTime = 0;

String currentSymbol = "";
String fullMessage = "";

// Morse code lookup
String morseLetters[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};
char alphabet[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z'
};

bool endDecoded = false;  // To prevent double-decoding

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Receiver ready. Listening for Morse...");
}

void loop() {
  int lightValue = analogRead(photoPin);
  int invertedLight = 1023 - lightValue;
  currentTime = millis();

  bool currentState = invertedLight > threshold;

  if (currentState != lastState) {
    unsigned long duration = currentTime - lastChangeTime;

    if (currentState == true) {
      if (duration >= 1600) {
        decodeCurrentSymbol();
        fullMessage += " ";
        Serial.println("[ Word Gap ]");
      } else if (duration >= 800) {
        decodeCurrentSymbol();
        Serial.println("[ Letter Gap ]");
      }
    } else {
      if (duration < 600) {
        currentSymbol += ".";
        Serial.println("Detected DOT");
      } else {
        currentSymbol += "-";
        Serial.println("Detected DASH");
      }
    }

    lastChangeTime = currentTime;
    lastActivityTime = currentTime;
    lastState = currentState;
    endDecoded = false;
  }

  // Decode if idle for 2 seconds
  if (!endDecoded && (currentTime - lastActivityTime) > 2000 && currentSymbol.length() > 0) {
    decodeCurrentSymbol();
    Serial.println("=== End of Transmission ===");
    Serial.print("Full Message: ");
    Serial.println(fullMessage);
    endDecoded = true;
  }

  digitalWrite(ledPin, currentState ? LOW : HIGH);
  delay(10);
}

void decodeCurrentSymbol() {
  if (currentSymbol.length() == 0) return;

  char decoded = '?';
  for (int i = 0; i < 26; i++) {
    if (currentSymbol == morseLetters[i]) {
      decoded = alphabet[i];
      break;
    }
  }

  Serial.print("Decoded Letter: ");
  Serial.println(decoded);
  fullMessage += decoded;
  currentSymbol = "";

  Serial.print("Current Message: ");
  Serial.println(fullMessage);
}
