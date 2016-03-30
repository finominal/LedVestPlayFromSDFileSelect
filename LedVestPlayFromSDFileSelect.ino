/*
The circuit:
  * SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module. 
 
 CHANGELOG:
 Changed the data pin for Octows2811 adaptor. Blue pair Left.
 Button on pin 17. 
 
 */

//#include <Adafruit_NeoPixel.h>
#include "FastLED.h"
#include <SD.h>



//File entry =  dir.openNextFile();

const int DATA_PIN = 14;
uint16_t  LEDCOUNT = 0;
const int BUTTON_PIN = 17;
const int REMOTE_PIN = 18;
bool AUTOMODE = false;

//SD Card Control
File myFile;
File cardRoot;

const int chipSelect = 10;
boolean newProgram = false;
int requestedProgram = 0;
int countFiles = 0;
int currentProgram;

//SD Card Info
Sd2Card card;
SdVolume volume;
SdFile root;

void setup()
{
  delay(2000); //dev catch startup info 
  Serial.begin(115200);
  //Serial1.begin(9600);
  
  Serial.println("*Starting*");

  InitializePins();
 
  
  CheckForAutoModeOnStart();
   
  InitializeSD(); // will also display card info
  GetNextFile();  // get the first file
  GetLedCount(); //get the led count from the file header, two bytes combined.

 
}

void loop()
{
  PlayDataToVest();
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
      CheckAutoMode();
  
      for(int i = 0; i<LEDCOUNT; i++)//itterate each led
      {
      //  strip.setPixelColor(i,GetOneLedDataFromFile(myFile));
        leds[i] = GetOneLedDataFromFile(myFile);
      }
    
    //strip.show();
    FastLED.show();
    
    loopTime = (millis() - startTime);
    //Serial.println(loopTime); //show the loop time in ms
    if(loopTime < 33)
    {
      delay(33 - loopTime); //40 ms in 25 fps
    }
    startTime = millis();
  }
  
  myFile.seek(0); //reset the file
  }
}

void GetNextFile()
{ 
  myFile =  cardRoot.openNextFile();

  if (!myFile) 
  {
    cardRoot.rewindDirectory();
    myFile =  cardRoot.openNextFile();
    currentProgram=0;
  }
   currentProgram++; //keep track of the program to send to other controllers. 
  
   Serial.print("Opened File:");Serial.println(myFile.name());
   //Serial1.print(currentProgram);//send the programChange to the other controllers. 
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

  while(true)
  {
     myFile =  dir.openNextFile();
     if (! myFile) {
       // no more files
       Serial.println("**nomorefiles**");
     
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

  pinMode(REMOTE_PIN, INPUT); //button
  digitalWrite(REMOTE_PIN, LOW);

  delay(20);
}

void CheckButtonPress()

{
  if(digitalRead(BUTTON_PIN) == LOW || digitalRead(REMOTE_PIN) == HIGH)
  {
    delay(5);//debounce
    if(digitalRead(BUTTON_PIN) == LOW || digitalRead(REMOTE_PIN) == HIGH)
    {
      newProgram = true;
      AUTOMODE = false; //override the auto mode by a button press
      while(digitalRead(BUTTON_PIN) == LOW || digitalRead(REMOTE_PIN) == HIGH) {;}//wait for let go of button
    }
  }
}

void CheckForSerialProgram()
{
  if(Serial.available())
  {
    int recieved;
    while(Serial.available())
    { 
       recieved = Serial.read();
       recieved -=48;
       Serial.print("SerialRecieved ");Serial.println(recieved);
    }
    
    if(recieved > 0 && recieved <= countFiles)
    {
      requestedProgram = recieved;
      Serial.print(requestedProgram);//send on 
      newProgram = true;
    }
  }
}

