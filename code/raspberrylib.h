// *******************************
// FILE: 		raspberrylib.h
// AUTHOR:		SharpCoder
// DATE:		2012-03-18
// ABOUT:		This is the header file for my C++ raspberry pi library
//				which includes all the code and methods necessary
//				to interface with the raspberry pi at the kernel level.
//
// LICENSE:		Provided "AS IS". USE AT YOUR OWN RISK.
// *******************************
#ifndef __RASPBERRY_LIB_H_
#define __RASPBERRY_LIB_H_

// Include the common header to support my data types.
#include "common.h"

// Define some locations in memory specific to the raspberry pi.
#define		ARM_BUS_LOCATION		(unsigned int) 0x40000000
#define		ARM_GPIO_BASE_ADDR		(unsigned int) 0x20200000
#define		ARM_PERIPHERAL_BASE		(unsigned int) 0x20000000
#define		ARM_COUNTER_ADDR		(unsigned int) 0x20003004
#define		ARM_MAIL_BASE			(unsigned int) 0x2000B880
#define		MAIL_FULL				0x80000000
#define		MAIL_EMPTY				0x40000000
#define		ARM_MAIL_READ			0x00
#define		ARM_MAIL_WRITE			0x20
#define		ARM_MAIL_STATUS			0x18

// Define the headers within this library.
namespace RaspberryLib {
	
	// This is meant to encapsulate memory mapping
	// methods, specifically important ones!
	namespace Memory {
		// In the raspberry pi, you MUST translate
		// certain requests between BUS and PHYSICAL
		// locations in memory, otherwise your attempts
		// at, for example, acquiring a framebuffer pointer
		// will be INVALID.
		static uint32 PHYSICAL_TO_BUS( uint32 p ) {
			return (uint32)(p + ARM_BUS_LOCATION );
		}
		
		// And in those special cases, the RPI
		// will return a value that ALSO needs
		// to be 're-mapped'.
		static uint32 BUS_TO_PHYSICAL( uint32 p ) {
			if ( p > ARM_BUS_LOCATION ) 
				return (uint32)(p - ARM_BUS_LOCATION );
			return (uint32)p;
		}
		
		// Memory barrier is an ASSEMBLY function I have
		// which syncronizes memory, I guess.
		static void Barrier( void ) {
			asm volatile( "mcr p15, 0, ip, c7, c5, 0" );
			asm volatile( "mcr p15, 0, ip, c7, c5, 6" );
			asm volatile( "mcr p15, 0, ip, c7, c10, 4" );
			asm volatile( "mcr p15, 0, ip, c7, c5, 4" );
		}
	};
	
	typedef struct {
		uint32 framePtr;
		uint32 pitch;
		uint32 width;
		uint32 height;
		bool valid;
	}  GPU;

	// Assembly code based methods (or they used to be).
	// This is for hard core integral stuff.
	static uint32 GET32( uint32 addr );
	static void PUT32( uint32 addr, uint32 value );
	static char GET4( uint32 addr );
	static void PUT4( uint32 addr, char value );
	
	// Common RPI interaction methods.
	uint32 CheckCounter( void );
	void SetGPIO( uint32 pin, uint32 value );
	void Wait( uint32 time );
	void Blink( uint32 count, uint32 time );
	void PiFault( const char* msg );
	
	// Common Advanced RPI methods.
	static uint32 MailboxCheck( char channel );
	static void MailboxWrite( char channel, uint32 value );
	
	// Extreme RPI methods!
	GPU AcquireFrameBuffer( uint32 xres, uint32 yres );
}

#endif
