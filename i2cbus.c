/******************************************************************
creat date：2017/4/6
promgramer: yang
******************************************************************/
#include<reg52.h>
#include<intrins.h>

#define address 0xa0

sbit SDA = P2^0;
sbit SCL = P2^1;



/*5us延时*/
void delay_5us()  
{
	_nop_();
}

/*1Ms延时*/
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}

/*I2C初始化*/
void I2C_init()	
{
	SDA = 1;
	_nop_();
	SCL = 1;
	_nop_();
}

/*I2C起始信号*/
void I2C_Start()  
{
	SCL = 1;
	_nop_();
	SDA = 1;
	delay_5us();
	SDA = 0;
	delay_5us();
}

/*I2C终止信号*/
void I2C_Stop()
{
	SDA = 0;
	_nop_();
	SCL = 1;
	delay_5us();
	SDA = 1;
	delay_5us();
}

/*主机发送应答*/
void Master_ACK(bit i)		
{
	SCL = 0; // 拉低时钟总线允许SDA数据总线上的数据变化
	_nop_(); // 让总线稳定
	if (i)	 //如果i = 1 那么拉低数据总线 表示主机应答
	{
		SDA = 0;
	}
	else	 
	{
		SDA = 1;	 //发送非应答
	}
	_nop_();//让总线稳定
	SCL = 1;//拉高时钟总线 让从机从SDA线上读走 主机的应答信号
	delay_5us();
	SCL = 0;//拉低时钟总线， 占用总线继续通信
	_nop_();
	SDA = 1;//释放SDA数据总线。
	_nop_();
}

/*检测从机应答*/
bit Test_ACK()
{
	SCL = 1;
	delay_5us();
	if (SDA)
	{
		SCL = 0;
		_nop_();
		I2C_Stop();
		return(0);
	}
	else
	{
		SCL = 0;
		_nop_();
		return(1);
	}
}

/*发送一个字节*/
void I2C_send_byte(uchar byte)
{
	uchar i;
	for(i = 0 ; i < 8 ; i++)
	{
		SCL = 0;
		_nop_();
		if (byte & 0x80)
		{				
			SDA = 1;	
			_nop_();				   
		}				
		else
		{
			SDA = 0;
			_nop_();
		}
		SCL = 1;
		_nop_();
		byte <<= 1;	// 0101 0100B 
	}
	SCL = 0;
	_nop_();
	SDA = 1;
	_nop_();
}


/*I2C 读一字节*/
uchar I2C_read_byte()
{
	uchar dat,i;
	SCL = 0;
	_nop_();
	SDA = 1;
	_nop_();
	for(i = 0 ; i < 8 ; i++)
	{
		SCL = 1;
		_nop_();
		if (SDA)			    
		{
			 dat |= 0x01; //
		}
		else
		{
			dat &=  0xfe;	//1111 1110
		}
		_nop_();
		SCL = 0 ;
		_nop_();
		if(i < 7)
		{
			dat = dat << 1;	
		}
	}
	return(dat);
}

/*I2C发送数据*/
bit I2C_TransmitData(uchar adr, dat)
{
	I2C_Start();
	I2C_send_byte(adress+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(adr); 
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(dat);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_Stop();
	return(1);	
}

/*I2C接收数据*/
uchar I2C_ReceiveData(uchar adr)//return(dat);
{
	uchar dat;
	I2C_Start();
	I2C_send_byte(adress+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(adr);
	Master_ACK(0);
	I2C_Start();
	I2C_send_byte(adress+1);
	if (!Test_ACK())
	{
		return(0);
	}
	dat = I2C_read_byte();
	Master_ACK(0);
	I2C_Stop();
	return(dat);	
}



























