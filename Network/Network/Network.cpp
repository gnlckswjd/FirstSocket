#include <iostream>
#include <cstdio>
#include <WinSock2.h>
#include <thread>
#include <vector>

#pragma comment(lib,"ws2_32")

// 학번 뒷자리에서 따온 포트
const int Port = 46159;
const int Size = 1024;
void SocketThread(SOCKET Client)
{
 

    char Buffer[Size] = {};


    // 클라이언트가 정보를 보낼 때까지 바쁜 대기
    recv(Client, Buffer, Size, 0);
    //클라이언트에게 받은 데이터를 출력
    std::cout << "Recieve Message : " << Buffer << std::endl;

    //클라이언트에게 데이터 전송
    char Msg[] = "서버에서 보낸 메시지";
    send(Client, Msg, strlen(Msg), 0);
    std::cout << "클라이언트와 연결 해제\n";
    //다 쓴 소켓 닫음
    closesocket(Client);
          
}

int main()
{


    //소켓 초기화 할때 값들
    WSADATA Wsa;
    // MAKEWORD가 실수에서 정수로 변환
    WSAStartup(MAKEWORD(2, 2), &Wsa);

    //변수 순서대로 ipv4, socket stream , tcp
    SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == -1)
    {
        std::cout << "Socket is not exist\n";
        return 0;
    }

    // 주소 정보
    SOCKADDR_IN Address = {};
    //AF는 Address Family를 뜻하며 AF_INET 소켓은 stream 유형일 때 전송 프로토콜에 TCP를 쓴다. 비연결성은 UDP 
    memset(&Address, 0x00, sizeof(Address));
    Address.sin_family = AF_INET;
    // 위에서 정한 포트 번호 사용, 비트 저장 방식은 big endian 이다.
    // htons는  short Int 데이터를 네트워크 바이트 순서로 변환
    Address.sin_port = htons(Port);
    //변수는 디폴트 address 를 뜻함
    //htonl 은 long Int를 네트워크 바이트 순서로 변환
    Address.sin_addr.s_addr = htonl(INADDR_ANY);

    // null 체크
    if (ListenSocket == NULL)
    {
        return 0;
    }

    //소켓에 주소 바인딩 및 상태 : 대기
    //소켓에 주소 할당,
    auto IsBind = bind(ListenSocket, (SOCKADDR*)(&Address), sizeof(Address));
    //바인드 실패시 SOCKET_ERROR(-1) 리턴
    if (IsBind == -1)
    {
        std::cout << "Bind Error \n";
        return 0;
    } 

    //Listen!
        // SOMAXCONN socket max connection 줄임말
    listen(ListenSocket, SOMAXCONN);

    // 윈도우는 fork가 없어서 Thread 로 동일한 기능 구현
    std::vector<std::thread> Threads;
    Threads.resize(999);
    for (auto& Thread: Threads)
    {
        

        SOCKADDR_IN ClientAddress = {};
        int ClientSize = sizeof(ClientAddress);
        // 크기를 포인터로 받기 때문에 크기 변수 따로 설정
        std::cout << "새로운 클라이언트 요청 대기...\n";
    // 클라이언트 요청 올 때 까지 바쁜 대기
        SOCKET Client;
        Client = accept(ListenSocket, (SOCKADDR*)&ClientAddress, &ClientSize);

        if (Client == -1)
        {
            std::cout << "Client Socket Error\n";
            closesocket(Client);
            return 0;
        }
        std::cout << "새로운 클라이언트 연결\n";
        Thread = std::thread(&SocketThread, Client);
        //Thread.join();
        //closesocket(Client);
    }
    
    for (auto& Thread : Threads)
    {
        Thread.join();
    }

    //소켓을 닫는 함수
    //closesocket(Client);
    closesocket(ListenSocket);

    //소켓 사용 끝을 알려주는 함수
    WSACleanup();
    
}
