/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                         EX_SPI_SLAVE.C                          ////
////                                                                 ////
////  Uses the PIC's SPI peripheral in slave mode to emulate a 9356  ////
////  SPI EEPROM.  This example is the counterpart to EX_SPI.C.      ////
////  Run EX_SPI_SLAVE.C on one PIC and connect it to another PIC    ////
////  running EX_SPI.                                                ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.  The    ////
////  following conditional compilation lines are used to include a  ////
////  valid device for each compiler.  Change the device, clock and  ////
////  RS232 pins for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2006 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F877.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#elif defined(__PCH__)
#include <18F452.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=8MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)
#endif

int8 memory[80], data, instr, address;

/*
void write_data(void)
PURPOSE: Reads data from SPI and writes it to memory
PARAMS: none
RETURNS: None
*/
void write_data(void)
{
   while(!spi_data_is_in());
   
   data = spi_read();
   
   if(address >= 0 || address <= 80)
   {
      memory[address] = data;
   }
}


/*
void main(void)
PURPOSE: Peripheral Initialization
*/
void main(void)
{
   setup_spi(spi_slave | spi_h_to_l | spi_ss_disabled);

   while(true)
   {
      while(!spi_data_is_in());
      instr = spi_read();
      
      while(!spi_data_is_in());
      address = spi_read();

      if (instr == 0x18)
      {
         instr = spi_read(memory[address]);
      }
      else if(instr == 0xa)
      {
         write_data();
      } 
   }
}
