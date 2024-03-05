#include <SD.h>
#include <Keyboard.h>

// Mapping function to convert string representation to macro
int stringToMacro(String macroString) {
	if (macroString == "ESCAPE" || macroString == "ESC") {
		return KEY_ESC;
	} else if (macroString == "MENU" || macroString == "APP") {
		return KEY_MENU;
	} else if (macroString == "END") {
		return KEY_END;
	} else if (macroString == "SPACE") {
		return ' ';
	} else if (macroString == "TAB") {
		return KEY_TAB;
	} else if (macroString == "PRINTSCREEN") {
		return KEY_PRINT_SCREEN;
	} else if (macroString == "ENTER") {
		return KEY_RETURN;
	} else if (macroString == "UPARROW" || macroString == "UP") {
		return KEY_UP_ARROW;
	} else if (macroString == "DOWNARROW" || macroString == "DOWN") {
		return KEY_DOWN_ARROW;
	} else if (macroString == "LEFTARROW" || macroString == "LEFT") {
		return KEY_LEFT_ARROW;
	} else if (macroString == "RIGHTARROW" || macroString == "RIGHT") {
		return KEY_RIGHT_ARROW;
	} else if (macroString == "CAPSLOCK") {
		return KEY_CAPS_LOCK;
	} else if (macroString == "DELETE" || macroString == "DEL") {
		return KEY_DELETE;
	} else if (macroString == "F1") {
		return KEY_F1;
	} else if (macroString == "F2") {
		return KEY_F2;
	} else if (macroString == "F3") {
		return KEY_F3;
	} else if (macroString == "F4") {
		return KEY_F4;
	} else if (macroString == "F5") {
		return KEY_F5;
	} else if (macroString == "F6") {
		return KEY_F6;
	} else if (macroString == "F7") {
		return KEY_F7;
	} else if (macroString == "F8") {
		return KEY_F8;
	} else if (macroString == "F9") {
		return KEY_F9;
	} else if (macroString == "F10") {
		return KEY_F10;
	} else if (macroString == "F11") {
		return KEY_F11;
	} else if (macroString == "F12") {
		return KEY_F12;
	} else if (macroString == "ALT") {
		return KEY_LEFT_ALT;
	} else if (macroString == "SHIFT") {
		return KEY_LEFT_SHIFT;
	} else if (macroString == "CTRL" || macroString == "CONTROL") {
		return KEY_LEFT_CTRL;
	} else if (macroString == "GUI" || macroString == "WINDOWS" || macroString == "COMMAND") {
		return KEY_LEFT_GUI;
	} else if (macroString.length() == 1 && macroString[0] >= 'a' && macroString[0] <= 'z') {
    return macroString[0];
  }
	// If the macro string is not recognized, return -1
	return -1;
}

// Opening files
File codeFile;
File logFile;

// Main function
void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	Keyboard.begin();

	// Initialize SD Card
	if (!SD.begin()) { // Pin number blank for Arduino MKR Zero
		Serial.println("[-] SD initialization failed!");
    errorLEDLoop();
		return;
	} else {
		Serial.println("[+] SD Initialized success!");
	}

	// Open "code.txt" from SD Card
	codeFile = SD.open("code.txt");
	if (!codeFile) {
		Serial.println("[-] Failed to open code.txt");
    errorLEDLoop();
		return;
	} else {
		Serial.println("[+] code.txt found in SD Card.");
	}

	// Open "log.txt" in SD Card
	logFile = SD.open("log.txt", FILE_WRITE);
	if (!logFile) {
		Serial.println("[-] Failed to open log.txt");
    errorLEDLoop();
		return;
	} else {
		Serial.println("[+] log.txt opened in SD card.");
	}

	delay(2000); // Delay for USB detection in Windows

	// Read each line from "code.txt"
	Serial.println("[+] Starting Execution (REPLAY or REPEAT does not work)...\n");
  int lineNum = 0; // Count line number
	while (codeFile.available()) {
		String line = codeFile.readStringUntil('\n');
		if (line.length() > 0) {
			logFile.println(line);
			Serial.println(line);
			lineNum++; // Calculates line number
			// Execute the ducky command which is in "line", if the function returns "1", FATAL error, execution stops

			if (executeCommand(line, lineNum) == 1) {
        Serial.println("\n[-] Execution stopped: A FATAL error has occured.");
				codeFile.close();
				logFile.close();
				Keyboard.end();
        errorLEDLoop();
				return;
			}
		}
	}

	codeFile.close();
	logFile.close();
	Keyboard.end();

	Serial.println("\n\n[+] End of code.");
	digitalWrite(LED_BUILTIN, HIGH); //Static LED if successfully executed
}

void loop() {
	// Nothing to do here
}

// Handling each command, returns "1" if any fatal error
int executeCommand(String line, int lineNum) {
	// Remove leading and trailing spaces
	line.trim();
  line.replace("    ", " "); // Removing all multiple spaces from substring
  line.replace("   ", " "); // Removing all multiple spaces from substring
  line.replace("  ", " "); // Removing all multiple spaces from substring

	// Check if the line is empty after removing spaces
	if (line.length() == 0) {
		Serial.print("\n[ ] Info: Skipped line ");
		Serial.print(lineNum);
		Serial.println(" because it was empty");
    // Small Blink for Info
    digitalWrite(LED_BUILTIN, HIGH);
    delay (10);
    digitalWrite(LED_BUILTIN, LOW);
		return 0;
	}

	// Separating command from argument
	int spaceIndex = line.indexOf(' ');
	String command = line.substring(0, spaceIndex);
	String argument = line.substring(spaceIndex + 1);

	// Main Ducky Command checks
	if (command == "STRING") {

    for (int i = 0; i < argument.length(); i += 25) { // Breaking the String into smaller chunks of 25 to prevent printing issues (RAM issues)
      String chunk = argument.substring(i, min(i + 25, argument.length()));
      Keyboard.print(chunk);
      delay(100);
    }
		//Keyboard.print(argument);
    //delay(100);
	} else if (command == "DELAY") {
		if (argument.length() > 0) {
			int delayVal = argument.toInt();
			if (delayVal != 0 || argument != "0") {
				delay(delayVal);
			} else {
				Serial.print("\n[-] FATAL: Line ");
				Serial.print(lineNum);
				Serial.println(" : DELAY only accepts numbers.");
				return 1;
			}
		} else {
			Serial.print("\n[ ] Info: Skipped line ");
			Serial.print(lineNum);
			Serial.println(" because no DELAY value is entered.");
      // Small Blink for Info
      digitalWrite(LED_BUILTIN, HIGH);
      delay (10);
      digitalWrite(LED_BUILTIN, LOW);

			return 0;
		}
	} else if (command == "TYPE") {
		if (argument.length() != 1) { // In TYPE, only one character can be passed
			Serial.print("\n[-] FATAL: Line ");
			Serial.print(lineNum);
			Serial.println(" : TYPE only accepts 1 character.");
			return 1;
		}
		typeKey(argument.toInt());
	} else if (command == "REM") {
		Serial.print("\n[ ] Info: Skipped line ");
		Serial.print(lineNum);
		Serial.println(" : REM ignored");
    // Small Blink for Info
    digitalWrite(LED_BUILTIN, HIGH);
    delay (10);
    digitalWrite(LED_BUILTIN, LOW);
		return 0;
	} else {
		int startIndex = 0;

		for (int i = 0; i <= line.length(); i++) {
			if (i == line.length() || line.charAt(i) == ' ') { // Finding Spaces one by one and separating them to substring
				String currentSubStr = line.substring(startIndex, i);
        startIndex = i + 1;
				int macroResult = stringToMacro(currentSubStr); // Returns the value of Key to be pressed as Integer
        
				if (macroResult == -1) {
					Keyboard.releaseAll();
					Serial.print("\n[-] FATAL: Line ");
					Serial.print(lineNum);
					Serial.print(" : unrecognized command.");
					return 1;
				}
        delay (50);
				Keyboard.press(macroResult);
			}
		}
		delay(100);
		Keyboard.releaseAll();
	}
	return 0;
}

// Function for keypress
void typeKey(int key) {
	Keyboard.press(key);
	delay(50);
	Keyboard.release(key);
}

// If error occurs while execution, LED blinks
void errorLEDLoop() {
  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay (100);
    digitalWrite(LED_BUILTIN, LOW);
    delay (100);
  }
}