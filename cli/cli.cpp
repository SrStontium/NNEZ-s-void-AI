#include<winsock2.h>
#include<iostream>
#include<cstring>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

const int BUFFER_SIZE = 1024;//��������С
 
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
    //���÷������ĵ�ַ�Ͷ˿���Ϣ������洢��sockaddr_in�ṹ�������
    sockaddr_in sockaddr;
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    sockaddr.sin_port=htons(12345);

    //����connect���������ӵ���������������ӳ��ִ��󣬻�����������
    if(connect(clinsock,(SOCKADDR*)&sockaddr,sizeof(SOCKADDR))==SOCKET_ERROR)
    {
        cout << "error" << WSAGetLastError() << endl;
    }

    //����һ���߳������շ��������͵���Ϣ���̺߳�����recvMsgThread����ͨ�����������˿ͻ����׽���
    CloseHandle(CreateThread(NULL, 0, recvMsgThread, (LPVOID)&clinsock, 0, 0));

    //ͨ��ѭ����ȡ�û��������Ϣ�������͸�������������û�����"quit"�����˳������ҡ�
    while (1) 
	{
		char buf[BUFFER_SIZE] = { 0 };
		cin.getline(buf,sizeof(buf));
		if (strcmp(buf, "quit") == 0)//�����롰quit�������˳�������
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
      SOCKET clinsock = *(SOCKET*)IpParameter;//�������Ƚ������IpParameterת��ΪSOCKET���ͣ�����ֵ������clinsock��

      while(1)
      {
        char buffer[BUFFER_SIZE]={0};//����һ����СΪBUFFER_SIZE���ַ�����buffer������������Ԫ�س�ʼ��Ϊ0
        int nrecv=recv(clinsock,buffer,sizeof(buffer),0);//����recv������������clinsock�����ݣ��������յ������ݴ洢��buffer��
        if(recv>0)//������յ����ֽ�������0����buffer�е������������׼�������cout����
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
