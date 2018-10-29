/* Chương trình in ra bảng mã ASCII.
 *
 */
#include "syscall.h"

int main()
{
	unsigned short i;
	
	PrintString("ASCII(0->255) Table:\n");
	for(i = 0; i < 256; i++)
	{
		PrintInt(i);
		PrintString(": ");
		PrintChar((char)i);
		PrintChar('\n');
	}
	
	return 0;
}