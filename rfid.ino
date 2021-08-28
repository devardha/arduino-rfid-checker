#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define RED_PIN         4
#define GREEN_PIN       5
#define BUZZ_PIN        2

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);
	SPI.begin();
	mfrc522.PCD_Init();

  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
	
	// Dump debug info about the card; PICC_HaltA() is automatically called
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  String content= "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++){
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();

  content.toUpperCase();

  if (content.substring(1) == "04 5C 71 BA 8F 61 80"){
    Serial.println("Authorized access");
    digitalWrite(GREEN_PIN, HIGH);

    tone(BUZZ_PIN, 1500, 200);

    delay(1000);
    digitalWrite(GREEN_PIN, LOW);
  }else   {
    Serial.println("Access denied");
    
    digitalWrite(RED_PIN, HIGH);
    tone(BUZZ_PIN, 750, 200);

    delay(1000);
    digitalWrite(RED_PIN, LOW);
  }
}
