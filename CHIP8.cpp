#include<stdio.h>
#include<graphics.h>
#include <stdlib.h>
//SCREEN
int counter=0,PC=512,rom=0x50,ONE=0,c=0;
UINT16 grid[32][64];

void pixel_draw(int X, int Y, UINT16 value)
{
	int i;
	if(value>0)
		for(i=0;i<10;i++)
			{
				setcolor(GREEN);
				line(X,Y+i,X+10,Y+i);
			}
	else
		for(i=0;i<10;i++)
			{
				setcolor(BLACK);
				line(X,Y+i,X+10,Y+i);
			}		
}
void grid_draw()
{
	BYTE i,j;
	int pixel_count_x=0,pixel_count_y=0;
	for(i=0;i<32;i++)
	{
		pixel_count_x=0;
		pixel_count_y+=10;
		for(j=0;j<64;j++)
		{
			pixel_count_x+=10;
			pixel_draw(pixel_count_x,pixel_count_y,grid[i][j]);
		}
	}
}

//mem struct

typedef struct 
{
	BYTE RAM[4096];	//4K 
	UINT16 PC;     //counter
	BYTE INST_UP;       // I REG
	BYTE INST_DOWN;
	UINT16 I;  
	UINT16 STACK[10]; //STACK 
	int STACK_POINTER;
	BYTE TIMER1;    // TMR1
	BYTE TIMER2;    //SOUND BASED TIMER
	BYTE V[16];      //GP REGS
	//160-512 LOADING IN RAM program
}MEMORY;

void init_MEMORY(MEMORY *MEM)
{
	unsigned int i,j,counter=0;
	
	BYTE FONT[][5]=     //FONT 
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
	
	MEM->PC=PC;
	MEM->TIMER1=0;
	MEM->TIMER2=0;
	MEM->STACK_POINTER=0X00;
	
	for(i=0;i<16;i++)
	{
		MEM->V[i]=0x00;
	}
	
	for(i=0;i<10;i++)
	{
		MEM->STACK[i]=0;
	}
	
	for(i=0;i<4096;i++)
	{
		MEM->RAM[i]=0;
	}
		MEM->INST_UP=0;
		MEM->INST_DOWN=0;
	// init FONT  80-160
	for(i=0;i<16;i++)
	{
		for(j=0;j<5;j++)
			{
				MEM->RAM[rom+counter]=FONT[i][j];
				counter++;
			}
	}
}

void run(MEMORY *MEM)
{
	BYTE i,j;
	UINT16 N3=0X00,N2=0X00,N1=0X00,N0=0X00;
	MEM->INST_DOWN=MEM->RAM[MEM->PC];
	MEM->PC++;
	MEM->INST_UP=MEM->RAM[MEM->PC];
	MEM->PC++;
	printf("RUNING INST=%X%X\n",MEM->INST_UP,MEM->INST_UP);
	if(MEM->TIMER1>0)
	{
	
		MEM->TIMER1--;
		MEM->TIMER2--;
		
	}
	
	N3=(MEM->INST_DOWN&0XF0)>>4;
	N2=(MEM->INST_DOWN&0X0F);
	N1=(MEM->INST_UP&0XF0)>>4;         //divide to nibles N3 MS
	N0=(MEM->INST_UP&0X0F);

	//execute
	if(MEM->INST_UP==0XE0 && MEM->INST_DOWN==0X00)
	{
		for(i=0;i<32;i++)  //2 shifted right
    	for(j=0;j<64;j++)
    		grid[i][j]=0;
    	grid_draw();
	}
	
	if(N3==0X1)
	{
		MEM->PC=(N2<<8)+(N1<<4)+N0;
	}

	if(N3==0X6)
	{
		MEM->V[N2]=(N1<<4)+N0;
	}
	if(N3==0X07)
	{
		MEM->V[N2]=(N1<<4)+N0+MEM->V[N2];
	}
	if(N3==0X0A)
	{
		MEM->I=(N2<<8)+(N1<<4)+N0;
	}
	if(N3==0XD)
	{
		UINT16 i=0,j,k,pom;
		UINT16 DRAW[7];
		while(true)
		{
			i++;
			if((i-1)==((MEM->INST_UP&0X0F)))
				break;
			UINT16 pom=MEM->RAM[MEM->I+i-1];
	
			for(j=0;j<8;j++)
			{
				DRAW[j]=pom%2;
				pom/=2;
			}
			
			for(j=0;j<8;j++)
			{
				MEM->V[15]=0;
				if(DRAW[7-j])	//viditi
					{
						
						if(grid[MEM->V[N1]+i][MEM->V[N2]+j]==1)
						{
							MEM->V[15]=0x01;
						}
						else
						{
							MEM->V[15]=0x00;
						}
						grid[MEM->V[N1]+i][MEM->V[N2]+j]^=0XFFFFFF;
						pixel_draw((MEM->V[N2]+j)*10,(MEM->V[N1]+i)*10,grid[MEM->V[N1]+i][MEM->V[N2]+j]);	
					}
			}
		}
	}
	
	if(MEM->INST_DOWN==0X00 && MEM->INST_UP==0XEE)
	{
		MEM->STACK_POINTER;
		MEM->PC=MEM->STACK[MEM->STACK_POINTER];
		MEM->STACK_POINTER--;
	}
	
	if(N3==0X2)
	{
		MEM->STACK_POINTER++;
		MEM->STACK[MEM->STACK_POINTER]=MEM->PC;
		MEM->PC=(N2<<8)+(N1<<4)+N0;
	}
	
	if(N3==0X3)
	{
		
		if(MEM->V[N2]==(N1<<4)+N0)
			MEM->PC+=2;
	}
	
	if(N3==0X4)
	{
		if(MEM->V[N2]!=(N1<<4)+N0)
			MEM->PC+=2;
	}
	
	if(N3==0X5 && N0==0x0)
	{
		if(MEM->V[N2]==MEM->V[N1])
			MEM->PC+=2;
	}
	
	if(N3==0X8 && N0==0x0)
	{
	
		MEM->V[N2]=MEM->V[N1];	
	}
	
	if(N3==0X8 && N0==0x1)
	{
		MEM->V[N2]|=MEM->V[N1];
	}
	
	if(N3==0X8 && N0==0x2)
	{
		MEM->V[N2]&=MEM->V[N1];	
	}
	
	if(N3==0X8 && N0==0x3)
	{
		MEM->V[N2]^= MEM->V[N1];	
	}
	
	if(N3==0X8 && N0==0x4)
	{
		UINT16 rez=MEM->V[N2]+MEM->V[N1];
		if(rez>256)
			MEM->V[15]=0X01;
		else
			MEM->V[15]=0x00;
		MEM->V[N2]=rez&0XFF;	
	}
	
	if(N3==0X8 && N0==0x5)
	{	
		UINT16 rez=MEM->V[N2]-MEM->V[N1];
		if(MEM->V[N2]>MEM->V[N1])
			MEM->V[15]=0X01;
		else
			MEM->V[15]=0x00;
		MEM->V[N2]=rez&0XFF;		
	}
	
	if(N3==0X8 && N0==0x6)
	{
		UINT16 rez=MEM->V[N2]>>1;
		MEM->V[15] = (MEM->V[N1] & (1 << 0));
		MEM->V[N2]=rez;
	}
	
	if(N3==0X8 && N0==0x7)
	{
		UINT16 rez=MEM->V[N1]-MEM->V[N2];
		if(MEM->V[N2]<MEM->V[N1])
			MEM->V[15]=0X01;
		else
			MEM->V[15]=0x00;
		MEM->V[N2]=rez&0XFF;		
	}
	
	if(N3==0X8 && N0==0xE)
	{
		UINT16 rez=MEM->V[N2]<<1;
		MEM->V[15] = !(MEM->V[N1] & (1 << 0));
		MEM->V[N2]=rez;
	}
	
	if(N3==0X9 && N0==0x0)
	{
		if(MEM->V[N2]!=MEM->V[N1])
			MEM->PC+=2;	
	}
	
	if(N3==0XB)
	{
		MEM->PC=MEM->V[0]+((N2<<8)+(N1<<4)+N0);
	}
	if(N3==0XC)
	{
		MEM->V[N2]=(rand()%255)&((N1<<4)+N0);
	}
	
	if(N3==0XE && MEM->INST_UP==0X9E)
	{
		
				if((c-48)==MEM->V[N2])
				{
					MEM->PC+=2;
				}
	}
	
	if(N3==0XE && MEM->INST_UP==0XA1)
		{
	
				if((c-48)!=MEM->V[N2])
				{
					MEM->PC+=2;
				}
		}
	
	if(N3==0XF && MEM->INST_UP==0X07)
	{
		MEM->V[N2]=MEM->TIMER1;
	}
	
	if(N3==0XF && MEM->INST_UP==0X0A)
	{
		
		switch(c)
		{
			case '1':
				MEM->V[N2]=0x00;
				break;
			
			case '2':
				MEM->V[N2]=0x01;
				break;
			
			case '3':
				MEM->V[N2]=0x02;
				break;
				
			case '4':
				MEM->V[N2]=0x03;
				break;
				
			case 'q':
				MEM->V[N2]=0x04;
				break;
				
			case 'w':
				MEM->V[N2]=0x05;
				break;
				
			case 'e':
				MEM->V[N2]=0x06;
				break;
				
			case 'r':
				MEM->V[N2]=0x07;
				break;
			
			case 'a':
				MEM->V[N2]=0x08;
				break;
				
			case 's':
				MEM->V[N2]=0x09;
				break;
			
			case 'd':
				MEM->V[N2]=0x0A;
				break;
				
			case 'f':
				MEM->V[N2]=0x0B;
				break;
				
			case 'z':
				MEM->V[N2]=0x0C;
				break;
				
			case 'x':
				MEM->V[N2]=0x0D;
				break;
				
			case 'c':
				MEM->V[N2]=0x0E;
				break;
				
			case 'v':
				MEM->V[N2]=0x0F;
				break;
					
			default:
				MEM->V[N2]=0x00;	
		}	
	}
	if(N3==0XF&&MEM->INST_UP==0x15)
	{
		MEM->TIMER1=MEM->V[N2];
	}
	
	if(N3==0XF&&MEM->INST_UP==0x18)
	{
		MEM->TIMER2=MEM->V[N2];
	}
	
	if(N3==0XF&&MEM->INST_UP==0x1E)
	{
		MEM->I+=MEM->V[N2];
	}
	
	if(N3==0XF&&MEM->INST_UP==0x29)
	{
		MEM->I=(MEM->V[N2]*0X05)+0X50;    //POSSS ERROR
	}
	
	if(N3==0XF&&MEM->INST_UP==0x33)
	{
		MEM->RAM[MEM->I]=MEM->V[N2]/100;
		MEM->RAM[MEM->I+1]=(MEM->V[N2]/10)%10;
		MEM->RAM[MEM->I+2]=MEM->V[N2]%10;
	}
	
	if(N3==0XF&&(MEM->INST_UP==0X55))
	{
		UINT16 i;
		for(i=0;i<=N2;i++)
			MEM->RAM[MEM->I+i]=MEM->V[i];
	}
	
	if(N3==0XF && MEM->INST_UP==0X65)
	{
		UINT16 i;
		for(i=0;i<=N2;i++)
			{
				MEM->V[i]=MEM->RAM[MEM->I+i];
			}
	}
	//ende
}
int main()
{
	
	char text[20]="";
	MEMORY MEM;
	FILE *ROM;
	int x = 0;
	UINT16 i,j,count=0;
	int gdriver = DETECT, gmode, errorcode;
	BYTE buffer ;
	init_MEMORY(&MEM);
	initwindow(640,320);
	initgraph(&gdriver, &gmode, "");
    for(i=0;i<32;i++)  //2 shifted right
    for(j=0;j<64;j++)
    	grid[i][j]=0;
    //LOADING ROM
    fflush(stdin);
    printf("ROM NAME:\n");
    gets(text);
    if ((ROM = fopen(text,"rb")) == NULL)
	{
		printf("ERROR!!!!!");
		exit(1);
	}
    while(!feof(ROM))
	{
		fread(&buffer,sizeof(buffer),1,ROM);
		MEM.RAM[PC+count]=buffer;
		count++;
	}
		count=0;	
	    fclose(ROM);

	while(1)
	{
        delay(5);
		run(&MEM);
		if(kbhit())
		{
			c=getch();
		}
	}
	closegraph();
    system("exit");
	return 0;

}

