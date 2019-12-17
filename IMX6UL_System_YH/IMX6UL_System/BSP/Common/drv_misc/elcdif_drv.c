

#include "BSP.h"

/*=========================================================================================*/
/*=========================================================================================*/

#define	PIN_MUX		0
//#define	PIN_CFG		0x10B0
//#define	PIN_CFG		0x01B0B0u
#define	PIN_CFG		0x01B0B0u
//#define	PIN_CFG		(1<<16)|(2<<14)|(1<<13)|(1<<12)|(2<<6)|(3<<3)
//#define	PIN_CFG		0x0000

void	elcdif_port_init(int b24)
{
	int i;

	IOMUXC_MUX_LCD->CLK.U =PIN_MUX;
	IOMUXC_CFG_LCD->CLK.U =PIN_CFG;

	IOMUXC_MUX_LCD->ENABLE.U =PIN_MUX;
	IOMUXC_CFG_LCD->ENABLE.U =PIN_CFG;

	IOMUXC_MUX_LCD->HSYNC.U =PIN_MUX;
	IOMUXC_CFG_LCD->HSYNC.U =PIN_CFG;

	IOMUXC_MUX_LCD->VSYNC.U =PIN_MUX;
	IOMUXC_CFG_LCD->VSYNC.U =PIN_CFG;

	IOMUXC_MUX_LCD->RESET.U =PIN_MUX;
	IOMUXC_CFG_LCD->RESET.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA00.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA00.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA01.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA01.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA02.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA02.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA03.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA03.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA04.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA04.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA05.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA05.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA06.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA06.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA07.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA07.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA08.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA08.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA09.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA09.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA10.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA10.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA11.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA11.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA12.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA12.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA13.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA13.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA14.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA14.U =PIN_CFG;

	IOMUXC_MUX_LCD->DATA15.U =PIN_MUX;
	IOMUXC_CFG_LCD->DATA15.U =PIN_CFG;


#if 1
//	if(b24)
	{
		IOMUXC_MUX_LCD->DATA16.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA16.U =PIN_CFG;

		IOMUXC_MUX_LCD->DATA17.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA17.U =PIN_CFG;

		IOMUXC_MUX_LCD->DATA18.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA18.U =PIN_CFG;

		IOMUXC_MUX_LCD->DATA19.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA19.U =PIN_CFG;

		IOMUXC_MUX_LCD->DATA20.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA20.U =PIN_CFG;

		IOMUXC_MUX_LCD->DATA21.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA21.U =PIN_CFG;

		IOMUXC_MUX_LCD->DATA22.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA22.U =PIN_CFG;

		IOMUXC_MUX_LCD->DATA23.U =PIN_MUX;
		IOMUXC_CFG_LCD->DATA23.U =PIN_CFG;

	}
#endif

}

/*=========================================================================================*/
volatile u32 PLL5_Clk=0;
volatile u32 Video_Clk=0;

//LCDʱ������,LCD CLK=24Mhz*loopdiv/postdiv/lcdif1prediv/lcdif1div
//��Ҫ����LCD CLK=9.3Mhz����ôLCD CLK=24*31/8/5/2=9.3Mhz
//loopdiv: PLL5(VIDEO PLL)��loop Divider����ѡ��Χ27~54
//postdiv: PLL5(VIDEO PLL)��post Divider����ѡ��Χ1��2��4��8
//lcdif1prediv��LCDIF PRE��Ƶֵ����ѡ��Χ1~8
//lcdif1div��   LCDIF ��Ƶֵ����ѡ��Χ1~8
void elcdif_clock_init(u8 loopdiv,u8 postdiv,u8 lcdif1prediv,u8 lcdif1div)
{
    clock_video_pll_config_t video_config;
    CCM_Type *pCCM =CCM;
    u32 a;


    CLOCK_EnableClock(kCLOCK_Lcd);
    CLOCK_EnableClock(kCLOCK_Lcdif1);

//    video_config.src=kCLOCK_PllClkSrc24M;
    video_config.loopDivider=loopdiv;
    video_config.postDivider=postdiv;
    video_config.numerator=0;
    video_config.denominator=0;


    //����Video PLL(PLL5)
    CLOCK_InitVideoPll(&video_config);

    //����eLCDIF Preʱ��Դ������ѡ��PLL5(Video PLL)��ΪeLCDIFʱ��Դ����ѡ��Χ��
    //  0     PLL2
    //  1     PLL3 PFD3
    //  2     PLL5
    //  3     PLL2 PFD0
    //  4     PLL2 PFD1
    //  5     PLL3 PFD1

    CLOCK_SetMux(kCLOCK_Lcdif1PreMux,2);
    CLOCK_SetDiv(kCLOCK_Lcdif1PreDiv,lcdif1prediv-1);   //LCDIF_PRE��Ƶֵ����,������ֵ:0~7����ӦΪ1~8��Ƶ
    CLOCK_SetDiv(kCLOCK_Lcdif1Div,lcdif1div-1);         //LCDIF��Ƶֵ���ã�������ֵ:0~7,��ӦΪ1~8��Ƶ��


    a = pCCM->CSCDR2;
    GUI_msleep(50);
    PLL5_Clk=CLOCK_GetPllFreq(kCLOCK_PllVideo);
    GUI_msleep(50);
    Video_Clk= CLOCK_GetFreq(kCLOCK_VideoPllClk);
    GUI_msleep(50);
}

/*=========================================================================================*/

static int t0,t1,frame,fps;

static void lcd_isr(void)
{
	u32 state;

	state = ELCDIF_GetInterruptStatus(LCDIF);
	ELCDIF_ClearInterruptStatus(LCDIF, state);

	if(state & kELCDIF_CurFrameDone)
	{
		int t;

		t =GUI_GetTickCount();
		if((t-t0) >= 1000)
		{
			fps =frame;

			frame =0;
			t0 =t;
		}
		else
		{
			frame++;
		}


	#if 0
	   ELCDIF_SetNextBufferAddr(LCDIF,(uint32_t)lcd_fb[fb_idx%LCD_FB_NUM]);
	   fb_idx++;
	   if(fb_idx>=LCD_FB_NUM)
	   {
	     fb_idx=0;
	   }
	#endif
	  ////GUI_InputPost();
	  // s_frameDone = true;
	}

}

static void LCD_InterruptConfig(void)
{
	frame=0;
	fps =0;
	t0 =GUI_GetTickCount();

	ELCDIF_EnableInterrupts(LCDIF, kELCDIF_CurFrameDoneInterruptEnable);
	SetIRQHandler(LCDIF_IRQn,lcd_isr);
	EnableIRQ(LCDIF_IRQn);
}

void elcdif_init(u32 width,u32 height,u32 fb_addr,u8 hsw,u8 hfp,u8 hbp,u8 vsw,u8 vfp,u8 vbp,u32 pol)
{
	elcdif_rgb_mode_config_t config;

	config.panelWidth = width,
	config.panelHeight = height,
	config.hsw = hsw,
	config.hfp = hfp,
	config.hbp = hbp,
	config.vsw = vsw,
	config.vfp = vfp,
	config.vbp = vbp,
	config.polarityFlags =  pol;

	config.bufferAddr  = (uint32_t)fb_addr,
	config.pixelFormat = kELCDIF_PixelFormatRGB565,
	config.dataBus     = kELCDIF_DataBus24Bit,

//	lcdif_display_setup(fb_addr);

/*
	lcd_fb[0] =GUI_GRAM_Alloc(LCD_XSIZE*LCD_YSIZE*2);
	lcd_fb[1] =GUI_GRAM_Alloc(LCD_XSIZE*LCD_YSIZE*2);
	lcd_fb[2] =GUI_GRAM_Alloc(LCD_XSIZE*LCD_YSIZE*2);
*/
	ELCDIF_RgbModeInit(LCDIF, &config);
	ELCDIF_RgbModeStart(LCDIF);
	LCD_InterruptConfig();
}

/*=========================================================================================*/
