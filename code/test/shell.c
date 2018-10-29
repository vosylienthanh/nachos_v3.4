#include "syscall.h"

int
main()
{
    SpaceId newProc;
    char ProgramName[STR_MAX_LEN];
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

	while (0 == flgExit)
	{
		PrintString("Please enter program name: ");
		ReadString(ProgramName, STR_MAX_LEN);
		
		j = 0;
		for(j; j < 4; j++)
		{
			if(EXIT[j] != ProgramName[j])
			{
				break;
			}
		}
	
		if(4 == j)
		{
			flgExit = 1;
		}
		else
		{
			char *Temp = ProgramName;
			int flgBackground = 0;
			if(ProgramName[0] == '&')
			{
				++Temp;
				flgBackground = 1;
			}
				
			newProc = Exec(Temp);
			
			if(newProc != -1 && flgBackground != 1)
			{
				Join(newProc);
			}
			else
			{
				PrintString("Program doesn't exist! Please try again!\n");
			}
		}
		
		ProgramName[0] ='\0';
	}
	return 0;
}


