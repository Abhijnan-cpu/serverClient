
#include<iostream>
#include<winsock2.h>//networking applications
#include<Ws2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")//manually links the functions from ws2_32 library in windows



bool Initialize()//check if wsa can allocate resources
{
    WSADATA data;
    return WSAStartup(MAKEWORD(2,2),&data)==0;

    return 1;
}

void InteractWithClient(SOCKET clientSocket,vector<SOCKET>& clients)
{

    cout<<"client connected"<<endl;

    char buffer[4096];
    while(1)
    {
    int bytesrecvd= recv(clientSocket,buffer,sizeof(buffer),0);//blocking<<

    if(bytesrecvd<=0)//client has disconnected
    {
        cout<<"Client disconnected"<<endl;
        break;
    }

    string message(buffer,bytesrecvd);

    cout<<"Message from client : "<<message<<endl;
    
    for(auto client : clients)
    {
        if(client!=clientSocket)
        {
        send(client,message.c_str(),message.length(),0);
        }
    }
    }
    auto it=find(clients.begin(),clients.end(),clientSocket);//remove disconnected vector
    if(it !=clients.end())
    {
        clients.erase(it);
    }


    closesocket(clientSocket);
}

int main()
{
    if(!Initialize())
    {
        cout<<"Winsock initialization failed"<<endl;
        return 1;
    }
     
     SOCKET listenSocket=socket(AF_INET,SOCK_STREAM,0);

     if(listenSocket==INVALID_SOCKET)//check API
     {
        cout<<"Socket creation failed"<<endl;
        return 1;
     }
           
     //create serveraddr structure
     int port=12345;
     string serveraddress="127.0.0.1";
     sockaddr_in serveraddr;
     serveraddr.sin_family=AF_INET;//IPV4
     serveraddr.sin_port=htons(port);//host to n/w n/w stores in big endian format for n/w
     
    
    //convert the ip adress to bytes and writes it to the serveraddr struct address
    if(inet_pton(AF_INET,serveraddress.c_str(),&serveraddr.sin_addr)!=1)
    {
        cout<<"Setting adress structure failed"<<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    //bind ip and socket
    if(bind(listenSocket,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr))==SOCKET_ERROR)
    {
        cout<<"Bind error"<<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    
    cout<<endl<<endl;
    if(listen(listenSocket,SOMAXCONN)==SOCKET_ERROR)
    {
        cout<<"Listen Failed"<<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    cout<<"Server created succesfull"<<endl;
    cout<<"Server has started listening on port : "<<port<<endl;
    vector<SOCKET> clients;

    while(1)
    {
        //accept
    SOCKET clientSocket=accept(listenSocket,nullptr,nullptr);
    if(clientSocket==INVALID_SOCKET)
    {
        cout<<"Invalid client socket"<<endl;
        continue;
    }
    clients.push_back(clientSocket);
    thread t1(InteractWithClient,clientSocket, std::ref(clients));//new threads created when clients are connected
    t1.detach();//thread dies at the endof scope hence thread is dettached
    }


    closesocket(listenSocket);

    WSACleanup();//clean resource allocated by wsa
    cout<<"Server shutdown"<<endl;
    return 0;
    

}




