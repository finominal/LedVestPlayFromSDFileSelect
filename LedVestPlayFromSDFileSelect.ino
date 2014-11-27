
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
//#include <Adafruit_NeoPixel.h>
#include "FastLED.h"
#include <SD.h>

=======
//#include <Adafruit_NeoPixel.h>
#include "FastLED.h"
#include <SD.h>

>>>>>>> FETCH_HEAD
//File entry =  dir.openNextFile();

const int DATA_PIN = 6;
uint16_t  LEDCOUNT = 0;
const int BUTTON_PIN = 8;

//SD Card Control
File myFile;
File cardRoot;

const int chipSelect = 20;
boolean newProgram = false;
int requestedProgram = 0;
int countFiles = 0;
int currentProgram;
<<<<<<< HEAD
>>>>>>> FETCH_HEAD
=======
>>>>>>> FETCH_HEAD

//SD Card Info
Sd2Card card;
SdVolume volume;
SdFile root;

void setup()
{
<<<<<<< HEAD
<<<<<<< HEAD
  Serial.begin(115200); //for outputting text for debugging
  Serial1.begin(38400); //for communicating program to other controllers
  //serial 1 = RX=0 TX=1
  
  //strip.begin();
=======
  //delay(2000); //dev catch startup info 
  Serial.begin(115200);
  Serial1.begin(9600);
>>>>>>> FETCH_HEAD
=======
  //delay(2000); //dev catch startup info 
  Serial.begin(115200);
  Serial1.begin(9600);
>>>>>>> FETCH_HEAD
  
  Serial.println("*Starting*");

  InitializePins();
<<<<<<< HEAD
<<<<<<< HEAD
  DiplayCardInfo();
  InitializeSD();
=======
  InitializeSD(); // will also display card info
  GetNextFile();  // get the first file
  GetLedCount(); //get the led count from the file header, two bytes combined.
>>>>>>> FETCH_HEAD
=======
  InitializeSD(); // will also display card info
  GetNextFile();  // get the first file
  GetLedCount(); //get the led count from the file header, two bytes combined.
>>>>>>> FETCH_HEAD
}

void loop()
{
  PlayDataToVest();
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
}

void GetLedCount()
{
  LEDCOUNT = myFile.read();
  LEDCOUNT<<=8;
  LEDCOUNT |= myFile.read(); 
  Serial.print("LED COUNT = "); Serial.println(LEDCOUNT);
  myFile.seek(0); //redet the file to start.
}

void PlayDataToVest()
{
  CRGB leds[LEDCOUNT];
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LEDCOUNT);
  
  //Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, DATA_PIN, NEO_GRB + NEO_KHZ400);
  //strip.begin();
  
  while(true)//replaces loop so led array size can be sized dynamically from file
  {
    if(newProgram)
    {
      if(requestedProgram > 0)
      {
        LoadRequestedProgram();
      }
      else
      {
        GetNextFile();
      }
      newProgram = !newProgram;
      requestedProgram = 0;
    }
    
    long startTime = millis(); //used to set frame rate wait times
    long loopTime;
    
    //dispose of the header (first two bytes) 
    Serial.print("Playing File: "); Serial.println(myFile.name()); 
    myFile.read();
    myFile.read();
  
    while(myFile.available() && !newProgram) //untill there is no more data
    {
      CheckButtonPress();
      CheckForSerialProgram();
  
      for(int i = 0; i<LEDCOUNT; i++)//itterate each led
      {
      //  strip.setPixelColor(i,GetOneLedDataFromFile(myFile));
        leds[i] = GetOneLedDataFromFile(myFile);
      }
    
    //strip.show();
    FastLED.show();
    
    loopTime = (millis() - startTime);
    //Serial.println(loopTime); //show the loop time in ms
    if(loopTime < 40)
    {
      delay(40 - loopTime); //40 ms in 25 fps
    }
    startTime = millis();
  }
  
  myFile.seek(0); //reset the file
  }
>>>>>>> FETCH_HEAD
}

=======
}

void GetLedCount()
{
  LEDCOUNT = myFile.read();
  LEDCOUNT<<=8;
  LEDCOUNT |= myFile.read(); 
  Serial.print("LED COUNT = "); Serial.println(LEDCOUNT);
  myFile.seek(0); //redet the file to start.
}

void PlayDataToVest()
{
  CRGB leds[LEDCOUNT];
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LEDCOUNT);
  
  //Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDCOUNT, DATA_PIN, NEO_GRB + NEO_KHZ400);
  //strip.begin();
  
  while(true)//replaces loop so led array size can be sized dynamically from file
  {
    if(newProgram)
    {
      if(requestedProgram > 0)
      {
        LoadRequestedProgram();
      }
      else
      {
        GetNextFile();
      }
      newProgram = !newProgram;
      requestedProgram = 0;
    }
    
    long startTime = millis(); //used to set frame rate wait times
    long loopTime;
    
    //dispose of the header (first two bytes) 
    Serial.print("Playing File: "); Serial.println(myFile.name()); 
    myFile.read();
    myFile.read();
  
    while(myFile.available() && !newProgram) //untill there is no more data
    {
      CheckButtonPress();
      CheckForSerialProgram();
  
      for(int i = 0; i<LEDCOUNT; i++)//itterate each led
      {
      //  strip.setPixelColor(i,GetOneLedDataFromFile(myFile));
        leds[i] = GetOneLedDataFromFile(myFile);
      }
    
    //strip.show();
    FastLED.show();
    
    loopTime = (millis() - startTime);
    //Serial.println(loopTime); //show the loop time in ms
    if(loopTime < 40)
    {
      delay(40 - loopTime); //40 ms in 25 fps
    }
    startTime = millis();
  }
  
  myFile.seek(0); //reset the file
  }
}

>>>>>>> FETCH_HEAD
void GetNextFile()
{ 
  myFile =  cardRoot.openNextFile();

  if (!myFile) 
  {
<<<<<<< HEAD
<<<<<<< HEAD
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
  
=======
    cardRoot.rewindDirectory();
    myFile =  cardRoot.openNextFile();
    currentProgram=0;
  }
=======
    cardRoot.rewindDirectory();
    myFile =  cardRoot.openNextFile();
    currentProgram=0;
  }
>>>>>>> FETCH_HEAD
   currentProgram++; //keep track of the program to send to other controllers. 
  
   Serial.print("Opened File:");Serial.println(myFile.name());
   Serial1.print(currentProgram);//send the programChange to the other controllers. 
<<<<<<< HEAD
>>>>>>> FETCH_HEAD
=======
>>>>>>> FETCH_HEAD
}

void LoadRequestedProgram()
{
 //Serial.print("requestedProgram="); Serial.println(requestedProgram);
  currentProgram = 0;
  cardRoot.rewindDirectory();
  for(int i = 0; i<requestedProgram; i++)
  {
    myFile =  cardRoot.openNextFile();
    currentProgram++;
  }
  Serial.print("Opened File:");Serial.println(myFile.name());
}

void InitializeSD()
{
  Serial.print("Initializing SD card...");
   
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("SD Initialization Failed!");
    return;
  }
  
  Serial.println("SD Initialization Started.");
  
  cardRoot = SD.open("/");
  
  DiplayCardInfo();
  
  //count files
  cardRoot.rewindDirectory();
  while(true)
  {
     myFile =  cardRoot.openNextFile();
     if (! myFile) 
     {
       cardRoot.rewindDirectory();
       Serial.print("Counted Files: "); Serial.println(countFiles);
       break;
     }
     else 
     {
       countFiles++;
     }
  }
 
  Serial.println("SD INITILIAZED OK");
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

void printDirectory(File dir, int numTabs) {
   int l = 0;
  while(true)
  {
     myFile =  dir.openNextFile();
     if (! myFile) {
       // no more files
       Serial.println("**nomorefiles**");
       l=1;
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(myFile.name());
     if (myFile.isDirectory()) {
       Serial.println("/");
       printDirectory(myFile, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(myFile.size(), DEC);
     }
     myFile.close();
  }
}

void DiplayCardInfo()
{
  //half speed didnt work so well with teensy 3.1
  //might be able to get the frame rate up higher on Arduino on Full Speed?
  if (!card.init(SPI_QUARTER_SPEED, chipSelect)) {
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
  pinMode(20, OUTPUT);  //to talk to the SD card, 10 on UNO, 53 on mega
  
  pinMode(BUTTON_PIN, INPUT); //button
  digitalWrite(BUTTON_PIN, HIGH);
}

void CheckButtonPress()

{
  if(digitalRead(BUTTON_PIN) == LOW)
  {
    delay(5);//debounce
    if(digitalRead(BUTTON_PIN) == LOW)
    {
      newProgram = true;
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
=======
>>>>>>> FETCH_HEAD
      
      while(digitalRead(BUTTON_PIN) == LOW) {;}//wait for let go of button
>>>>>>> FETCH_HEAD
    }
  }
}

<<<<<<< HEAD
<<<<<<< HEAD
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
=======
=======
>>>>>>> FETCH_HEAD
void CheckForSerialProgram()
{
  if(Serial1.available())
  {
    int recieved;
    while(Serial1.available())
    { 
       recieved = Serial1.read();
       recieved -=48;
       Serial.print("SerialRecieved ");Serial.println(recieved);
    }
    
    if(recieved > 0 && recieved <= countFiles)
    {
      requestedProgram = recieved;
      Serial1.print(requestedProgram);//send on 
      newProgram = true;
    }
  }
}

<<<<<<< HEAD
>>>>>>> FETCH_HEAD
=======
>>>>>>> FETCH_HEAD
