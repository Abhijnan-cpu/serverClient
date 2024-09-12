/*
initialize winsock
create socket
conneect to server
send/recv
close the socket
*/
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>
#include<string>

using namespace std;

#pragma comment(lib, "ws2_32.lib") //link winsock library

bool Initialize()
{
    WSADATA data;
    return WSAStartup(MAKEWORD(2,2),&data)==0;//(version2.2,data)

}

void SendMsg(SOCKET s)
{
    cout<<"Enter chat identifier name : "<<endl;
    string name;
    getline(cin,name);//takes spaces also
    string message;
    cout<<"You can start chatting :)"<<endl;
    while(1)
    {   cout<<">>";
        getline(cin,message);
        string msg=name+ " : " +message;
        int bytesent=send(s,msg.c_str(),msg.length(),0);
        if(bytesent==SOCKET_ERROR)
        {
            cout<<"error sending message"<<endl;
            break;
        }
        if(message=="quit/"){
            cout<<"Stopping the application"<<endl;
            break;
        }
    }

closesocket(s);


}


void RecieveMsg(SOCKET s)
{
    char buffer[4096];
    int recvlength;
    string msg="";
    while(1){
        recvlength=recv(s,buffer,sizeof(buffer),0);
        if(recvlength<=0)
        {
            cout<<"disconnected from the server"<<endl;
            break;
        }
        else
        {
            msg=string(buffer,recvlength);
            cout<<msg<<endl;
            cout<<">>";

        }
    }
    closesocket(s);
   
    
}

int main()
{
    if(!Initialize())
    {
        cout<<"Winsock Initialization failed"<<endl;

        return 1;
    }

    SOCKET s;
    s=socket(AF_INET,SOCK_STREAM,0);//(ipv4,reliable,tcp)

    if(s==INVALID_SOCKET)//returns -1
    {
        cout<<"Socket creation failed"<<endl;
        WSACleanup();
        return 1;
    }
    
    int port=12345;
    string serveraddress="127.0.0.1";//loopback adress
    sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);//host to n/w stores in big endian format
    inet_pton(AF_INET,serveraddress.c_str(),&(serveraddr.sin_addr));//inet presentation stores server adress in bits from string
    //.c_str() is c way to represent sstring const*char
    if(connect(s,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr))==SOCKET_ERROR)//connect takes sockaddr hence casting to sockaddr
    {
        cout<<"Not able to connect to server"<<endl;
        cout<<": "<<WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout<<"Successfully connected to server"<<endl;

    thread senderthread(SendMsg,s);
    thread recieverthread(RecieveMsg,s);

    senderthread.join();//waits for thread to finish and join main
    recieverthread.join();

    WSACleanup();
   
    return 0;
}