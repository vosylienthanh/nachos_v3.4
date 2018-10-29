#include"syscall.h"
int main()
{
	char FileName[STR_MAX_LEN];
	int flg;
  
	PrintString("Moi ban nhap ten file: ");
	ReadString(FileName, STR_MAX_LEN - 1);
	flg = CreateFile(FileName);
	
	if(flg == -1)
	{
		PrintString("Can't Create file!");
	}
	
	return flg;
}

