#include<iostream>
#include<Winsock2.h>//socketͷ�ļ�
#include<cstring>
using namespace std;

#pragma comment(lib,"ws2_32.lib");//socket��

const int WAIT_TIME=10;
const int buffer_size=1024;//�������Ĵ�С
const int max_link_num=10;//���ͻ�����������
SOCKET clinsock[max_link_num];//�ͻ����׽���Ϊ0���Ƿ�����
WSAEVENT clinevent[max_link_num];//�ͻ����¼� 0��Ϊ�����,�������ó����һ���ֵȴ�������һ���ֵ��ź�
SOCKADDR_IN clinaddr[max_link_num];//�ͻ��˵�ַ
int total;//Ŀǰ���ӵ�����

DWORD WINAPI servEventThread(LPVOID IpParameter);//...

int main()
{
    //��ʼ��socket��
    WSADATA data;//��ȡ�汾��Ϣ
    WSAStartup(MAKEWORD(2,2),&data);//MAKEWORD�����汾�����汾��

    //����socket����
    SOCKET servsock=socket(AF_INET,SOCK_STREAM,0);//����������ʽ�׽���->AF_INET��IP��ַ�壬��IPЭ��汾4��SOCK_STREAM��������ӵ���ʽ�׽��֡�0��ʹ�õ�Ĭ��Э��

    //����������ַ�����һ���ṹ����
    sockaddr_in servaddr;//sockaddr_in ��sockaddr����internat���׽��ֵĵ�ַ��ʽ
    servaddr.sin_family=AF_INET;//sin_familyΪЭ���ipv4
    servaddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");//����˵�ַ����Ϊ���ػػ���ַ,��IP��ַת��Ϊ32λ�����ֽ�
    servaddr.sin_port=htons(12345);//s���ö˿ں�,htons����˼���Ǳ����ֽ���ת�������ֽ���short���͵ĳ���

    //�󶨷���˵�socket�ʹ���õĵ�ַ
    bind(servsock,(SOCKADDR*)&servaddr,sizeof(servaddr));

    //���ͻ��˰�һ���¼������������տͻ��˷���������
    WSAEVENT servEvent=WSACreateEvent();
    WSAEventSelect(servsock,servEvent,FD_ALL_EVENTS);
    
    clinsock[0]=servsock;
    clinevent[0]=servEvent;

    //��������
    listen(servsock,10);//�ȴ����ӵ������г���


    CloseHandle(CreateThread(NULL, 0, servEventThread, (LPVOID)&servsock, 0, 0));//����һ���̣߳��������̵߳ľ���洢��һ����������С�����߳̽�ִ��`servEventThread`���������ҽ�`servsock`��Ϊ�������ݸ��ú�����`CreateThread`�����ķ���ֵ���̵߳ľ����ͨ��`CloseHandle`�������ر������������ͷ���Դ


    cout << "start communiate" << endl;
	connect;
    
    //�����ڶ�ȡ�û���������ݣ������䷢�͸����������ӵ��׽��֣�ʵ������Ϣ�Ĺ㲥���ܣ�
    while(1)
    { 
        //�����������ַ��������contentbuf��sendbuf�����ڴ洢�û���������ݺͷ��͸��׽��ֵ���Ϣ��
        char contentbuf[buffer_size]={0};
        char sendbuf[buffer_size]={0};
        //ʹ��cin.getline()������ȡ�û���������ݣ�������洢��contentbuf�����С�
        cin.getline(contentbuf,sizeof(contentbuf));
        //ʹ��sprintf()�������û���������ݼ���ǰ׺"[����С��]"����������洢��sendbuf�����С�
        sprintf(sendbuf,"[AI]%s", contentbuf);
        //ʹ��forѭ���������������ӵ��׽��֣�ʹ��send()������sendbuf�����е����ݷ��͸�ÿ���׽���
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
    SOCKET servsock=*(SOCKET*)IpParameter;//LPVOIDΪ��ָ�����ͣ���Ҫ��ת��SOCKET���������ã�����ʹ�ô����SOCKET
    while(1)
    {
         for(int i=0;i<total+1;i++)
        {
            //����һ���ͻ������ӣ�total==1��ѭ�����Σ������ͻ��˺ͷ����
			//��ÿһ���նˣ��ͻ��˺ͷ���ˣ����鿴�Ƿ����¼����ȴ�WAIT_TIME����
            int index=WSAWaitForMultipleEvents(1,&clinevent[i],false,WAIT_TIME,0);//�����¼�����������
            index-=WSA_WAIT_EVENT_0;//��ʱindexΪ�����¼����ն��±�
            //���������߳�ʱ�����������ն�
            if(index==WSA_WAIT_TIMEOUT||index==WSA_WAIT_FAILED)
            {
                continue;
            }
            //1��û���¼�������ִ�����²�����
            else if(index==0)
            {
                //1�����Ի�ȡ�ض��׽���cliSock[i]�Ϸ����������¼���������洢��networkEvents�ṹ������С�
                WSANETWORKEVENTS networkEvents;//����һ����ΪnetworkEvents��WSANETWORKEVENTS�ṹ�����
                WSAEnumNetworkEvents(clinsock[i],clinevent[i],&networkEvents);//����WSAEnumNetworkEvents���������ڻ�ȡ��ָ���׽���������������¼�
                
                //��������¼��а���FD_ACCEPT����ʾ���µ��������󣩣���ִ�����²���
                if(networkEvents.lNetworkEvents & FD_ACCEPT)//`lNetworkEvents` �ֶδ洢�˷����������¼�������
                {
                    //���FD_ACCEPT_BIT��Ӧ�Ĵ����룬�����Ϊ0����������ӳ��ִ������Ϣ����������һ��ѭ����
                    if(networkEvents.iErrorCode[FD_ACCEPT_BIT]!=0)//
                    {
                        cout<<"error connect"<<networkEvents.iErrorCode[FD_ACCEPT_BIT]<<endl;
                        continue;
                    }
                    //�����ǰ��������total����1С�������������max_link_num������ִ�����²�����
                    if(total+1<max_link_num)
                    {
                        //��ȡ�����ӵĵ�ַ���ȣ�addrlen������ʹ��accept���������µ��������󣬷���һ���µ��׽��֣�newsock��
                        int nextindex=total+1;
                        int addrlen=sizeof(SOCKADDR);
                        SOCKET newsock=accept(servsock,(SOCKADDR*)&clinaddr[nextindex],&addrlen);

                        // ����µ��׽��ֲ�����Ч�׽��֣�INVALID_SOCKET������ִ�����²�����
                        if(newsock!=INVALID_SOCKET)
                        {
                            //���µ��׽��֣�newsock�����浽clinsock�����е���һ��λ�ã�nextindex��
                            clinsock[nextindex]=newsock;
                            //����һ���µ��¼���newevent�������µ��׽��֣�newsock������¼�����������FD_CLOSE��FD_READ��FD_WRITE�¼�
                            WSAEVENT newevent=WSACreateEvent();
                            WSAEventSelect(clinsock[nextindex],newevent,FD_CLOSE | FD_READ | FD_WRITE);
                            //���µ��¼���newevent�����浽clinevent�����е���һ��λ�ã�nextindex��
                            clinevent[nextindex] = newevent;
                            //��������һ
							total++;

                            // ����һ����ӭ��Ϣ��buf���������ο͵�IP��ַ�������͸����������ӵ��׽��֡�
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
            
             //��������¼��а���FD_CLOSE����ʾ�����ӹرգ�����ִ�����²�����
             else if(networkEvents.lNetworkEvents & FD_CLOSE)
             {
                //���ٵ�ǰ��������total����ֵ��
                total--;
                cout<<"#"<<i<<endl;

                //�رն�Ӧ���׽��֣�clinsock[i]��
                closesocket(clinsock[i]);
                //�رն�Ӧ���¼���clinevent[i]����
                WSACloseEvent(clinevent[i]);
                
                //�������������Ϣ��clinsock��clinevent��clinaddr���飩��ǰ�ƶ�һ��λ�ã����ǵ����رյ�������Ϣ��
                for(int j=i;j<total;j++)
                {
                    clinsock[j]=clinsock[j+1];
                    clinevent[j]=clinevent[j+1];
                    clinaddr[j]=clinaddr[j+1];
                }
                
                //����һ���˳���Ϣ��buf���������˳���IP��ַ�������͸����������ӵ��׽���
                char buf[buffer_size]="[AI](IP:";
                strcat(buf,inet_ntoa(clinaddr[i].sin_addr));
                strcat(buf, ")quit");
                for (int j = 1; j <=total; j++)
				{
					send(clinsock[j], buf, sizeof(buf), 0);
 
				}

             }
          
             //��������¼��а���FD_READ����ʾ�����ݿɶ�ȡ������ִ�����²�����
             else if(networkEvents.lNetworkEvents & FD_READ)
             {
                //����һ����������buffer����һ�����ڴ洢��ʽ������Ϣ�Ļ�������buffer1����
                char buffer[buffer_size]={0};
                char buffer1[buffer_size]={0};
                
                //�������������ӵ��׽��֣���1��total��
                for(int i=1;i<=total;i++)
                {
                    //ʹ��recv���������׽����е����ݣ������ؽ��յ����ֽ�����nrecv��
                    int nrecv=recv(clinsock[i],buffer,sizeof(buffer),0);
                    //������յ����ֽ�������0����ִ�����²�����
                    if(nrecv>0)
                    {
                        //�����յ������ݸ�ʽ��Ϊ�������������ŵ���Ϣ��buffer1��
                        sprintf(buffer1,"[#%d]%s",i,buffer);
                        //�����ʽ�������Ϣ��buffer1��
                        cout<<buffer1<<endl;
                        //�������������ӵ��׽��֣�������ʽ�������Ϣ���͸�ÿ���׽��֡�
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
