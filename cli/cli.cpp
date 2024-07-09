#include<winsock2.h>
#include<iostream>
#include<cstring>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

const int BUFFER_SIZE = 1024;//缓冲区大小
 
DWORD WINAPI recvMsgThread(LPVOID IpParameter);

int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2,2),&data);

    SOCKET clinsock=socket(AF_INET,SOCK_STREAM,0);

   
    sockaddr_in clinaddr;
    clinaddr.sin_family=AF_INET;
    clinaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    clinaddr.sin_port=htons(12345);
    //设置服务器的地址和端口信息，将其存储在sockaddr_in结构体对象中
    sockaddr_in sockaddr;
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    sockaddr.sin_port=htons(12345);

    //调用connect函数来连接到服务器。如果连接出现错误，会输出错误代码
    if(connect(clinsock,(SOCKADDR*)&sockaddr,sizeof(SOCKADDR))==SOCKET_ERROR)
    {
        cout << "error" << WSAGetLastError() << endl;
    }

    //创建一个线程来接收服务器发送的消息。线程函数是recvMsgThread，它通过参数传递了客户端套接字
    CloseHandle(CreateThread(NULL, 0, recvMsgThread, (LPVOID)&clinsock, 0, 0));

    //通过循环读取用户输入的消息，并发送给服务器。如果用户输入"quit"，则退出聊天室。
    while (1) 
	{
		char buf[BUFFER_SIZE] = { 0 };
		cin.getline(buf,sizeof(buf));
		if (strcmp(buf, "quit") == 0)//若输入“quit”，则退出聊天室
		{
			break;
		}
		send(clinsock, buf, sizeof(buf), 0);
	}
	closesocket(clinsock);
	WSACleanup();
	return 0;
}


DWORD WINAPI recvMsgThread(LPVOID IpParameter)
{
      SOCKET clinsock = *(SOCKET*)IpParameter;//函数首先将传入的IpParameter转换为SOCKET类型，并赋值给变量clinsock。

      while(1)
      {
        char buffer[BUFFER_SIZE]={0};//定义一个大小为BUFFER_SIZE的字符数组buffer，并将其所有元素初始化为0
        int nrecv=recv(clinsock,buffer,sizeof(buffer),0);//调用recv函数接收来自clinsock的数据，并将接收到的数据存储在buffer中
        if(recv>0)//如果接收到的字节数大于0，则将buffer中的内容输出到标准输出流（cout）。
        {
             cout<<buffer<<endl;
        }
        else
        {
            cout<<"don't connect"<<endl;
            break;
        }
      }
      return 0;
}
