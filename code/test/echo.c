#include"syscall.h"

int main()
{
	char str[STR_MAX_LEN];
	char EXIT[5];
	int j;
	int flgExit;
	flgExit = 0;
	EXIT[0] = 'e';
	EXIT[1] = 'x';
	EXIT[2] = 'i';
	EXIT[3] = 't';
	EXIT[4] = 0;
	
	PrintString("\nNeu Nhan exit thi thoat chuong trinh");
	PrintString("\n");
	PrintString(str);

	while (0 == flgExit)
	{
		PrintString("User : ");
		ReadString(str, STR_MAX_LEN);
		PrintString("Console : ");
		PrintString(str);
		PrintChar('\n');
		
		j = 0;
		for(j; j < 4; j++)
		{
			if(EXIT[j] != str[j])
			{
				break;
			}
		}
	
		if(4 == j)
		{
			flgExit = 1;
		}
		
		str[0] ='\0';
	}
	return 0;
}

