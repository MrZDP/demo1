#include "oled.h"

static void OLED_AF_Init()
{
	GPIO_InitTypeDef gpio_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	//����GPIOA��GPIOBʱ��
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB,ENABLE);   
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_SPI2,ENABLE);
	//�ֱ�����PB13��PB15�������
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	
	GPIO_Init (GPIOB, &gpio_InitStruct);
	
	SPI_InitStruct.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_4 ;	
	SPI_InitStruct.SPI_CPHA 				=SPI_CPHA_1Edge ;
	SPI_InitStruct.SPI_CPOL 				=SPI_CPOL_Low ;
	SPI_InitStruct.SPI_CRCPolynomial 		=0x7;	
	SPI_InitStruct.SPI_DataSize 			=SPI_DataSize_8b ;
	SPI_InitStruct.SPI_Direction 			=SPI_Direction_2Lines_FullDuplex ;
	SPI_InitStruct.SPI_FirstBit 			=SPI_FirstBit_MSB ;
	SPI_InitStruct.SPI_Mode 				=SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS 					=SPI_NSS_Soft ;

	SPI_Init (SPI2, &SPI_InitStruct);
	
	SPI_Cmd (SPI2, ENABLE);
}

static void OLED_Pin_Init()
{
	GPIO_InitTypeDef gpio_InitStruct;
	
	//����GPIOA��GPIOBʱ��
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB,ENABLE);   

	gpio_InitStruct.GPIO_Mode 	= GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin 	= GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12 ;
	gpio_InitStruct.GPIO_Speed 	= GPIO_Speed_10MHz ;
	
	GPIO_Init (GPIOB, &gpio_InitStruct);
}

static u8 OLED_SPI2_send_recvByte(u8 byte)
{
	while(SPI_I2S_GetFlagStatus (SPI2, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData (SPI2, byte);
	while(SPI_I2S_GetFlagStatus (SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData (SPI2);
}

static void OLED_GPIO_Init()
{
	OLED_AF_Init();
	OLED_Pin_Init();
}

static void OLED_Send_Command(u8 Command)
{
	OLED_CS_L();
	OLED_DC_L();
	OLED_SPI2_send_recvByte(Command);
	OLED_DC_H();
	OLED_CS_H();
	
}

static void OLED_Send_Data(u8 Data)
{
	OLED_CS_L();
	OLED_DC_H();
	OLED_SPI2_send_recvByte(Data);
	OLED_DC_H();
	OLED_CS_H();
}

//OLED�ĳ�ʼ��
void OLED_Init(void)
{
	OLED_GPIO_Init();
 	//GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	
	OLED_RST_L() ;
	TIM1_Delayms(200);
	OLED_RST_H() ;
	
	OLED_Send_Command(0xAE);//--turn off oled panel
	OLED_Send_Command(0x00);//---set low column address
	OLED_Send_Command(0x10);//---set high column address
	OLED_Send_Command(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Send_Command(0x81);//--set contrast control register
	OLED_Send_Command(0xCF);// Set SEG Output Current Brightness
	OLED_Send_Command(0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_Send_Command(0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_Send_Command(0xA6);//--set normal display
	OLED_Send_Command(0xA8);//--set multiplex ratio(1 to 64)
	OLED_Send_Command(0x3f);//--1/64 duty
	OLED_Send_Command(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Send_Command(0x00);//-not offset
	OLED_Send_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_Send_Command(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Send_Command(0xD9);//--set pre-charge period
	OLED_Send_Command(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Send_Command(0xDA);//--set com pins hardware configuration
	OLED_Send_Command(0x12);
	OLED_Send_Command(0xDB);//--set vcomh
	OLED_Send_Command(0x40);//Set VCOM Deselect Level
	OLED_Send_Command(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Send_Command(0x02);//
	OLED_Send_Command(0x8D);//--set Charge Pump enable/disable
	OLED_Send_Command(0x14);//--set(0x10) disable
	OLED_Send_Command(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_Send_Command(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_Clear();
	OLED_Send_Command(0xAF);
}

u8 OLED_GRAM[144][8];

//���Ժ���
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_Send_Command(0xA6);//������ʾ
		}
	if(i==1)
		{
			OLED_Send_Command(0xA7);//��ɫ��ʾ
		}
}

//��Ļ��ת180��
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_Send_Command(0xC8);//������ʾ
			OLED_Send_Command(0xA1);
		}
	if(i==1)
		{
			OLED_Send_Command(0xC0);//��ת��ʾ
			OLED_Send_Command(0xA0);
		}
}


//����OLED��ʾ 
void OLED_DisPlay_On(void)
{
	OLED_Send_Command(0x8D);//��ɱ�ʹ��
	OLED_Send_Command(0x14);//������ɱ�
	OLED_Send_Command(0xAF);//������Ļ
}

//�ر�OLED��ʾ 
void OLED_DisPlay_Off(void)
{
	OLED_Send_Command(0x8D);//��ɱ�ʹ��
	OLED_Send_Command(0x10);//�رյ�ɱ�
	OLED_Send_Command(0xAE);//�ر���Ļ
}

//�����Դ浽OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_Send_Command(0xb0+i); //��������ʼ��ַ
	   OLED_Send_Command(0x00);   //���õ�����ʼ��ַ
	   OLED_Send_Command(0x10);   //���ø�����ʼ��ַ
	   for(n=0;n<128;n++)
		 OLED_Send_Data(OLED_GRAM[n][i]);
	}
}
//��������
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//�����������
			}
  }
	OLED_Refresh();//������ʾ
}

//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���	
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

//����
//x1,y1:�������
//x2,y2:��������
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)
	{
		incx=1; //���õ������� 
	}
	else if (delta_x==0)
	{
		incx=0;//��ֱ�� 
	}
	else 
	{
		incx=-1;
		delta_x=-delta_x;
	}
	if(delta_y>0)
	{
		incy=1;
	}
	else if (delta_y==0)
	{
		incy=0;//ˮƽ�� 
	}
	else 
	{
		incy=-1;
		delta_y=-delta_x;
	}
	if(delta_x>delta_y)
	{
		distance=delta_x; //ѡȡ�������������� 
	}
	else 
	{
		distance=delta_y;
	}
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size1:ѡ������ 6x8/6x12/8x16/12x24
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)
	{
		size2=6;
	}
	else 
	{
		size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	}
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
		{
			temp=asc2_0806[chr1][i];
		} //����0806����
		else if(size1==12)
        {
			temp=asc2_1206[chr1][i];
		} //����1206����
		else if(size1==16)
        {
			temp=asc2_1608[chr1][i];
		} //����1608����
		else if(size1==24)
        {
			temp=asc2_2412[chr1][i];
		} //����2412����
		else 
		{
			return;
		}
		for(m=0;m<8;m++)
		{
			if(temp&0x01)
			{
				OLED_DrawPoint(x,y,mode);
			}
			else 
			{
				OLED_DrawPoint(x,y,!mode);
			}
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{
			x=x0;y0=y0+8;
		}
		y=y0;
  }
}


//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)
		{
			x+=6;
		}
		else 
		{
			x+=size1/2;
		}
		chr++;
  }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//��ʾ����
//x,y :�������
//num :Ҫ��ʾ������
//len :���ֵ�λ��
//size:�����С
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	for(i=0;i<size3;i++)
	{
		if(size1==16)
		{
			temp=Hzk1[num][i];
		}//����16*16����
		else if(size1==24)
		{
			temp=Hzk2[num][i];
		}//����24*24����
		else if(size1==32)       
		{
			temp=Hzk3[num][i];
		}//����32*32����
		else if(size1==64)
		{	
			temp=Hzk4[num][i];
		}//����64*64����
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)
			{
				OLED_DrawPoint(x,y,mode);
			}
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{
			x=x0;y0=y0+8;
		}
		y=y0;
	}
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16,mode); //д��һ�����ֱ�����OLED_GRAM[][]������
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //��ʾ���
				 {
					for(i=1;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //ʵ������
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y���������
//sizex,sizey,ͼƬ����
//BMP[]��Ҫд���ͼƬ����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
	u16 j=0;
	u8 i,n,temp,m;
	u8 x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}
