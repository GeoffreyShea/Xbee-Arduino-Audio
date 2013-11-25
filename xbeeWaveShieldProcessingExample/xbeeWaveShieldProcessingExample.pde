/* This sketch sends info about audio file names via an xbee coordinator on your computer to a
   corresponding Arduino sketch that receives the info and plays the files with a WaveShield from
   Adafruit. The xbees are all Series 2 and are setup in API mode. It uses XBee API by Andrew Rapp
   on the host computer and XBee Arduino on the players
   
   by Geoffrey Shea
*/

import processing.serial.*;

// the XBee API is installed in your sketch's Code folder

XBeeAddress64[] nodes = {
  new XBeeAddress64(0, 0x13, 0xa2, 0, 0x40, 0x8c, 0xc8, 0x3e), // node 0 // the first 4 hex digits are
  new XBeeAddress64(0, 0x13, 0xa2, 0, 0x40, 0x8c, 0xc8, 0x48), // node 1 // common to all arduinos
  new XBeeAddress64(0, 0x13, 0xa2, 0, 0x40, 0x8c, 0xc8, 0x46), // node 2
  new XBeeAddress64(0, 0x13, 0xa2, 0, 0x40, 0x8b, 0x18, 0xbb)  // node 3
  };
  
XBee xbee = new XBee();

int delayDuration = 6000;               // time between each play, i.e. 6 seconds
int node = 0;                           // to specify one of the nodes above
int fileNumber;                         // to specify which file is to be played
long oldTime = millis();                // use in calculating time elapsed
int[] array0 = { 22, 33, 44, 11 };      // the names of the files to play on node 0
int[] array1 = { 22, 33, 44, 11 };      // etc.
int[] array2 = { 22, 33, 44, 11 };      // these will have ".WAV" appended by arduino
int[] array3 = { 22, 33, 44, 11 };      // so they represent 22.WAV, etc.
int plays = 0;

void setup() {
  size(400, 800);
  try {                                 // the try/catch form is required by the XBee API library
    xbee.open("/dev/tty.usbserial-FTFXIB4A", 9600);  // change the serial port to match yours
  }
  catch (Exception e) {
    e.printStackTrace();
  }
}

void mouseReleased() {                  // this is just to change the delayDurationon on the fly. Optional
  if (mouseX > 50 && mouseX < 100 && mouseY > 50 && mouseY < 100) {
    delayDuration+=200;
  }
  if (mouseX > 50 && mouseX < 100 && mouseY > 600 && mouseY < 650) {
    delayDuration-=200;
  }
}

void draw() {
  background(250);
  fill(0);
  rect(50, 50, 50, 50);
  rect(50, 600, 50, 50);
  textAlign(CENTER);
  text("delayDuration = " + delayDuration, width/2, height/2);

  if (millis() - oldTime >= delayDuration) {
    if (node == 0) {
      fileNumber = array0[plays];
      play(node, fileNumber);
      node++;
    } 
    else if (node == 1) {                 // this section goes through each node and plays the
      fileNumber = array1[plays];         // first fileNumber from its array, then the second for
      play(node, fileNumber);             // each, then resets and starts over
      node++;
    } 
    else if (node == 2) {
      fileNumber = array2[plays];
      play(node, fileNumber);
      node++;
    } 
    else if (node == 3) {
      fileNumber = array2[plays];
      play(node, fileNumber);
      node = 0;
      if (plays < array1.length - 1){
        plays++;
      } else {
        plays = 0;
      }
    } 
    oldTime = millis();
  }
}

void play(int node, int file) {
  println("plays = " + plays + " / node = " + node + " / fileNumber = " + fileNumber + " (Omits leading zero)");
  try {
    // the payload is the info we will send. The floor and modulo convert the int (say, 22) to an int array of 2 and 2.
    // add 48 to convert numbers to ASCII
    int[] payload = new int[] { floor(file/10) + 48, file%10 + 48 }; 
    ZNetTxRequest request = new ZNetTxRequest(nodes[node], payload); 
    xbee.sendAsynchronous(request);
  }
  catch (Exception e) {
    e.printStackTrace();
  }
}
