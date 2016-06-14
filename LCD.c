/* lcd.c **********************************************************************************
 *        Funktionen zur Ansteuerung des LCD-Displays
 ******************************************************************************************/
 
// #include <reg51.h>							//	Registeradressen											
#include <AT898252.h>
#include <intrins.h>						// für _crol_ -Funktion									
#include <stdio.h>							//	für sprintf													

unsigned char LCD_PORT;
#define LCD_PORT P1 							//	LCD-Display an P0 im Port-4-Bit-Modus				

/******************************************************************************************
 *  Zeitverzögerung:  wartet ca.  anzahl mal 100µs                                                     
 ******************************************************************************************/
void warte100u (unsigned char anzahl)	
	{
 /*   unsigned char z1,z2; 
	for (z2 = anzahl;	z2 != 0;	--z2)
		{
		for (z1 = 255; 	z1!= 0; 	--z1);
		}; 
 */
	}

/*****************************************************************************************
 * 	Gibt das Byte  befehl  als Befehl an LCD-Display im 4Bit-Modus                              
 *      Entweder Busy-Flag des LCD abfragen oder Zeitverzögerung verwenden        
 *****************************************************************************************/
void LCDbefehl (unsigned char befehl)		
	{	
	unsigned char a1;
	a1 = befehl;	
	a1 = _crol_ (a1,4); 			// in intrins.h, char um 4 Bits rolieren -> High und Lownibbel vertauschen 
																
	a1 = (a1 & 0x0F) | 0x10;	//	4 Bits maskieren, Übergabetakt = 1, High-Nibbel senden	
	LCD_PORT = a1;
	LCD_PORT = a1 & 0x00;		// Takt = 0 																
	a1 = befehl;					// Low-Nibbel																
	a1 = (a1 & 0x0F) | 0x10;	//	4 Bits maskieren, Übergabetakt = 1								
	LCD_PORT = a1;
	LCD_PORT = a1 & 0x00;		// Takt = 0 																
	
/***** Busy-Abfrage ***********************************************************************	
	do {
		LCD_PORT = 0x5F;			//  Busy lesen, Takt=1, RS = 0										
		a1 = LCD_PORT	;			//	und holen																
		LCD_PORT = 0x4F;			//	Takt=0																	
		a = a;
		LCD_PORT = 0x5F;			//	Low-Byte holen (ohne Bedeutung)									
		LCD_PORT = 0x4F;		
		}
	while ( (a1 & 0x08) != 0 ); //	warten solange Busy high										
 ***** Ende Busy-Abfrage ******************************************************************/
	
/*	wenn Busy nicht geht, Zeitverzögerung 1ms verwenden 	**********************************/
	warte100u (10);
	}
	
/******************************************************************************************
 * Initialisierung des LCD- Displays für den 4-Bit-Modus                                               
 ******************************************************************************************/
void initlcd (void)				
	{
	LCD_PORT = 0x13;			//	aufwecken! 	Takt=1	00010011											
	LCD_PORT = 0x03;			//					Takt=0	00000011											
	warte100u (50);		  	    //	5ms warten		       													
	LCD_PORT = 0x13;			//	aufwecken! 	Takt=1														
	LCD_PORT = 0x03;			//					Takt=0														
	warte100u (1);				//	100us warten		      												
	LCD_PORT = 0x13;			//	aufwecken! 	Takt=1														
	LCD_PORT = 0x03;			//				Takt=0															
	warte100u (1);				//	100us warten		       												
	LCD_PORT = 0x12;			//	8->4  BITS UMSCHALTEN,	Takt=1										
	LCD_PORT = 0x02;   		    //						Takt=0													
	warte100u (1);				//	100us warten		       												
	LCDbefehl (0x28);			//	Function set 4 bits  													
	LCDbefehl (0x0C);			//	Display AN, Cursor  AUS													
	LCDbefehl (0x06);			//	Not Shifted Display, Increment										
	}

/******************************************************************************************
 * Display löschen	                                                                                                      
 ******************************************************************************************/
void loeschenlcd (void)		
	{	LCDbefehl (0x01);	}

/*****************************************************************************************
 * 	Ausgabe eines Zeichens an das LCD-Display                                                          
 *      Entweder Busy-Flag des LCD abfragen oder Zeitverzögerung verwenden         
 ******************************************************************************************/
void charlcd (unsigned char zeichen)	
	{
	unsigned char a1;
	a1 = zeichen ;	
	a1 = _crol_ (a1,4); 	// in intrins.h, char um 4 Bits rolieren -> High und Lownibbel 
								//	vertauschen im Debugger getestet, geht 																			
	a1 = (a1 & 0x0F) | 0x90;	//	4 Bits maskieren, Übergabetakt = 1, High-Nibbel senden	
	LCD_PORT = a1;
	LCD_PORT = a1 & 0x80;		// Takt = 0 																
	a1 = zeichen;					// Low-Nibbel																
	a1 = (a1 & 0x0F) | 0x90;	//	4 Bits maskieren, Übergabetakt = 1								
	LCD_PORT = a1;
	LCD_PORT = a1 & 0x80;		// Takt = 0 																
	
/**** Busy-Abfrage ***********************************************************************	
	do {
		LCD_PORT = 0x5F;			//  Busy lesen, Takt=1, RS = 0										
		a1 = LCD_PORT	;			//	und holen																
		LCD_PORT = 0x4F;			//	Takt=0																	
		a = a;
		LCD_PORT = 0x5F;			//	Low-Byte holen (ohne Bedeutung)									
		LCD_PORT = 0x4F;		
		}
	while ( (a1 & 0x08) != 0 ); //	warten solange Busy high										
 **** Ende Busy-Abfrage ******************************************************************/	
	
//	wenn Busy nicht geht, Zeitverzögerung 1ms verwenden ************************************/
	warte100u (10);
	}

/*****************************************************************************************
 * LCD-Cursor auf eine position setzen: Zeile 1: Adresse 0x00 bis 0x0F                     
 *                                      Zeile 2: Adresse 0x40 bis 0x4F                     
 ******************************************************************************************/
void cursorpos (unsigned char position)
	{
	LCDbefehl ( position | 0x80 );	// Kennung für DD RAM address set							
	}

/*****************************************************************************************
 * LCD-Cursor auf die Position Home 0x00 setzen                                           
 ******************************************************************************************/	
void cursorhome (void)
	{	LCDbefehl (0x02 );	}

// *********** Cursor AN ************************************************
void cursoran (void)
    { LCDbefehl ( 0x0E );  }   // Cursor und Display AN

// *********** Cursor AUS ***********************************************
void cursoraus (void)
    { LCDbefehl ( 0x0C );  }   // Cursor aus und Display AN


/*****************************************************************************************
 * Text an das LCD-Display in Zeile 1 oder 2 ausgeben                          
 ******************************************************************************************/
void textlcd (unsigned char *text, unsigned char zeile)
// void textlcd (unsigned char text [16], unsigned char zeile) 									
	{
	if 	(zeile == 1)	{ cursorpos (0x00);}	//	LCD-Zeile 1											
	else 	            { cursorpos (0x40);}	// LCD-Zeile 2										
	while (*text)								//	Text													
	  {
    	charlcd(*text++);                      //	zeichenweise ausgeben 
        warte100u (1000);
       } 							
	}
	

