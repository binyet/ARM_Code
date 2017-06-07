/*********************************************************************************************
* File£º	lcdapp.c
* Author:	Embest	
* Desc£º	LCD_Test
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include <string.h>
#include "2410lib.h"
#include "lcdlib.h"
#include "glib.h"
#include "lcd.h"
#include <math.h>

/*------------------------------------------------------------------------------------------*/
/*	 								constant define							 			    */
/*------------------------------------------------------------------------------------------*/
#define LCD_BUF_SIZE		(SCR_XSIZE_TFT_640480*SCR_YSIZE_TFT_640480/2)
#define LCD_ACTIVE_BUFFER	(0x33800000)
#define LCD_VIRTUAL_BUFFER  (0x33800000 + LCD_BUF_SIZE)
#define LCD_D_OFF 			rGPCDAT &= ~(1 << 4); 
#define LCD_D_ON 			rGPCDAT |= (1 << 4); 

/*------------------------------------------------------------------------------------------*/
/*	 								extern variables						 			    */
/*------------------------------------------------------------------------------------------*/
#ifdef BOARDTEST_EXH
#undef BOARDTEST
#endif	

#ifndef BOARDTEST	
extern const UINT8T g_ucBitmap[][76800];
extern UINT8T g_ucAscii8x16[];
extern UINT8T g_auc_Ascii6x8[];
extern UINT8T g_auc_hzk24[];
#endif
 
/*------------------------------------------------------------------------------------------*/
/*	 								function declare                                        */
/*------------------------------------------------------------------------------------------*/
void lcd_clr(void);
extern void Lcd_port_init(void);
extern void Lcd_Port_Return(void);

/*********************************************************************************************
* name:		lcd_init_app
* func:		lcd application initialization code 
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init_app()
{
    Lcd_port_init();
    Lcd_Init(MODE_TFT_16BIT_800480);
    Glib_Init(MODE_TFT_16BIT_800480);
    Glib_ClearScr(0, MODE_TFT_16BIT_800480);
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);	  
    uart_printf(" [TFT 64K COLOR(16bit/1pixel) LCD TEST]\n");

}

/*********************************************************************************************
* name:		lcd_init_app_end
* func:		end lcd application initialization
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init_app_end()
{
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}



/*********************************************************************************************
* name:		Lcd_Draw_Line()
* func:		Draw line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1,usY1 -- line's end point coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_Line(INT16T usX0, INT16T usY0, INT16T usX1, INT16T usY1, UINT16T ucColor, UINT16T usWidth)
{
	INT16T usDx;
	INT16T usDy;
	INT16T y_sign;
	INT16T x_sign;
	INT16T decision;
	INT16T wCurx, wCury, wNextx, wNexty, wpy, wpx;

	usDx = abs(usX0 - usX1);
	usDy = abs(usY0 - usY1);
	if( ((usDx >= usDy && (usX0 > usX1)) ||
        ((usDy > usDx) && (usY0 > usY1))) )
    {
        GUISWAP(usX1, usX0);
        GUISWAP(usY1, usY0);
    }
    y_sign = (usY1 - usY0) / usDy;
    x_sign = (usX1 - usX0) / usDx;

    if( usDx >= usDy )
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDx >> 1);
             wCurx <= wNextx; wCurx++, wNextx--, decision += usDy )
        {
            if( decision >= usDx )
            {
                decision -= usDx;
                wCury += y_sign;
                wNexty -= y_sign;
            }
            for( wpy = wCury - usWidth / 2;
                 wpy <= wCury + usWidth / 2; wpy++ )
            {
                PutPixel(wCurx, wpy, ucColor);
            }

            for( wpy = wNexty - usWidth / 2;
                 wpy <= wNexty + usWidth / 2; wpy++ )
            {
                PutPixel(wNextx, wpy, ucColor);
            }
        }
    }
    else
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDy >> 1);
             wCury <= wNexty; wCury++, wNexty--, decision += usDx )
        {
            if( decision >= usDy )
            {
                decision -= usDy;
                wCurx += x_sign;
                wNextx -= x_sign;
            }
            for( wpx = wCurx - usWidth / 2;
                 wpx <= wCurx + usWidth / 2; wpx++ )
            {
                PutPixel(wpx, wCury, ucColor);
            }

            for( wpx = wNextx - usWidth / 2;
                 wpx <= wNextx + usWidth / 2; wpx++ )
            {
                PutPixel(wpx, wNexty, ucColor);
            }
        }
    }
}

/*********************************************************************************************
* name:		color_lcd_test()
* func:		LCD test function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void color_lcd_test(void)
{
	int i=290;
	int j = 150;
	float xx = 0;
	int lastX = 50;
	int lastY = 230;
	int time =300;
	uart_printf("\n LCD display Test Example (please look at LCD screen)\n");

	lcd_init_app();
	//BitmapViewTft16Bit_800480((UINT8T *)(g_ucBitmap));

	/*
	Lcd_Draw_Line(5,5,795,475,BLACK,1);
	Lcd_Draw_Line(10,10,790,470,RED,1);
	Lcd_Draw_Line(15,15,785,465,BLUE,1);
	Lcd_Draw_Line(20,20,780,460,GREEN,1);*/

	Lcd_Draw_Line(0, 230, 900, 230, RED, 5);
	Lcd_Draw_Line(50, 0, 50, 460, RED, 5);
	//Lcd_Draw_Line(0, 230, 100,100, RED, 5);
	while(j--)
	{
		Lcd_Draw_Line(lastX, lastY, lastX+5,230-(sin(xx+0.1)*50), RED, 2);
		
		lastX = lastX+5;
		lastY = 230-(sin(xx+0.1)*50);
		xx+=0.1;
	}

	while(1);
 

}
