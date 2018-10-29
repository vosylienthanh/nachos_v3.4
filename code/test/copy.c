#include "syscall.h"
#include "copyright.h"

int main(int argc, char* argv[])
{
	OpenFileID srcFileID, destFileID;
	int filesz;
	char c;
	char srcFileName[STR_MAX_LEN];
	char destFileName[STR_MAX_LEN];
	
	PrintString("Moi ban nhap ten file goc: ");
	ReadString(srcFileName, STR_MAX_LEN - 1);
	PrintString("Moi ban nhap ten file goc: ");
	ReadString(destFileName, STR_MAX_LEN - 1);
	
	srcFileID = Open(srcFileName, 1);
	destFileID = Open(destFileName, 0);
	
	if (srcFileID == -1)
	{
		PrintString("Can not open source file ");
		return 0;
	}
	if (destFileID == -1)
	{
		PrintString("Can not open dest file ");
		Close(srcFileID);
		return 0;
	}
	
	//Lay file size.
	filesz = Seek(-1, srcFileID);
	Seek(0, srcFileID);
	
	while(filesz > 0)
	{
		Read(&c, 1, srcFileID);
		Write(&c, 1, destFileID);
		
		--filesz;
	} 
	
	Close(srcFileID);
	Close(srcFileID);

	return 0;
}

