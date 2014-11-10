
#include <Adafruit_NeoPixel.h>

#include <SD.h>
//Git Checkin
//Written for Teensy 3


long frameTime = 0;

const int DATAPIN = 6;
int LEDCOUNT = 1;
const int BUTTON_PIN = 8;

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, DATAPIN, NEO_GRB + NEO_KHZ400);

//SD Card Control
File myFile;
const int chipSelect = 20; //pin for DS comms
int fileToPlay = 1; //start with first file
boolean newProgram = false; //Indicates if the next animation should be loaded. Set by button press

//SD Card Info
Sd2Card card;
SdVolume volume;
SdFile root;

void setup()
{
  Serial.begin(115200); //for outputting text for debugging
  Serial1.begin(38400); //for communicating program to other controllers
  //serial 1 = RX=0 TX=1
  
  //strip.begin();
  
  InitializePins();
  DiplayCardInfo();
  InitializeSD();
}

void loop()
{
  PlayDataToVest();
}



void PlayDataToVest()
{
  //first, load up the led count from file. 
  //Dependancy is that all files on the card are for the same LED array. 
  OpenFile();
  LEDCOUNT = (int)myFile.read();
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, DATAPIN, NEO_GRB + NEO_KHZ400);
  myFile.close(); 
  
  while(true)//replaces loop so that the led driver library can be maintained
  {
    long startTime = millis();
    OpenFile();
    
    //dispose the first header byte that contains the LED count
    myFile.read();
    
    while(myFile.available() && !newProgram) //untill there is no more data
    {
      frameTime = millis();
      CheckButtonPress();
      
      for(int i = 0; i<LEDCOUNT; i++)//itterate each led
      {
        strip.setPixelColor(i,GetOneLedDataFromFile(myFile));
      }
      
      strip.show();
     CheckForNewSerialProgram();
      delay(40 - (millis() - frameTime ));
      //frames++;
    }
    
    myFile.close();  
    //Serial.println( frames );
    //Serial.println( millis() - startTime);
  }
}

void OpenFile()
{
  switch(fileToPlay)
  {
  case 1:
    myFile = SD.open("01.led");
    Serial.println("Playing file 01");
    break;
  case 2:
    myFile = SD.open("02.led");
    Serial.println("Playing file 02");
    break;
  case 3:
    myFile = SD.open("03.led");
    Serial.println("Playing file 03");
    break;  
  case 4:
    myFile = SD.open("04.led");
    Serial.println("Playing file 04");
    break;
  case 5:
    myFile = SD.open("05.led");
    Serial.println("Playing file 05");
    break;
  case 6:
    myFile = SD.open("06.led");
    Serial.println("Playing file 06");
    break;
  case 7:
    myFile = SD.open("07.led");
    Serial.println("Playing file 07");
    break;
  case 8:
    myFile = SD.open("08.led");
    Serial.println("Playing file 08");
    break;
  case 9:
    myFile = SD.open("09.led");
    Serial.println("Playing file 09");
    break;
  case 10:
    myFile = SD.open("10.led");
    Serial.println("Playing file 10");
    break;
  case 11:
    myFile = SD.open("11.led");
    Serial.println("Playing file 11");
    break;
  case 12:
    myFile = SD.open("12.led");
    Serial.println("Playing file 12");
    break;
  }
  newProgram = false;//we have a new program loaded now
  
}


void InitializeSD()
{
  Serial.print("Initializing SD card...");
   
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("SD Initialization Failed!");
    return;
  }
  
  Serial.println("SD Initialization OK.");
}

uint32_t GetOneLedDataFromFile(File file)//gets 24 bits, return as 32
{
  uint32_t result;
  result = file.read();//msb
  result<<=8;
  result |= file.read();
  result<<=8;
  result |= file.read();//lsb
  return result;
}

void DiplayCardInfo()
{

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }

  
  
 // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


void InitializePins()
{
  pinMode(53, OUTPUT);  //to talk to the SD card, 10 on UNO, 53 on mega
  
  //led light indicator
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  pinMode(BUTTON_PIN, INPUT); //button
  digitalWrite(BUTTON_PIN, HIGH);
}

void FlashLed()
{
      digitalWrite(13, HIGH);
      delay(1);
      digitalWrite(13, LOW);
}

void CheckButtonPress()
{
  if(digitalRead(BUTTON_PIN) == LOW)
  {
    delay(5);//debounce
    if(digitalRead(BUTTON_PIN) == LOW)
    {
      newProgram = true;
      if(fileToPlay == 12)
      { 
         fileToPlay = 1; 
      }
      else 
      {
        fileToPlay++;
      }
      //wait for let go of button
      while(digitalRead(BUTTON_PIN) == LOW) {;}
      delay(1);//debounce
      //Broadcast the new pattern.
      Serial1.print(fileToPlay);//send the change to the serial comms
    }
  }
}

void CheckForNewSerialProgram()
{
  byte incomingByte;
  while (Serial1.available() > 0) 
  { 
    digitalWrite(13, HIGH); //flash to show recieved byte
    incomingByte = Serial1.read();
    digitalWrite(13, LOW); //reset LED
  }
  
  //is numeric and different to what is currently playing
  if(incomingByte >= 1 && incomingByte <= 12 && incomingByte != fileToPlay)
  {
    Serial1.print(incomingByte);//send the change to the serial comms
    fileToPlay = incomingByte;
    newProgram = true;
  }
}
