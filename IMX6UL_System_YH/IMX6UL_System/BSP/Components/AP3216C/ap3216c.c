
#include "BSP.h"
#include "ap3216c.h"
#include "Kernel.h"

/*=========================================================================================*/

static const struct i2c_ops *pIIC=NULL;

/*=========================================================================================*/

static void delay_ms(int ms)
{
	SYS_msleep(ms);
}

/*=========================================================================================*/

//IICдһ���ֽ�  
//reg:�Ĵ�����ַ
//data:Ҫд�������
//����ֵ:0,����
//    ����,�������
static BOOL AP3216C_WriteOneByte(u8 reg,u8 data)
{
	pIIC->Open();

    pIIC->Start();
    if(!pIIC->WriteByte(AP3216C_ADDR|0X00,IIC_ACK))//����������ַ+д����
    {
        pIIC->Stop();

        pIIC->Close();
        return FALSE;
    }
    pIIC->WriteByte(reg,IIC_ACK);         //д�Ĵ�����ַ
    pIIC->WriteByte(data,IIC_ACK);        //��������
    pIIC->Stop();

    pIIC->Close();
    return TRUE;
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 AP3216C_ReadOneByte(u8 reg)
{
    u8 res;

    pIIC->Open();

    pIIC->Start();
    if(!pIIC->WriteByte(AP3216C_ADDR|0X00,IIC_ACK)) //����������ַ+д����
    {
    	pIIC->Stop();

    	pIIC->Close();
    	return FALSE;
    }

    pIIC->WriteByte(reg,IIC_ACK);         //д�Ĵ�����ַ
	pIIC->Start();
	pIIC->WriteByte(AP3216C_ADDR|0X01,IIC_ACK); //����������ַ+������
    res=pIIC->ReadByte(IIC_NoACK);		//������,����nACK
    pIIC->Stop();                 //����һ��ֹͣ����

    pIIC->Close();
    return res;  
}


//��ʼ��AP3216C
//����ֵ:0,��ʼ���ɹ�
//      1,��ʼ��ʧ��
BOOL AP3216C_Init(const struct i2c_ops *iic)
{
	u8 temp=0;
	pIIC =iic;

    //IIC_Init();         				//��ʼ��IIC
    AP3216C_WriteOneByte(0x00,0X04);	//��λAP3216C
    delay_ms(50);						//AP33216C��λ����10ms
    AP3216C_WriteOneByte(0x00,0X03);	//����ALS��PS+IR
	temp=AP3216C_ReadOneByte(0X00);		//��ȡ�ո�д��ȥ��0X03
	if(temp==0X03)
	{
		return TRUE;				//AP3216C����
	}
	return FALSE;					//AP3216Cʧ��
}

//��ȡAP3216C������
//��ȡԭʼ���ݣ�����ALS,PS��IR
//ע�⣡���ͬʱ��ALS,IR+PS�Ļ��������ݶ�ȡ��ʱ����Ҫ����112.5ms
void AP3216C_ReadData(u16 *ir,u16 *ps,u16 *als)
{
    u8 buf[6];
    u8 i;
    for(i=0;i<6;i++)
    {
        buf[i]=AP3216C_ReadOneByte(0X0A+i);		//ѭ����ȡ���д���������
    }
    if(buf[0]&0X80)
    {
    	*ir=0;						//IR_OFλΪ1,��������Ч
    }
	else
	{
		*ir=((u16)buf[1]<<2)|(buf[0]&0X03); 	//��ȡIR������������
	}

    *als=((u16)buf[3]<<8)|buf[2];				//��ȡALS������������

    if(buf[4]&0x40)
    {
    	*ps=0;    					//IR_OFλΪ1,��������Ч
    }
	else
	{
		*ps=((u16)(buf[5]&0X3F)<<4)|(buf[4]&0X0F); //��ȡPS������������
	}
}
