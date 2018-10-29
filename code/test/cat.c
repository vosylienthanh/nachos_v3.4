/* cat program */
#include "syscall.h"
#include "copyright.h"

int main(int argc, char* argv[])
{
	OpenFileID fileID;
	int filesz;
	char c;
	char FileName[STR_MAX_LEN];
	
	PrintString("Moi ban nhap ten file: ");
	ReadString(FileName, STR_MAX_LEN);
	
	fileID = Open(FileName, 1);
	
	if (fileID == -1)
	{
		PrintString("Can not open file ");
		return 0;
	}

	filesz = Seek(-1, fileID);
	Seek(0, fileID);
	
	while(filesz > 0)
	{
		Read(&c, 1, fileID);
		PrintChar(c);
		
		--filesz;
	} 
	PrintChar('\n');
	
	Close(fileID);
	
	return 0;
}
