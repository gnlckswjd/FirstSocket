#include <iostream>
#include <cstdio>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")



const char* IP_Adrress = "10.55.47.16";
const int Port = 46159;
const int Size = 1024;

int main()
{
	//소켓 초기화 할 때 값들
	WSADATA Wsa;
	WSAStartup(MAKEWORD(2, 2), &Wsa);

	SOCKET Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//주소정보
	SOCKADDR_IN Address = {};
	Address.sin_family = AF_INET;
	Address.sin_port = htons(Port);
	Address.sin_addr.s_addr = inet_addr(IP_Adrress);

	connect(Socket, (SOCKADDR*)&Address, sizeof(Address));
	
	//전송
	std::cout << "보낼 메시지를 입력해 주세요. : ";
	char* Msg = new char[99];
	std::cin >> Msg;
	send(Socket, Msg, strlen(Msg), 0);
	
	//수신
	char Buffer[Size] = {};
	recv(Socket, Buffer, Size, 0);
	std::cout << "내가 받은 메세지: " << Buffer << std::endl;;
	//종료
	closesocket(Socket);
	int a;
	std::cin >> a;


	delete[] Msg;
	WSACleanup();
}