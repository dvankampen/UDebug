/*
  UDebug.cpp - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "UDebug.h"

// Initialize Class Variables //////////////////////////////////////////////////


// Local constants  ////////////////////////////////////////////////////////////
char *prologue = "ARDUBUGGER|";
byte myIP[] = {192,168,1,8};
EthernetUDP dbg;
char buffer[UDP_TX_PACKET_MAX_SIZE];

// Constructors ////////////////////////////////////////////////////////////////

UDebug::UDebug(uint16_t port)
{
	_port = port;
}

UDebug::UDebug(byte IP[], uint16_t port)
{
	memcpy(myIP, IP, sizeof(IP));
	_port = port;
}

// Private Methods //////////////////////////////////////////////////////////////

void UDebug::writePrologue(uint8_t type)
{
	dbg.beginPacket(myIP, _port);
	for (int i = 0; i < strlen(prologue); i++) {
		dbg.write((uint8_t)prologue[i]);
	}
	dbg.write(type);
}

void UDebug::writeInt(int variable)
{
	writePipe();
	dbg.write(variable);
}

void UDebug::writePipe()
{
	dbg.write((uint8_t)'|');
}

void UDebug::waitOnResponse()
{
	int size = dbg.parsePacket();
	while (!size) {
		delay(10);
		size = dbg.parsePacket();
		dbg.flush();
	}
	// if there's data available, read a packet
	if(size) {
		dbg.read(buffer, size);
		dbg.flush();
	}
}

void UDebug::waitOnResponse(int *newVariable)
{
	int size = dbg.parsePacket();
	while (!size) {
		delay(10);
		size = dbg.parsePacket();
		dbg.flush();
	}
	// if there's data available, read a packet
	if(size) {
		dbg.read(buffer, size);
		itoa(*newVariable, buffer,10);
		dbg.flush();
	}
}

// Public Methods //////////////////////////////////////////////////////////////

void UDebug::begin(void)
{
	if (!dbg.begin(_port)) {
		Serial.println("Could not start debugger!");
	}
	pinMode(31, OUTPUT);
}

void UDebug::log(char* message, uint8_t severity)
{
	uint8_t sev = severity;
	writePrologue(8);
	writePipe();
    dbg.write((uint8_t *)message, strlen(message));
	writePipe();
	if (sev > 10) {
		sev = 10;
	}
	dbg.write(sev);
    dbg.endPacket();
}

void UDebug::breakpoint()
{
	long loopCount = 0;
	writePrologue(0);
	dbg.endPacket();
	delay(1000);
	waitOnResponse();
}

void UDebug::breakpoint(int variable)
{
	long loopCount = 0;
	writePrologue(1);
	writeInt(variable);
	dbg.endPacket();
	delay(1000);
	waitOnResponse();
}

void UDebug::breakpoint(int* variable)
{
	long loopCount = 0;
	writePrologue(1);
	writeInt(*variable);
	dbg.endPacket();
	delay(1000);
	waitOnResponse(variable);
}

// Preinstantiate Objects //////////////////////////////////////////////////////



