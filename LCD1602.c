#include <REGX52.H>
#include <intrins.h>

//引腳配置：
sbit LCD_RS=P2^6;       //宣告P2_6腳位為命令/資料端,0為命令,1為資料
sbit LCD_RW=P2^5;       //宣告P2_5腳位為讀/寫端,0為寫入LCD,1為從LCD讀取
sbit LCD_EN=P2^7;       //宣告P2_7腳位為使能
#define LCD_DataPort P0 //定義P0暫存器傳輸或接取資料

//函式定義：
/**
  * @brief  LCD1602延時函式，11.0592MHz調用可延時1ms
  * @param  無
  * @retval 無
  */
void LCD_Delay()		//@11.0592MHz
{
	unsigned char i, j;
	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}


/**
  * @brief  LCD1602寫命令
  * @param  Command 要寫入的命令
  * @retval 無
  */
void LCD_WriteCommand(unsigned char Command) //LCD寫命令之函式
{
	LCD_RS=0;             //0表命令
	LCD_RW=0;             //0表寫
	LCD_DataPort=Command; //P0=command
	LCD_EN=1;             //讓LCD把D0~D7的東西接收進去
	LCD_Delay();          //使能後也要等待時間
	LCD_EN=0;
	LCD_Delay();          //等待LCD完成本次內部操作，避免MCU太快送下一筆。
	//(其實能用D7=STA7判斷Busy flag)
}

/**
  * @brief  LCD1602寫數據
  * @param  Data 要寫入的數據
  * @retval 無
  */
void LCD_WriteData(unsigned char Data) //LCD寫資料之函式
{
	LCD_RS=1;          //1表資料
	LCD_RW=0;          //0表寫
	LCD_DataPort=Data; //P0=Data
	LCD_EN=1;          //讓LCD把D0~D7的東西接收進去
	LCD_Delay();       //使能後也要等待時間
	LCD_EN=0;          
	LCD_Delay();       //等待LCD完成本次內部操作，避免MCU太快送下一筆。
	//(其實能用D7=STA7判斷Busy flag)
}

/**
  * @brief  LCD1602設置游標位置
  * @param  Line 行位置，範圍：1~2
  * @param  Column 列位置，範圍：1~16
  * @retval 無
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
//設定LCD游標之函式
{
	if(Line==1)      //若要設定游標在第一行,就執行
	{
		LCD_WriteCommand(0x80|(Column-1));
		//呼叫寫命令之函式,參數為0x80|(Column-1)
		//ex:column=1,0x80|(1-1)==0x80|0x00==1000 0000(0x80)
		//然而1000 0000中的1是給P0_7腳位,實際上P0_7是STA7
		//,並不傳輸或接收資料,實際上000 0000在是DDRAM address
		//因此0x80會被解讀成00H(000 0000)
	}
	else if(Line==2) //若要設定游標在第一行,就執行
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));
		//呼叫寫命令之函式,參數為0x80|(Column-1+0x40)
		//ex:column=1,0x80|(1-1+0x40)==0x80|0x40==1100 0000(0xB0)
		//而1100 0000中的1是給P0_7腳位,100 0000才是DDRAM address
		//因此0xB0被解讀成40H(100 0000),而40H就是第2行第1列之DDRAM address
	}
}

/**
  * @brief  LCD1602初始化函数
  * @param  無
  * @retval 無
  */
void LCD_Init()
{
	LCD_WriteCommand(0x38);//八位數據接口，兩行顯示，5*7點陣
	LCD_WriteCommand(0x0c);//顯示開，游標顯示關，游標閃爍關
	LCD_WriteCommand(0x06);//數據讀寫操作後，游標自動加一，畫面不動
	LCD_WriteCommand(0x01);//游標復位，清屏
}

/**
  * @brief  在LCD1602指定位置上印一個字符
  * @param  Line 行位置，範圍：1~2
  * @param  Column 列位置，範圍：1~16
  * @param  Char 要印的字符
  * @retval 無
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
	//印1個字符之函式
{
	LCD_SetCursor(Line,Column); //決定游標在LCD螢幕哪個位置印字符
	LCD_WriteData(Char);        //呼叫寫資料之函式,參數為欲寫之字符
}

/**
  * @brief  在LCD1602指定位置開始印所给字符串
  * @param  Line 起始行位置，範圍：1~2
  * @param  Column 起始列位置，範圍：1~16
  * @param  String 要印出的字符串
  * @retval 無
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
	//印出字串之函式
	//參數為行,列及字串指標(將字串指定給char *變數,
	//String會存到字串第1個字元之位址)
{
	unsigned char i;
	LCD_SetCursor(Line,Column); //決定游標在LCD螢幕哪個位指開始印字串
	for(i=0;String[i]!='\0';i++)//String[i]!='\0'==*(String+i)!='\0'
	{
		LCD_WriteData(String[i]);
		//每執行1次迴圈印1個字元(String在該輪指向的字元)
	}
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X; //x的y次方
	}
	return Result;
}

/**
  * @brief  在LCD1602指定位置開始印出所给數字
  * @param  Line 起始行位置，範圍：1~2
  * @param  Column 起始列位置，範圍：1~16
  * @param  Number 要印出的數字，範圍：0~65535
  * @param  Length 要印出數字的長度，範圍：1~5
  * @retval 無
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
	//印無號數之函式
{
	unsigned char i;
	LCD_SetCursor(Line,Column); //決定游標在LCD螢幕哪個位指開始印無號數
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
		//ex:Number=12345,Length=5
		//第1輪i=5
		//Number/LCD_Pow(10,i-1)%10+'0'==12345/LCD_Pow(10,4)%10+'0'
		//==12345/(10^4)%10+'0'==12345/10000%10+'0'==1%10+'0'==1+'0'=='1'
		//所以執行LCD_WriteData('1');
		//第2輪i=4
		//Number/LCD_Pow(10,4-1)%10+'0'==12345/LCD_Pow(10,3)%10+'0'
		//==12345/(10^3)%10+'0'==12345/1000%10+'0'==12%10+'0'==2+'0'=='2'
		//所以執行LCD_WriteData('2');
		//依此類推,第3輪i=3.....
	}
}

/**
  * @brief  在LCD1602指定位置開始以有符號十進制印出所给數字
  * @param  Line 起始行位置，範圍：1~2
  * @param  Column 起始列位置，範圍：1~16
  * @param  Number 要印出的数字，範圍：-32768~32767
  * @param  Length 要印出數字的長度，範圍：1~5
  * @retval 無
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column); //決定游標從LCD螢幕哪個位置開始印數字
	if(Number>=0)               //若有號數>=0
	{
		LCD_WriteData('+');     //印出+號
		Number1=Number;         
		//將有號數number指定給無號數number1
	}
	else //否則(若有號數<0)就執行
	{
		LCD_WriteData('-');     //印出-號
		Number1=-Number;
		//在有號數number加負號並指定給無號數number1
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
	//ex:Number1=12345,Length=5
	//第1輪i=5
	//Number1/LCD_Pow(10,i-1)%10+'0'==12345/LCD_Pow(10,4)%10+'0'
	//==12345/(10^4)%10+'0'==12345/10000%10+'0'==1%10+'0'==1+'0'=='1'
	//所以執行LCD_WriteData('1');
	//第2輪i=4
	//Number1/LCD_Pow(10,4-1)%10+'0'==12345/LCD_Pow(10,3)%10+'0'
	//==12345/(10^3)%10+'0'==12345/1000%10+'0'==12%10+'0'==2+'0'=='2'
	//所以執行LCD_WriteData('2');
	//依此類推,第3輪i=3.....
}

/**
  * @brief  在LCD1602指定位置開始以十六進制印出所给數字
  * @param  Line 起始行位置，範圍：1~2
  * @param  Column 起始列位置，範圍：1~16
  * @param  Number 要印出的數字，範圍：0~0xFFFF
  * @param  Length 要印出數字的長度，範圍：1~4
  * @retval 無
  */
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}

/**
  * @brief  在LCD1602指定位置開始以二進制顯示所给數字
  * @param  Line 起始行位置，範圍：1~2
  * @param  Column 起始列位置，範圍：1~16
  * @param  Number 要顯示的數字，範圍：0~1111 1111 1111 1111
  * @param  Length 要顯示數字的長度，範圍：1~16
  * @retval 無
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}
