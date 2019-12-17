
/*
 *LCD_BL_Setup.c
 *2019��7��3������8:07:40
 *
 */

#include "emXGUI.h"
#include "BSP.h"

/*============================================================================*/
//����ؼ�ID
#define	ID_OK		0x1000
#define	ID_SCROLL1	0x1001

#define	WIN_W	GUI_XSIZE
#define	WIN_H	100

void BL_PWM_Set(u8 val);

/*============================================================================*/


#define	MOVE_STOP	0
#define	MOVE_UP		1
#define	MOVE_DOWN	2
#define	MOVE_EXIT	3

static int move_stat=0;
static u8 bl_val=30;
void BL_PWM_Set(u8 val);

static LRESULT	WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	RECT rc;

	switch(msg)
	{
		case WM_CREATE: //���ڴ���ʱ,���Զ���������Ϣ,��������һЩ��ʼ���Ĳ����򴴽��Ӵ���.
		{
			move_stat =MOVE_DOWN;
			GetClientRect(hwnd,&rc); //��ô��ڵĿͻ�������.

			//CreateWindow(BUTTON,L"OK",WS_VISIBLE,rc.w-80,8,68,32,hwnd,ID_OK,NULL,NULL); //����һ����ť(ʾ��).

			if(1)
			{

				HWND wnd;
				SCROLLINFO sif;
				sif.cbSize		=sizeof(sif);
				sif.fMask		=SIF_ALL;
				sif.nMin		=1;
				sif.nMax		=100;

				sif.nValue		=bl_val;
				sif.TrackSize		=60;
				sif.ArrowSize		=0;//20;//20;

				wnd = CreateWindow(SCROLLBAR,L"HScroll",SBS_NOARROWS|WS_VISIBLE,0,0,rc.w,50,hwnd,ID_SCROLL1,NULL,NULL);
				SendMessage(wnd,SBM_SETSCROLLINFO,TRUE,(LPARAM)&sif);
			}

			SetTimer(hwnd,1,50,TMR_START,NULL);
		}
		return TRUE;
		////

		case WM_TIMER:
		{
			if(move_stat==MOVE_DOWN)
			{
				int endy=20;
				GetWindowRect(hwnd,&rc);
				if(rc.y < endy)
				{
					rc.y += MAX(1,(endy-rc.y)>>2);
					rc.y  =MIN(rc.y,endy);

					ScreenToClient(GetParent(hwnd),(POINT*)&rc.x,1);
					MoveWindow(hwnd,rc.x,rc.y,rc.w,rc.h,TRUE);
				}
				else
				{
					move_stat =MOVE_STOP;
				}

			}

			if(move_stat==MOVE_UP)
			{
				int endy =-WIN_H;
				GetWindowRect(hwnd,&rc);
				if(rc.y >  endy)
				{
					rc.y -= MAX(1,(rc.y-endy)>>2);
					rc.y  =MAX(rc.y,endy);

					ScreenToClient(GetParent(hwnd),(POINT*)&rc.x,1);
					MoveWindow(hwnd,rc.x,rc.y,rc.w,rc.h,TRUE);
				}
				else
				{
					move_stat =MOVE_EXIT;
					PostCloseMessage(hwnd);
				}

			}

		}
		break;
		////

		case WM_NOTIFY: //WM_NOTIFY��Ϣ:wParam��16λΪ���͸���Ϣ�Ŀؼ�ID,��16λΪ֪ͨ��;lParamָ����һ��NMHDR�ṹ��.
		{
			u16 code,id;

			code =HIWORD(wParam); //���֪ͨ������.
			id   =LOWORD(wParam); //��ò�������Ϣ�Ŀؼ�ID.

			if(id==ID_OK && code==BN_CLICKED) // ��ť����������.
			{
				PostCloseMessage(hwnd); //ʹ����WM_CLOSE��Ϣ�رմ���.
			}

			if(id == ID_SCROLL1)
			{
				NM_SCROLLBAR *nr;
				int i;

				nr =(NM_SCROLLBAR*)lParam;
				if(code==SBN_THUMBTRACK) //�����ƶ�
				{
					bl_val =nr->nTrackValue;
					BL_PWM_Set(bl_val);
					SendMessage(nr->hdr.hwndFrom,SBM_SETVALUE,TRUE,bl_val); //����λ��ֵ


				}

			}
		}
		break;
		////

		case WM_PAINT: //������Ҫ����ʱ�����Զ���������Ϣ.
		{
			PAINTSTRUCT ps;
			HDC hdc;

			hdc =BeginPaint(hwnd,&ps); //��ʼ��ͼ

			////�û��Ļ�������...
			//TextOut(hdc,10,10,L"Hello",-1);

			EndPaint(hwnd,&ps); //������ͼ
		}
		break;
		////

		case WM_CLOSE: //���ڹر�ʱ�����Զ���������Ϣ.
		{
			if(move_stat==MOVE_EXIT)
			{
				return DestroyWindow(hwnd); //����DestroyWindow�������ٴ��ڣ��ú�����ʹ�����ڽ������˳���Ϣѭ��;���򴰿ڽ���������.
			}
			else
			{
				move_stat=MOVE_UP;
			}
		}
		break;
		////

		default: //�û������ĵ���Ϣ,��ϵͳ����.
		{
			return DefWindowProc(hwnd,msg,wParam,lParam);
		}

	}

	return WM_NULL;
}

/*============================================================================*/

//static void	AppMain(void)
void	LCD_BL_Setup(void)
{
	HWND	hwnd;
	WNDCLASS	wcex;
	MSG msg;
	int x,y,w,h;
	/////

	w =WIN_W-8*2;
	h =WIN_H;
	x =(GUI_XSIZE-w)/2;
	y =-(h+4);

	wcex.Tag 		    = WNDCLASS_TAG;

	wcex.Style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WinProc; //������������Ϣ����Ļص�����.
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;

	//����������
	hwnd	=CreateWindowEx(	WS_EX_FRAMEBUFFER,
								&wcex,
								L"Backlight", //��������
								WS_OVERLAPPEDWINDOW,
								x,y,w,h,    //����λ�úʹ�С
								NULL,0,NULL,NULL);

	//��ʾ������
	ShowWindow(hwnd,SW_SHOW);

	//��ʼ������Ϣѭ��(���ڹرղ�����ʱ,GetMessage������FALSE,�˳�����Ϣѭ��)��
	while(GetMessage(&msg,hwnd))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/*============================================================================*/

#if 1
static int app_run=FALSE;
static int app_thread(void *argv)
{
	LCD_BL_Setup();
	app_run=FALSE;
}

void	app_LCD_BL_Setup(HWND hwndPrev)
{
	if(app_run==FALSE)
	{
		app_run =TRUE;
		GUI_Thread_Create(app_thread,hwndPrev,4096,NULL,0);
	}
	else
	{
#if 0
		MSGBOX_OPTIONS ops;
		const WCHAR *btn[]={L"Return"};

		ops.Flag =MB_BTN_WIDTH(60)|MB_ICONINFORMATION;
		ops.pButtonText =btn;
		ops.ButtonCount =1;

		MessageBox(hwndPrev,20,50,250,150,L"������������ ...",L"��Ϣ",&ops);
#endif
	}

}
#endif
/*=========================================================================*/

