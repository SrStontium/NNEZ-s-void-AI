#include <bits/stdc++.h>
#include <thread>
#include <Winsock2.h>
#pragma comment(lib,"ws2_32.lib");

using namespace std;

thread cli[31];

void meeting(int id)
{

}

int main()
{
    WSADATA data;
    int a=WSAStartup(MAKEWORD(2,2),&data);
    if(a)
    {
        freopen("erro.txt","w",stdout);
        cout<<"初始化错误，小问题"<<'\n';
        return 0;
    }
    SOKET sersock=soket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
    {
        freopen("erro.txt","w",stdout);
        cout<<"创建套接字错误，小问题"<<'\n';
        return 0;
    }
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=
    addr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
    a=bind(sersock,(sockaddr*)&addr,sizeof(addr));
    if(a==-1)
    {
        freopen("erro.txt","w",stdout);
        cout<<"bind监听问题，小问题"<<'\n';
        return 0;
    }
    a=listen(sock,30);
    if(a==-1)
    {
        freopen("erro.txt","w",stdout);
        cout<<"listen监听问题，小问题"<<'\n';
        return 0;
    }


    sockaddr addrcli[31];
    int len=sizeof(addrcli[1]);
    SOCKET sockcli[31];
    for(int i=1;i<=30;i++)
    {
        sockcli[i]=accept(sersock,&addrcli[i],&len);
        if(sockcli==-1)
        {
        freopen("erro.txt","w",stdout);
        cout<<"连接问题，小问题"<<'\n';
        return 0;
        }
        else
        {
            cli[i]=thread(meeting);
        }

    }
    return 0;
}