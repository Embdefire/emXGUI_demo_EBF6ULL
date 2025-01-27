#include	"GUI_Drv.h"

/*============================================================================*/


/*============================================================================*/

 
/*============================================================================*/
  
void SHP430_Init(int xsize,int ysize,int bpp,struct LCD_TIMING_CFG *cfg) 
{  

	
	cfg->VBPD		=10;
	cfg->VFPD		=10;
	cfg->VSPW		=10;
	
	cfg->HBPD		=10;
	cfg->HFPD		=10;
	cfg->HSPW		=10;
	
	cfg->DPL	=1;
	cfg->EPL	=1;
	cfg->HSPL	=1;
	cfg->VSPL	=1;
	

  	GUI_Printf("SHP430_Init:%d,%d,%dBPP\r\n",xsize,ysize,bpp);

  
} 

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
