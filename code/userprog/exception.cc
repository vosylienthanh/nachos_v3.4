// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
#include "directory.h"
# include "copyright.h"
# include "system.h"
# include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
#define MaxFileLength 32
# include "string.h"

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	switch (which)
	{
	case NoException:
		return;
	case SyscallException:
		switch (type)
		{
			//Syscall Halt.
		case SC_Halt:
		{
			DEBUG('a', "Shutdown, initiated by user program.\n");
			interrupt->Halt();

			break;
		}
		//Đọc một số nguyên.
		case SC_ReadInt:
		{
			DEBUG('a', "Read integer number from console.\n");
			/* NOTE: Trong trường hợp này ta có 2 cách làm.
			* C1: Đọc dùng synchconsole sau đó dùng hàm 'atoi' được khai báo extern trong sysdep.h
			* nếu như vượt overflow -> lấy mod.
			* C2: Đọc dùng synchconsole sau đó tự viết xử lí.
			*/

			int iNumber = 0;
			int nReadLen = 0;
			char Temp;
			char* pBuffer = new char[INT_MAX_LEN];//12
												  //Đọc dùng synchconsole.
												  //nReadLen = gSynchConsole->Read(pBuffer, INT_MAX_LEN - 1);

			while (gSynchConsole->Read(&Temp, 1) == 1 && Temp != '\n')
			{
				if (nReadLen < INT_MAX_LEN - 1)
				{
					pBuffer[nReadLen] = Temp;
					++nReadLen;
				}
			}

			pBuffer[nReadLen] = 0;

			//Cách 01:
			int iIndex = 0;
			bool NotANumber = false; //Flag đánh dấu là số.
			if ('-' == pBuffer[0] || '+' == pBuffer[0]) //Bỏ qua dấu '-'
			{
				iIndex = 1;
			}

			while (pBuffer[iIndex] != 0 && !NotANumber)
			{
				if (pBuffer[iIndex] >= '0' && pBuffer[iIndex] <= '9')
				{
					iNumber = iNumber * 10 + (int)(pBuffer[iIndex] & 0xF);
				}
				else
				{
					NotANumber = true;
				}

				++iIndex;
			}

			if (NotANumber)
			{
				iNumber = 0; //Theo yêu cầu của đồ án.
			}
			else
			{
				if ('-' == pBuffer[0])
				{
					iNumber = -iNumber;
				}
			}
			//printf("%d\n", iNumber); //Kiểm tra.
			/*
			//Cách 02:
			iNumber = atoi(pBuffer);
			//printf("%d\n", iNumber); //Kiểm tra.
			*/
			machine->WriteRegister(2, iNumber);
			if (pBuffer != NULL)
			{
				delete[] pBuffer;
			}

			break;
		}
		//Xuất một số nguyên.
		case SC_PrintInt:
		{
			DEBUG('a', "Print integer number to console.\n");
			/* NOTE: Trong trường hợp này ta có 2 cách làm.
			* C1: Tự viết xử lí chuyển số sang chuỗi.
			* C2: Dùng sprintf().
			*/

			int iNumber = machine->ReadRegister(4); //Đọc từ register số 4.;
			char* pBuffer = new char[INT_MAX_LEN];

			//CÁCH 01:
			char cSign = '-';

			if (0 == iNumber)
			{
				pBuffer[0] = '0';
				gSynchConsole->Write(&pBuffer[0], 1);
			}
			else
			{
				int iIndex = INT_MAX_LEN;
				char* pPointer;

				//Kiểm tra số âm.
				if (iNumber < 0)
				{
					gSynchConsole->Write(&cSign, 1);
					iNumber = -iNumber;
				}

				//Chuyển số sang chuỗi.
				while (iNumber > 0 && iIndex > 0)
				{
					--iIndex;
					pBuffer[iIndex] = (iNumber % 10) + '0';
					iNumber = iNumber / 10;
				}

				//Ghi ra console.
				pPointer = pBuffer + iIndex;
				gSynchConsole->Write(pPointer, INT_MAX_LEN - iIndex);

			}

			/*
			//CÁCH 02:
			sprintf(pBuffer, "%d", iNumber);
			gSynchConsole->Write(pBuffer, strlen(pBuffer));
			*/

			if (pBuffer != NULL)
			{
				delete[] pBuffer;
			}

			break;
		}
		//Đọc một ký tự.
		case SC_ReadChar:
		{
			DEBUG('a', "Read character from console.\n");
			/* NOTE: Trong trường hợp này ta sẽ trả về ký tự đầu tiên lấy được từ console.
			*/

			char ch = 0;
			//gSynchConsole->Read(&ch, 1);
			gSynchConsole->Read(&ch, 1);

			char Temp;

			while (gSynchConsole->Read(&Temp, 1) == 1 && Temp != '\n')
			{
				if (ch == 0)
				{
					ch = Temp;
					//++icbRead;
				}
			}

			machine->WriteRegister(2, ch);

			break;
		}
		//Xuất một ký tự
		case SC_PrintChar:
		{
			DEBUG('a', "Print character to console.\n");

			char ch = machine->ReadRegister(4);
			gSynchConsole->Write(&ch, 1);

			break;
		}
		//Đọc một chuỗi.
		case SC_ReadString:
		{
			DEBUG('a', "Read string from console.\n");

			//
			int iUserBufAddress = machine->ReadRegister(4);
			int iUserBufLen = machine->ReadRegister(5);
			char* pBuffer = new char[iUserBufLen];

			if (pBuffer != NULL)
			{
				int icbRead = 0;//gSynchConsole->Read(pBuffer, iUserBufLen - 1);
				char Temp;

				while (gSynchConsole->Read(&Temp, 1) == 1 && Temp != '\n')
				{
					//printf("%c,", Temp);
					if (icbRead < iUserBufLen - 1)
					{
						pBuffer[icbRead] = Temp;
						++icbRead;
					}
				}

				pBuffer[icbRead] = 0;
				//printf("%s\n", pBuffer);
				machine->System2User(iUserBufAddress, icbRead + 1, pBuffer);

				delete[] pBuffer;
			}

			break;
		}
		//Xuất một chuỗi.
		case SC_PrintString:
		{
			DEBUG('a', "Print string to console.\n");

			int iUserBufAddress = machine->ReadRegister(4);
			int iIndex = 0;
			char* pBuffer = machine->User2System(iUserBufAddress, 255);

			if (pBuffer != NULL)
			{
				gSynchConsole->Write(pBuffer, strlen(pBuffer));

				delete[] pBuffer;
			}

			break;
		}
		case SC_CreateFile:
		{
			int virtAddr;
			char* filename;
			DEBUG('a', "\n SC_Create call ...");
			DEBUG('a', "\n Reading virtual address of filename");
			virtAddr = machine->ReadRegister(4);
			DEBUG('a', "\n Reading filename.");

			// MaxFileLength = 32
			filename = machine->User2System(virtAddr, MaxFileLength + 1);
			if (filename == NULL)
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				machine->WriteRegister(2, -1); // Error
				delete filename;
				return;
			}
			DEBUG('a', "\n Finish reading filename.");
			if (!fileSystem->Create(filename, 0))
			{
				DEBUG('f', "Can not create file");
				printf("\n Error create file '%s'", filename);
				machine->WriteRegister(2, -1);
			}
			else// Success
			{
				DEBUG('f', "Create file successfully");
				machine->WriteRegister(2, 0);
			}
			delete filename;
			break;
		}
		case SC_Open:
		{
			int virtAddr;
			int type;
			virtAddr = machine->ReadRegister(4);
			type = machine->ReadRegister(5);
			char* buf;
			if (fileSystem->index > 10)
			{
				printf("full index tim cach xoa di\n");
				machine->WriteRegister(2, -1);
				break;
			}
			buf = machine->User2System(virtAddr, MaxFileLength + 1);
			if (strcmp(buf, "stdin") == 0)// console input
			{
				printf("console input\n");
				machine->WriteRegister(2, 0);
				delete buf;
				break;
			}
			if (strcmp(buf, "stdout") == 0)// console output
			{
				printf("console output\n");
				machine->WriteRegister(2, 1);
				delete buf;
				break;
			}
			if ((fileSystem->openlist[fileSystem->index] = fileSystem->Open(buf, type)) != NULL)
			{
				DEBUG('f', "Open file successfully");
				machine->WriteRegister(2, fileSystem->index - 1);
			}
			else
			{
				DEBUG('f', "Cannot open file");
				machine->WriteRegister(2, -1);
			};
			delete buf;
			break;
		}
		case SC_Read:
		{
			int virtAddr = machine->ReadRegister(4);
			int NumBuf = machine->ReadRegister(5);
			int m_index = machine->ReadRegister(6);
			int byteread;
			char* buf;

			// Check m_index
			if (m_index < 0 || m_index > 10)
			{
				machine->WriteRegister(2, -1);
				break;
			}
			// Check openlist[m_index]
			if (fileSystem->openlist[m_index] == NULL)
			{
				machine->WriteRegister(2, -1);
				break;
			}

			buf = machine->User2System(virtAddr, NumBuf);
			if (fileSystem->openlist[m_index]->type == 2)
			{
				int size = gSynchConsole->Read(buf, NumBuf);
				machine->System2User(virtAddr, size, buf);
				machine->WriteRegister(2, size);
				break;
			}

			byteread = fileSystem->openlist[m_index]->Read(buf, NumBuf);
			if (byteread > 0)
			{
				// Copy data from kernel to user space
				if (byteread == NumBuf)
				{
					machine->System2User(virtAddr, byteread, buf);
					machine->WriteRegister(2, -2);
				}
				else
				{
					machine->System2User(virtAddr, byteread, buf);
					machine->WriteRegister(2, byteread);
				}
			}
			else
			{
				machine->WriteRegister(2, -1);//Error
			}
			delete buf;
			break;
		}
		case SC_Write:
		{
			int bufAddr = machine->ReadRegister(4);
			int NumBuf = machine->ReadRegister(5);
			int m_index = machine->ReadRegister(6);
			int Oldpos;
			int Newpos;
			char* buf;
			// Check m_index
			if (m_index < 0 || m_index > 10)
			{

				machine->WriteRegister(2, -1);
				break;
			}
			// check openf[m_index]
			if (fileSystem->openlist[m_index] == NULL)
			{
				machine->WriteRegister(2, -1);
				break;
			}
			Oldpos = fileSystem->openlist[m_index]->GetCurrentPos();
			// type must equals '0'
			buf = machine->User2System(bufAddr, NumBuf);
			if (fileSystem->openlist[m_index]->type == 0 || fileSystem->openlist[m_index]->type == 3)//File read and write + console output
			{
				if ((fileSystem->openlist[m_index]->Write(buf, NumBuf)) > 0)
				{
					// Copy data from kernel to user space
					printf("%s", buf);
					Newpos = fileSystem->openlist[m_index]->GetCurrentPos();
					machine->WriteRegister(2, Newpos - Oldpos + 1);
				}
				else if (fileSystem->openlist[m_index]->type == 1);//File read-only
				{
					machine->WriteRegister(2, -1);
					delete buf;
					break;
				}
			}
			// Write data to console
			if (fileSystem->openlist[m_index]->type == 3)
			{
				int i = 0;
				printf("console ouput\n");
				while (buf[i] != 0 && buf[i] != '\n')
				{
					gSynchConsole->Write(buf + i, 1);
					i++;
				}
				buf[i] = '\n';
				gSynchConsole->Write(buf + i, 1);
				machine->WriteRegister(2, i - 1);
			}
			delete buf;
			break;
		}
		case SC_Close:
		{
			int m_index = machine->ReadRegister(4);
			if (fileSystem->openlist[m_index] == NULL)
				break;
			delete fileSystem->openlist[m_index];
			fileSystem->openlist[m_index] = NULL;
			break;
		}
		case SC_Seek:
		{
			int pos = machine->ReadRegister(4);
			int m_index = machine->ReadRegister(5);

			if (m_index < 0 || m_index > 10)
			{
				machine->WriteRegister(2, -1);
				break;
			}
			// check openlist[m_index]
			if (fileSystem->openlist[m_index] == NULL)
			{
				printf("Error, cannot seek\n");
				machine->WriteRegister(2, -1);
				break;
			}
			if (pos == -1)
				pos = fileSystem->openlist[m_index]->Length();
			if (pos > fileSystem->openlist[m_index]->Length() || pos < 0)
			{
				machine->WriteRegister(2, -1);
			}
			else
			{
				fileSystem->openlist[m_index]->Seek(pos);
				machine->WriteRegister(2, pos);
			}
			break;
		}
		case SC_Exec:
		{
			int name = machine->ReadRegister(4);
			if (name == 0) {
				machine->WriteRegister(2, -1);
				return;
			}
			/*load file name from user to kernel*/
			char s[FileNameMaxLen + 1] = { 0 };
			for (int j = 0; j < FileNameMaxLen; ++j) {
				int oneChar = 0;
				if (machine->ReadMem(name + j, 1, &oneChar) == FALSE) {
					machine->WriteRegister(2, -1);
					return;
				}
				if (oneChar == 0) break;
				s[j] = (char)oneChar;
			}
			/*execute process*/
			int ret = pTab->ExecUpdate(s);
			machine->WriteRegister(2, ret);
			break;
		}
		case SC_Join:
		{
			int id = machine->ReadRegister(4);
			/*join process*/
			int ret = pTab->JoinUpdate(id);
			machine->WriteRegister(2, ret);
			break;
		}
		case SC_Exit:
		{
			int exitCode = machine->ReadRegister(4);
			/*exit process*/
			int ret = pTab->ExitUpdate(exitCode);
			machine->WriteRegister(2, ret);
			break;
		}
		case SC_CreateSemaphore:
		{
			int name = machine->ReadRegister(4);
			int semval = machine->ReadRegister(5);
			if (name == 0 || semval < 0) {
				machine->WriteRegister(2, -1);
				return;
			}
			/*use SEM_MAXNAMESIZE to get name*/
			char s[SEM_MAXNAMESIZE] = { 0 };
			for (int i = 0; i < SEM_MAXNAMESIZE - 1; ++i) {
				int oneChar = 0;
				machine->ReadMem(name + i, 1, &oneChar);
				if (oneChar == 0) break;
				s[i] = (char)oneChar;
			}
			/*create semaphore*/
			int ret = semTab->Create(s, semval);
			machine->WriteRegister(2, ret);
			break;
		}
		case SC_Up:
		{
			int name = machine->ReadRegister(4);
			if (name == 0) {
				machine->WriteRegister(2, -1);
				return;
			}
			/*use SEM_MAXNAMESIZE to get name*/
			char s[SEM_MAXNAMESIZE] = { 0 };
			for (int i = 0; i < SEM_MAXNAMESIZE - 1; ++i) {
				int oneChar = 0;
				machine->ReadMem(name + i, 1, &oneChar);
				if (oneChar == 0) break;
				s[i] = (char)oneChar;
			}
			/*wait semaphore*/
			int ret = semTab->Wait(s);
			machine->WriteRegister(2, ret);
			break;
		}
		case SC_Down:
		{
			int name = machine->ReadRegister(4);
			if (name == 0) {
				machine->WriteRegister(2, -1);
				return;
			}
			/*use SEM_MAXNAMESIZE to get name*/
			char s[SEM_MAXNAMESIZE] = { 0 };
			for (int i = 0; i < SEM_MAXNAMESIZE - 1; ++i) {
				int oneChar = 0;
				machine->ReadMem(name + i, 1, &oneChar);
				if (oneChar == 0) break;
				s[i] = (char)oneChar;
			}
			/*signal semaphore*/
			int ret = semTab->Signal(s);
			machine->WriteRegister(2, ret);
			break;
		}

		}
		// Advance program counters.
		machine->registers[PrevPCReg] = machine->registers[PCReg];  // for debugging, in case we are jumping into lala-land
		machine->registers[PCReg] = machine->registers[NextPCReg];
		machine->registers[NextPCReg] += 4;
		break;
	case PageFaultException:
		// No valid translation found
		printf("No valid translation found %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case ReadOnlyException:
		// Write attempted to page marked 
		printf("Write attempted tp page marked %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case BusErrorException:
		// Translation resulted in an invalid physical address
		printf("Translaion resulted in an invalid physical address %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case AddressErrorException:
		// Unaligned reference or one that
		// was beyond the end of the
		// address space
		printf("Unaligned reference or one that was beyond the end of the address space %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case OverflowException:
		// Integer overflow in add or sub.
		printf("Integer overflow in add or sub %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case IllegalInstrException:
		// Unimplemented or reserved instr.
		printf("Unimplemented or reserved instr %d %d\n", which, type);
		ASSERT(FALSE);
		break;
	case NumExceptionTypes:
		ASSERT(FALSE);
		break;
	}

	//if ((which == SyscallException) && (type == SC_Halt)) {
	//DEBUG('a', "Shutdown, initiated by user program.\n");
	//interrupt->Halt();
	// } else {
	//printf("Unexpected user mode exception %d %d\n", which, type);
	//ASSERT(FALSE);
	// }
}
