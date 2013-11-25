/*
Play a soundfile by name (based on an int array sent by xbee)
 */

/* This sketch receives info about audio file names from an xbee coordinator on your computer
   sent to this Arduino which plays the files with a WaveShield from Adafruit.
   The xbees are all Series 2 and are setup in API mode. (Series 1 uses different commands.)
   This uses XBee Arduino by Andrew Rapp
   
   by Geoffrey Shea
*/

#include <WaveHC.h>                // the WaveShield libraries, including SD card reader
#include <WaveUtil.h>              // in your Arduio Libraries folder
#include <XBee.h>                  // the XBee Arduino library

SdReader card;                     // This object holds the information for the card
FatVolume vol;                     // This holds the information for the partition on the card
FatReader root;                    // This holds the information for the volumes root directory
FatReader file;                    // This object represent the WAV file 
WaveHC wave;                       // This is the only wave (audio) object, since we will only play one at a time

char fileName[] = "xx.WAV";        // This is the form of the filename to play. It assumes a 2 digit name, e.g. 33.WAV
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

void setup() {
  card.init();
  card.partialBlockRead(true);
  vol.init(card);
  root.openRoot(vol);
  xbee.begin(9600);
}

void loop() {
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {  // this is a standard test
      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);
      // play file specified in payload
      fileName[0] = rx.getData(0);   // change the first and second character of the file name, i.e. from "xx"
      fileName[1] = rx.getData(1);   // to whatever two characters you have sent from the Processing sketch
      playFile();
    } 
  }
}

void playFile() {
  if (wave.isplaying) {
    wave.stop();
  }
  file.open(root, fileName);          // use your newly contructed file name here
  wave.create(file);
  wave.play();
}
