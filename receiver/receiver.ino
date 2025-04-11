// Receiver Arduino Code
#define SENSOR_PIN A0

String letterBuffer = "";  // Stores dots/dashes for one letter
String morseBuffer = "";  // Stores decoded letters
char decodedMessage[100];  // Stores final message
int msgIndex = 0;
unsigned long lastTime = 0;

// Morse code lookup table (A-Z)
const char* morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
  "..-", "...-", ".--", "-..-", "-.--", "--.."
};

// User-defined function: Decodes a Morse letter
void decodeLetter() {
  for (int i = 0; i < 26; i++) {
    if (letterBuffer == morseCode[i]) {
      char letter = 'A' + i;
      morseBuffer += letter;
      decodedMessage[msgIndex++] = letter;
      printMessage();
      break;
    }
  }
  letterBuffer = "";
}

// User-defined function: Prints the decoded message
void printMessage() {
  Serial.print("Received: ");
  for (int i = 0; i < msgIndex; i++) {
    Serial.print(decodedMessage[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);
  bool isLightOn = sensorValue > 600;  // Adjust threshold after testing

  unsigned long currentTime = millis();

  // Detect start of a pulse (laser ON)
  if (isLightOn && lastTime == 0) {
    lastTime = currentTime;
  }
  // Detect end of a pulse (laser OFF)
  else if (!isLightOn && lastTime != 0) {
    unsigned long duration = currentTime - lastTime;
    if (duration > 100 && duration < 400) {
      letterBuffer += ".";  // Dot (200ms ± 100ms)
    } else if (duration > 400 && duration < 800) {
      letterBuffer += "-";  // Dash (600ms ± 200ms)
    }
    lastTime = 0;
  }

  // Detect gaps to end a letter or word
  if (!isLightOn && letterBuffer != "" && currentTime - lastTime > 400 && lastTime != 0) {
    decodeLetter();
    lastTime = 0;
  }
  if (!isLightOn && morseBuffer != "" && currentTime - lastTime > 1200 && lastTime != 0) {
    morseBuffer = "";
    decodedMessage[msgIndex++] = ' ';
    printMessage();
    lastTime = 0;
  }
}