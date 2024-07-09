#include<iostream>
#include<Winsock2.h>//socket头文件
#include<cstring>
using namespace std;

#pragma comment(lib,"ws2_32.lib");//socket库

const int WAIT_TIME=10;
const int buffer_size=1024;//缓冲区的大小
const int max_link_num=10;//最大客户端链接数量
SOCKET clinsock[max_link_num];//客户端套接字为0的是服务器
WSAEVENT clinevent[max_link_num];//客户端事件 0号为服务端,它用于让程序的一部分等待来自另一部分的信号
SOCKADDR_IN clinaddr[max_link_num];//客户端地址
int total;//目前链接的总数

DWORD WINAPI servEventThread(LPVOID IpParameter);//...

int main()
{
    //初始化socket库
    WSADATA data;//获取版本信息
    WSAStartup(MAKEWORD(2,2),&data);//MAKEWORD（主版本，副版本）

    //创建socket（）
    SOCKET servsock=socket(AF_INET,SOCK_STREAM,0);//面对网络的流式套接字->AF_INET：IP地址族，用IP协议版本4。SOCK_STREAM：面对连接的流式套接字。0：使用的默认协议

    //将服务器地址打包进一个结构体中
    sockaddr_in servaddr;//sockaddr_in （sockaddr）是internat下套接字的地址形式
    servaddr.sin_family=AF_INET;//sin_family为协议簇ipv4
    servaddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//服务端地址设置为本地回环地址,将IP地址转化为32位网络字节
    servaddr.sin_port=htons(12345);//s设置端口号,htons：意思就是本机字节序转到网络字节序，short类型的长度

    //绑定服务端的socket和打包好的地址
    bind(servsock,(SOCKADDR*)&servaddr,sizeof(servaddr));

    //给客户端绑定一个事件对象，用来接收客户端发来的链接
    WSAEVENT servEvent=WSACreateEvent();
    WSAEventSelect(servsock,servEvent,FD_ALL_EVENTS);
    
    clinsock[0]=servsock;
    clinevent[0]=servEvent;

    //开启监听
    listen(servsock,10);//等待连接的最大队列长度


    CloseHandle(CreateThread(NULL, 0, servEventThread, (LPVOID)&servsock, 0, 0));//创建一个线程，并将该线程的句柄存储在一个句柄变量中。这个线程将执行`servEventThread`函数，并且将`servsock`作为参数传递给该函数。`CreateThread`函数的返回值是线程的句柄，通过`CloseHandle`函数来关闭这个句柄，以释放资源


    cout << "start communiate" << endl;
	connect;
    
    //（用于读取用户输入的内容，并将其发送给所有已连接的套接字，实现了消息的广播功能）
    while(1)
    { 
        //声明了两个字符数组变量contentbuf和sendbuf，用于存储用户输入的内容和发送给套接字的消息。
        char contentbuf[buffer_size]={0};
        char sendbuf[buffer_size]={0};
        //使用cin.getline()函数读取用户输入的内容，并将其存储在contentbuf数组中。
        cin.getline(contentbuf,sizeof(contentbuf));
        //使用sprintf()函数将用户输入的内容加上前缀"[智能小易]"，并将结果存储在sendbuf数组中。
        sprintf(sendbuf,"[AI]%s", contentbuf);
        //使用for循环遍历所有已连接的套接字，使用send()函数将sendbuf数组中的内容发送给每个套接字
        for(int j=0;j<=total;j++)
        {
            send(clinsock[j],sendbuf,sizeof(sendbuf),0);
        }
    }
    WSACleanup();
    return 0;
}

DWORD WINAPI servEventThread(LPVOID IpParameter)
{
    SOCKET servsock=*(SOCKET*)IpParameter;//LPVOID为空指针类型，需要先转成SOCKET类型再引用，即可使用传入的SOCKET
    while(1)
    {
         for(int i=0;i<total+1;i++)
        {
            //若有一个客户端链接，total==1，循环两次，包含客户端和服务端
			//对每一个终端（客户端和服务端），查看是否发生事件，等待WAIT_TIME毫秒
            int index=WSAWaitForMultipleEvents(1,&clinevent[i],false,WAIT_TIME,0);//返回事件发生的索引
            index-=WSA_WAIT_EVENT_0;//此时index为发生事件的终端下标
            //如果出错或者超时，即跳过此终端
            if(index==WSA_WAIT_TIMEOUT||index==WSA_WAIT_FAILED)
            {
                continue;
            }
            //1，没有事件发生，执行以下操作：
            else if(index==0)
            {
                //1，可以获取特定套接字cliSock[i]上发生的网络事件，并将其存储在networkEvents结构体变量中。
                WSANETWORKEVENTS networkEvents;//声明一个名为networkEvents的WSANETWORKEVENTS结构体变量
                WSAEnumNetworkEvents(clinsock[i],clinevent[i],&networkEvents);//调用WSAEnumNetworkEvents函数，用于获取与指定套接字相关联的网络事件
                
                //如果网络事件中包含FD_ACCEPT（表示有新的连接请求），则执行以下操作
                if(networkEvents.lNetworkEvents & FD_ACCEPT)//`lNetworkEvents` 字段存储了发生的网络事件的掩码
                {
                    //检查FD_ACCEPT_BIT对应的错误码，如果不为0，则输出连接出现错误的信息，并继续下一次循环。
                    if(networkEvents.iErrorCode[FD_ACCEPT_BIT]!=0)//
                    {
                        cout<<"error connect"<<networkEvents.iErrorCode[FD_ACCEPT_BIT]<<endl;
                        continue;
                    }
                    //如果当前连接数（total）加1小于最大连接数（max_link_num），则执行以下操作：
                    if(total+1<max_link_num)
                    {
                        //获取新连接的地址长度（addrlen），并使用accept函数接受新的连接请求，返回一个新的套接字（newsock）
                        int nextindex=total+1;
                        int addrlen=sizeof(SOCKADDR);
                        SOCKET newsock=accept(servsock,(SOCKADDR*)&clinaddr[nextindex],&addrlen);

                        // 如果新的套接字不是无效套接字（INVALID_SOCKET），则执行以下操作：
                        if(newsock!=INVALID_SOCKET)
                        {
                            //将新的套接字（newsock）保存到clinsock数组中的下一个位置（nextindex）
                            clinsock[nextindex]=newsock;
                            //创建一个新的事件（newevent）并将新的套接字（newsock）与该事件关联，监听FD_CLOSE、FD_READ和FD_WRITE事件
                            WSAEVENT newevent=WSACreateEvent();
                            WSAEventSelect(clinsock[nextindex],newevent,FD_CLOSE | FD_READ | FD_WRITE);
                            //将新的事件（newevent）保存到clinevent数组中的下一个位置（nextindex）
                            clinevent[nextindex] = newevent;
                            //连接数加一
							total++;

                            // 构造一个欢迎消息（buf），包含游客的IP地址，并发送给所有已连接的套接字。
                            char buf[buffer_size] = "[AI]welcome IP:";
							strcat(buf, inet_ntoa(clinaddr[nextindex].sin_addr));
							strcat(buf, ")had came");
							for (int j = i; j <=total; j++)
							{
								
								send(clinsock[j], buf, sizeof(buf),0);
								
							}
                        }
                   }
                } 
            
             //如果网络事件中包含FD_CLOSE（表示有连接关闭），则执行以下操作：
             else if(networkEvents.lNetworkEvents & FD_CLOSE)
             {
                //减少当前连接数（total）的值。
                total--;
                cout<<"#"<<i<<endl;

                //关闭对应的套接字（clinsock[i]）
                closesocket(clinsock[i]);
                //关闭对应的事件（clinevent[i]）。
                WSACloseEvent(clinevent[i]);
                
                //将后面的连接信息（clinsock、clinevent和clinaddr数组）向前移动一个位置，覆盖掉被关闭的连接信息。
                for(int j=i;j<total;j++)
                {
                    clinsock[j]=clinsock[j+1];
                    clinevent[j]=clinevent[j+1];
                    clinaddr[j]=clinaddr[j+1];
                }
                
                //构造一个退出消息（buf），包含退出的IP地址，并发送给所有已连接的套接字
                char buf[buffer_size]="[AI](IP:";
                strcat(buf,inet_ntoa(clinaddr[i].sin_addr));
                strcat(buf, ")quit");
                for (int j = 1; j <=total; j++)
				{
					send(clinsock[j], buf, sizeof(buf), 0);
 
				}

             }
          
             //如果网络事件中包含FD_READ（表示有数据可读取），则执行以下操作：
             else if(networkEvents.lNetworkEvents & FD_READ)
             {
                //创建一个缓冲区（buffer）和一个用于存储格式化后消息的缓冲区（buffer1）。
                char buffer[buffer_size]={0};
                char buffer1[buffer_size]={0};
                
                //遍历所有已连接的套接字（从1到total）
                for(int i=1;i<=total;i++)
                {
                    //使用recv函数接收套接字中的数据，并返回接收到的字节数（nrecv）
                    int nrecv=recv(clinsock[i],buffer,sizeof(buffer),0);
                    //如果接收到的字节数大于0，则执行以下操作：
                    if(nrecv>0)
                    {
                        //将接收到的数据格式化为带有连接索引号的消息（buffer1）
                        sprintf(buffer1,"[#%d]%s",i,buffer);
                        //输出格式化后的消息（buffer1）
                        cout<<buffer1<<endl;
                        //遍历所有已连接的套接字，并将格式化后的消息发送给每个套接字。
                        for(int k=0;k<total;k++)
                        {
                            send(clinsock[k],buffer1,sizeof(buffer1),0);
                        }

                     }

                 }
              }
            }
        } 
    }
  return 0;
}
