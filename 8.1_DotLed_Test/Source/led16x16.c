/*********************************************************************************************
* File:		Dotled.c
* Author:	embest
* Desc:		DotLed_Test 
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "2410lib.h"
#include "fonts.h"
#include "led16x16.h"

/*------------------------------------------------------------------------------------------*/
/*	 								function declare					 				    */
/*------------------------------------------------------------------------------------------*/
extern void led_char_disp(void);

/*------------------------------------------------------------------------------------------*/
/*	 								global variables						 				*/
/*------------------------------------------------------------------------------------------*/
u8 l_display_array[2*16];
u8 assic_buffer[3*16];
/*============================================================================================
l_display_array:
+-----+-----+
|     |     |     
|  D  |  E  |  
|     |     |     
+-----+-----+
A buffer data and B buffer data -->D buffer data
B buffer data and C buffer data -->E buffer data

assic_buffer:
+-----+-----+-----+
|     |     |     |
|  A  |  B  |  C  |<---- update the C buffer and move the B buffer data to the A buffer
|     |     |     |   and move the C buffer data to the B buffer data
+-----+-----+-----+

============================================================================================*/

/*********************************************************************************************
* name:		led_update
* func:		refresh the led display
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void led_update(void)
{
	int j = 20;
	while(j--)
	{
		led_char_disp();	
	}	
}

/*********************************************************************************************
* name:		l_display_scroll
* func:		shift the display
* para:		bits:the position str:point the buffer
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void refresh_l_display_array(u8 bits, u8 *str)
{
	u32 i;
	u32 remaining_bits = 8-bits; 
	
	for(i=0;i<16;i++)
	{
		l_display_array[2*i] = (*str<<bits) |(*(str+16)>>remaining_bits);
		l_display_array[2*(i+1)-1] = (*(str+16)<<bits) |(*(str+32)>>remaining_bits);
		str++;
	}
}

/*********************************************************************************************
* name:		l_display_scroll
* func:		scroll the display
* para:		str:point the buffer
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void l_display_scroll ( u8 *str )
{
	int i;
	for(i=0;i<8;i++)
	{
		refresh_l_display_array(i, str);
		led_update();
	}	
}

/*********************************************************************************************
* name:		copy_data
* func:		copy data
* para:		dst:point the dest data src:points the source data
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void copy_data(u8 *dst, u8 *src, u32 size)
{
	while(size--)
		*(dst++) = *(src++);	
}

/*********************************************************************************************
* name:		refresh_assic_buffer
* func:		refresh buffer
* para:		str:points the new char
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void refresh_assic_buffer(u8 *str)
{
	copy_data(&assic_buffer[0], &assic_buffer[16],16);
	copy_data(&assic_buffer[16], &assic_buffer[32],16);
	copy_data(&assic_buffer[32], str,16);
	l_display_scroll(assic_buffer);
}

/*********************************************************************************************
* name:		char_out
* func:		display the chars
* para:		font:0  str:points of the chars
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void char_out(u8 font, u8 *str)
{
	u8 *str_ptr;
	u8 glyph;
	
	glyph = ( u8 )*str;
	
	while(glyph != '\0' )
	{
		str_ptr = fonts[font].ascii_code + ( glyph - fonts[font].ascii_beg) * fonts[font].ascii_height;
		refresh_assic_buffer(str_ptr);
		str++;
		glyph = ( u8 )*str;
	}
}


/*********************************************************************************************
* name:		led_init
* func:		initial the led display
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led_init(void)
{
	rGPBCON = rGPBCON & 0xfff0ff | 0x500;     		// GPB4,GPB5=01:Output
	rGPCCON = rGPCCON & 0xffff3ffc | 0x4001;     	// GPC0,GPC7=01:Output
	rGPDCON = rGPDCON & 0xffcfffff | 0x100000;     	// GPD10=01:Output
	rGPGCON = rGPGCON & 0xfffcffff | 0x10000;     	// GPG8=01:Output
}
