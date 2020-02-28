// Arduino music player controlled via web GUI
//
// Created by combining the following examples:
// * "backSoon" example by <jc@wippler.nl>
//   https://github.com/njh/EtherCard/blob/master/examples/backSoon/backSoon.ino
// * "Jingle Bells" example by Emmanuel
//   http://repairmypc.net/2017/08/jingle-bells-arduino-sketch/
// * "SSDP.ino" by Nicholas Humfrey
//   https://github.com/njh/EtherCard/blob/master/examples/SSDP/SSDP.ino
//
// License: GPLv2

#include <EtherCard.h>

#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,1,200 };
// gateway ip address
static byte gwip[] = { 192,168,1,1 };
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

const char blankReply[] PROGMEM =
"HTTP/1.0 200 Found\r\n"
"Content-Type: text/html\r\n"
"\r\n";

const char page[] PROGMEM =
"HTTP/1.0 200 Found\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Welcome to Arduino web server"
  "</title>"
  "<style> * { font-size:3rem; margin:.2em;}</style>"
  "</head>"
  "<body>"
    "<button onClick=\"fetch('/on',{method: 'GET'})\">PLAY</button><br/>"
    "<button onClick=\"fetch('/off',{method: 'GET'})\">PAUSE</button>"
  "</body>"
"</html>";

void setup(){
  Serial.begin(9600);
  Serial.println("\n[setup started]");
  setupPlayer();

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");
  #if STATIC
    ether.staticSetup(myip, gwip);
    Serial.println("using static IP");
  #else
    Serial.println("using DHCP");
  
    if (!ether.dhcpSetup())
      Serial.println("DHCP failed");
  #endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
}

int isOn = 0;

void loop(){
  // wait for an incoming TCP packet and get it's contents
  word pos = ether.packetLoop(ether.packetReceive());

  // check if valid tcp data is received
  if (pos) {

    Serial.println("processing request");

    char* data = (char *) Ethernet::buffer + pos;

    if (strncmp("GET /on", data, 7) == 0) {
      isOn = 1;
      Serial.println("play started");
      sendBlankResponse();
    } else if (strncmp("GET /off", data, 8) == 0) {
      isOn = 0;
      Serial.println("play paused");
      sendBlankResponse();
    } else {
      Serial.println("serving default route");
      // default ruta: vraćam HTML stranicu
      memcpy_P(ether.tcpOffset(), page, sizeof page);
      ether.httpServerReply(sizeof page - 1);
    }
  }

  if(isOn==1) {
    playerLoop();
  }
}

void sendBlankResponse() {
//    ether.httpServerReplyAck();
    memcpy_P(ether.tcpOffset(), blankReply, sizeof blankReply);
    ether.httpServerReply(sizeof blankReply - 1);
}

// --------------------------------------------------------------------------
// The followinf block was taken from "Jingle Bells" example by Emmanuel
//   http://repairmypc.net/2017/08/jingle-bells-arduino-sketch/
// --------------------------------------------------------------------------
int speakerPin = 9;
int length = 26;
char notes[] = "eeeeeeegcde fffffeeeeddedg";
int beats[] = { 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};

int tempo = 200;

void setupPlayer() {
  pinMode(speakerPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

int playerPosition = 0;

void playerLoop() {

  Serial.println(playerPosition);

  if (notes[playerPosition] == ' ') {
    delay(beats[playerPosition] * tempo); // rest
  } else {
    playNote(notes[playerPosition], beats[playerPosition] * tempo);
  }
  
  // pause between notes
  delay(tempo / 2); 

  if(playerPosition++ >= length) {
    playerPosition = 0;
  }
}
