#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          4          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}

//*****************************************************************************************//
void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;

  //-------------------------------------------

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid)); 
  //-------------------------------------------

  //------------------------------------------- GET First name
  Serial.print(F("Name: "));
  block = 8;
  getData(block, 11, key);
  Serial.println(" ");
  //-------------------------------------------

  //------------------------------------------- GET Last name
  Serial.print(F("Last name: "));
  block = 10;
  getData(block, 11, key);
  Serial.println(" ");
  //-------------------------------------------

  //------------------------------------------- GET Middle name
  Serial.print(F("Middle name: "));
  block = 13;
  getData(block, 15, key);
  Serial.println(" ");
  //-------------------------------------------

  //------------------------------------------- GET Grade
  Serial.print(F("Grade: "));
  block = 16;
  getData(block, 19, key);
  Serial.println(" ");
  //-------------------------------------------

  //------------------------------------------- GET Group
  Serial.print(F("Group: "));
  block = 18;
  getData(block, 19, key);
  Serial.println(" ");
  //-------------------------------------------

  delay(1000); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void getData(byte block, byte authBlock, MFRC522::MIFARE_Key &key) {
  MFRC522::StatusCode status;
  byte buffer1[32];
  byte len = 32;
  
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, authBlock, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  for (uint8_t i = 0; i < 32; i++) {
    if (buffer1[i] != 0x20) {
      Serial.write(buffer1[i]);
    } else {
      return;
    }
  }

  Serial.print(" ");
}
//*****************************************************************************************//
