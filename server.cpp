#include <bits/stdc++.h>
#include <thread>
#include <Winsock2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <mutex>
#pragma comment(lib,"ws2_32.lib");

using namespace std;

std::mutex mtx;

thread cli[31];

int i;

void meeting(int id，SOCKET s)
{
    char welc[]="连接成功"；
    int r;
    char messages[1024]={'\0'};
    int e=send(s,welc,strlen(welc),0);
    while(1)
    {
        r=recv(s,mesages,sizeof(messages),0);
        if(r<=0)
        {
            break;
        }
        //假装已经调用了ai
        char ans[]="答案";
        int se=send(s,ans,sizeof(ans),0);
        memset(messages,'\0',sizeof(messages));
    }
    std::lock_guard<std::mutex> lock(mtx);
    i=i-1;
    for(int j=id;j<=i;j++)
    {
        cli[j+1]=cli[j];
    }
    closesocket(s);
}

int main()
{
    WSADATA data;
    int a=WSAStartup(MAKEWORD(2,2),&data);
    if(a)
    {
        freopen("error.txt","w",stdout);
        cout<<"初始化错误，小问题"<<'\n';
        return 0;
    }
    SOKET sersock=soket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
    {
        freopen("error.txt","w",stdout);
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
        freopen("error.txt","w",stdout);
        cout<<"bind监听问题，小问题"<<'\n';
        return 0;
    }
    a=listen(sock,30);
    if(a==-1)
    {
        freopen("error.txt","w",stdout);
        cout<<"listen监听问题，小问题"<<'\n';
        return 0;
    }


    sockaddr addrcli[31];
    int len=sizeof(addrcli[1]);
    SOCKET sockcli[31];
    for(i=1;i<=30;i++)
    {
        sockcli[i]=accept(sersock,&addrcli[i],&len);
        if(sockcli==-1)
        {
        freopen("error.txt","w",stdout);
        cout<<i<<"连接问题，小问题"<<'\n';
        i--;
        continue;
        }
        else
        {
            cli[i]=thread(meeting,i,sockcli[i]);
        }
    }
    return 0;
}
