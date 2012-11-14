/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2012 Dave VanKampen.  All right reserved.

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

*/

#ifndef UDebug_h
#define UDebug_h

#include <inttypes.h>
#include <EthernetUDP.h>
#include <Arduino.h> //for the byte typedef

class UDebug
{
  private:
	uint16_t _port;
	
	void writePrologue(uint8_t type);
	void writeInt(int variable);
	void writePipe();
	void waitOnResponse();
	void waitOnResponse(int *newVariable);
	  
  public:
	UDebug(uint16_t);	
	UDebug(byte[], uint16_t);
    void begin();
	void log (char*, uint8_t);
	void breakpoint();
	void breakpoint(int variable);
	void breakpoint(int* variable);//editable variable
	
};

#endif

