/*
 Receive Voice Call

 This sketch, for the Arduino GSM shield, receives voice calls,
 displays the calling number, waits a few seconds then hangs up.

 Circuit:
 * GSM shield
 * Voice circuit. Refer to to the GSM shield getting started guide
   at http://arduino.cc/en/Guide/ArduinoGSMShield#toc11
 * SIM card that can accept voice calls
 
  ATTENTION: to work correctly with M0/M0 pro you have to connect pin 2 to pin 4 of the shield

 With no voice circuit the call will connect, but will not send or receive sound

 created Mar 2012
 by Javier Zorzano

 This example is in the public domain.

 
 Added Arduino GSM Shield 2 features
 modified Feb 2016
 by Arduino.org team (http://arduino.org) 
 
 */

// Include the GSM library
#include <GSM.h>

// PIN Number
#define PINNUMBER ""

// initialize the library instance
GSM gsmAccess;
GSM2 gsmAccessV2;
GSMVoiceCall vcs;

#define number_len 20    // define incoming call number lenght

// Array to hold the number for the incoming call
char number[number_len];

void setup()
{
  // initialize serial communications and wait for port to open:
  SerialUSB.begin(9600);
  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  SerialUSB.println("Receive Voice Call");

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected)
  {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY)
      notConnected = false;
    else
    {
      SerialUSB.println("Not connected");
      delay(1000);
    }
  }

  // This makes sure the modem correctly reports incoming events
  vcs.hangCall();
  gsmAccessV2.muteControl(0);  //not allowed by the module
  gsmAccessV2.CommandEcho(1);  // set echo
  gsmAccessV2.speakerMode(1);  //set speaker mode
  gsmAccessV2.speakerLoudness(3); //set speaker loudness
  gsmAccessV2.swapAudioChannel(1); //set audio channel
 gsmAccessV2.microphoneGainLevel(1,13); //set microphone gain level
 gsmAccessV2.ringerSoundLevel(20);  //set ring sound level
 gsmAccessV2.alertSoundMode(0);   //set alert sound mode (sound)
 gsmAccessV2.loudSpeakerVolumeLevel(20); //set loudspeaker volume level
 
  SerialUSB.println("Waiting for a call");
}

void loop()
{
  // Check the status of the voice call
  switch (vcs.getvoiceCallStatus())
  {
    case IDLE_CALL: // Nothing is happening

      break;

    case RECEIVINGCALL: // Yes! Someone is calling us
      
      SerialUSB.println("RECEIVING CALL");
      
      
      // Retrieve the calling number
      vcs.retrieveCallingNumber(number, number_len);

      // Print the calling number
      SerialUSB.print("Number:");
      SerialUSB.println(number);
      SerialUSB.println("Press a to answer");
      while(SerialUSB.read() !='a');
      // Answer the call, establish the call
      vcs.answerCall();
      break;

    case TALKING:  // In this case the call would be established

      SerialUSB.println("TALKING. Press h to hang up.");
      while (SerialUSB.read() != 'h')
        delay(100);
      vcs.hangCall();
      SerialUSB.println("Hanging up and waiting for the next call.");
      break;
  }
  delay(1000);
}


