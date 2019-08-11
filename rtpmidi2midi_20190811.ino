// rtpMIDI to serial MIDI converter
// MIDI Thru is OFF
// created by Rinaldo Jonathan <https://rinaldo.id>
// built for Rinaldo's final project at Politeknik Caltex Riau <https://pcr.ac.id>, Diploma 3 Computer Engineer. 

// These need to be included when using standard Ethernet
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <AppleMidi.h>  // lathoub rtpMIDI llibrary <https://github.com/lathoub/Arduino-AppleMIDI-Library/>
#include <MIDI.h>       // FortySeven MIDI library  <https://github.com/FortySevenEffects/arduino_midi_library/>


char ssid[] = "systemv";   //  your network SSID (name)
char pass[] = "gakadapassword";             // your network password (use for WPA, or use as key for WEP)

unsigned long t0 = millis();
bool isConnected = false;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI);  // start rtpMIDI
MIDI_CREATE_DEFAULT_INSTANCE();                 // start serial MIDI







// ====================================================================================
// Event handlers for incoming MIDI messages
// ====================================================================================

// -----------------------------------------------------------------------------
// rtpMIDI session. Device connected
// -----------------------------------------------------------------------------
void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected  = true;
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected  = false;
}

// -----------------------------------------------------------------------------
// rtpMIDI receive Note ON
// -----------------------------------------------------------------------------
void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOn(note, velocity, channel); //send Note ON to serial MIDI
}

// -----------------------------------------------------------------------------
// rtpMIDI receive Note OFF
// -----------------------------------------------------------------------------
void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  MIDI.sendNoteOff(note, velocity, channel); //send Note Off to serial MIDI

}

// -----------------------------------------------------------------------------
// rtpMIDI receive MIDI CC
// -----------------------------------------------------------------------------
void OnAppleMidiCc(byte channel, byte number, byte value){
  MIDI.sendControlChange(number, value, channel); //send Note Off to serial MIDI

}


// rtpMIDI OnAppleMidiReceiveAfterTouchPoly
void OnAppleMidiReceiveAfterTouchPoly(byte channel, byte note, byte pressure)
{ 
  MIDI.sendAfterTouch(note,pressure,channel);
}

// rtpMIDI OnAppleMidiReceiveProgramChange
void OnAppleMidiReceiveProgramChange(byte channel, byte number)
{
  MIDI.sendProgramChange(number,channel);
}

// rtpMIDI OnAppleMidiReceiveAfterTouchChannel

void OnAppleMidiReceiveAfterTouchChannel(byte channel, byte pressure)
{
  MIDI.sendAfterTouch(pressure,channel);
}

// rtpMIDI OnAppleMidiReceivePitchBend
void OnAppleMidiReceivePitchBend(byte channel, int bend)
{
  MIDI.sendPitchBend(bend, channel);
}

// rtpMIDI OnAppleMidiReceiveTimeCodeQuarterFrame
void OnAppleMidiReceiveTimeCodeQuarterFrame(byte data)
{
  MIDI.sendTimeCodeQuarterFrame(data);
}



// -----------------------------------------------------------------------------
// serial MIDI receive Note ON
// -----------------------------------------------------------------------------
void OnSerialMidiNoteOn(byte channel, byte note, byte velocity)
{
  AppleMIDI.sendNoteOn(note, velocity, channel); //send Note On to rtpMIDI
}

// -----------------------------------------------------------------------------
// serial MIDI receive Note OFF
// -----------------------------------------------------------------------------
void OnSerialMidiNoteOff(byte channel, byte note, byte velocity)
{
  AppleMIDI.sendNoteOff(note, velocity, channel); //send Note OFF to rtpMIDI
}


// -----------------------------------------------------------------------------
// serial MIDI CC
// -----------------------------------------------------------------------------
void OnSerialMidiCC (byte channel, byte number, byte value)
{
  AppleMIDI.sendControlChange(number, value, channel); //send Note OFF to rtpMIDI
}



// OnSerialMidiAfterTouchPoly
void OnSerialMidiAfterTouchPoly(byte channel, byte note, byte pressure)
{
  AppleMIDI.sendPolyPressure(note, pressure, channel);
}

void OnSerialMidiProgramChange(byte channel, byte number)
{
  AppleMIDI.sendProgramChange(number,channel);
}

void OnSerialMidiAfterTouchChannel (byte channel, byte pressure)
{
  AppleMIDI.sendAfterTouch(pressure,channel); 
}

void OnSerialMidiPitchBend (byte channel, int bend)
{
  AppleMIDI.sendPitchBend(bend, channel);
}

void OnSerialMidiTimeCodeQuarterFrame (byte data)
{
  AppleMIDI.sendTimeCodeQuarterFrame(data); 
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  // Serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.print(F("Getting IP address..."));

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  
  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.print(WiFi.localIP());
  Serial.println(F(":5004")); 
  Serial.println();
   Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());

  delay(5000);
  Serial.begin(31250);
  // Create a session and wait for a remote host to connect to us
  AppleMIDI.begin("test2");                            // rtpMIDI session name

  AppleMIDI.OnConnected(OnAppleMidiConnected);        // unused, leftover codes from examples
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);  // unused, leftover codes from examples

  AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);       // handle rtpMIDI Note ON
  AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);     // handle rtpMIDI Note OFF
  AppleMIDI.OnReceiveControlChange(OnAppleMidiCc);    // handle rtpMIDI CC

  //handle everything else that I don't fully understand
  AppleMIDI.OnReceiveAfterTouchPoly(OnAppleMidiReceiveAfterTouchPoly);
  AppleMIDI.OnReceiveProgramChange(OnAppleMidiReceiveProgramChange);
  AppleMIDI.OnReceiveAfterTouchChannel(OnAppleMidiReceiveAfterTouchChannel);
  AppleMIDI.OnReceivePitchBend(OnAppleMidiReceivePitchBend);
  //AppleMIDI.OnReceiveSysEx(OnAppleMidiReceiveSysEx); //skipped
  AppleMIDI.OnReceiveTimeCodeQuarterFrame(OnAppleMidiReceiveTimeCodeQuarterFrame);
  //AppleMIDI.OnReceiveSongPosition(OnAppleMidiReceiveSongPosition);
  //AppleMIDI.OnReceiveSongSelect(OnAppleMidiReceiveSongSelect);
  //AppleMIDI.OnReceiveTuneRequest(OnAppleMidiReceiveTuneRequest);
  //AppleMIDI.OnReceiveClock(OnAppleMidiReceiveClock);
  //AppleMIDI.OnReceiveStart(OnAppleMidiReceiveStart);
  //AppleMIDI.OnReceiveContinue(OnAppleMidiReceiveContinue);
  //AppleMIDI.OnReceiveStop(OnAppleMidiReceiveStop);
  //AppleMIDI.OnReceiveActiveSensing(OnAppleMidiReceiveActiveSensing);
  //AppleMIDI.OnReceiveReset(OnAppleMidiReceiveReset);

  MIDI.begin(MIDI_CHANNEL_OMNI);                      // start serial MIDI
  MIDI.turnThruOff();                                 // because we don't need one

  MIDI.setHandleNoteOn(OnSerialMidiNoteOn);           // handle serial MIDI note ON, Put only the name of the function, 
  MIDI.setHandleNoteOff(OnSerialMidiNoteOff);         // handle serial MIDI note OFF
  MIDI.setHandleControlChange(OnSerialMidiCC);        // handle serial MIDI CC

  //handle everything else that I don't fully understand
  MIDI.setHandleAfterTouchPoly(OnSerialMidiAfterTouchPoly);
  MIDI.setHandleProgramChange(OnSerialMidiProgramChange);
  MIDI.setHandleAfterTouchChannel(OnSerialMidiAfterTouchChannel);
  MIDI.setHandlePitchBend(OnSerialMidiPitchBend);
  //MIDI.setHandleSystemExclusive(OnSerialMidiSysEx); //skipped
  MIDI.setHandleTimeCodeQuarterFrame(OnSerialMidiTimeCodeQuarterFrame);
  //MIDI.setHandleSongPosition(OnSerialMidiSongPosition); //skipped
  //MIDI.setHandleSongSelect(OnSerialMidiSongSelect); //skipped
  //MIDI.setHandleTuneRequest(OnSerialMidiTuneRequest); //skipped
  //MIDI.setHandleClock(OnSerialMidiClock); //skipped
  //MIDI.setHandleStart(OnSerialMidiStart); //skipped 
  //MIDI.setHandleContinue(OnSerialMidiContinue); //skipped
  //MIDI.setHandleStop(OnSerialMidiStop); //skipped
  //MIDI.setHandleActiveSensing(OnSerialMidiActiveSensing); /skipped
  //MIDI.setHandleSystemReset(OnSerialMidiReset); //skipped


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  // Listen to incoming notes
  AppleMIDI.read();
  MIDI.read();

}


