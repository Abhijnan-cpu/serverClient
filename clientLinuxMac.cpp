#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<iostream>
#include<unistd.h>
#include<pthread.h>

using namespace std;


void *SendMsg(void *arg)
{
    int s=*((int*)arg);//convert argument to socket identifier
    cout<<"Enter chat identifier name : "<<endl;
    string name;
    getline(cin,name);//takes space also
    string message;
    cout<<"You can start chatting :)"<<endl;
    while(1)
    {   getline(cin,message);
        string msg=name+ " : " +message;
        int bytesent=send(s,msg.c_str(),msg.length(),0);
        if(bytesent==-1)
        {
            cout<<"Error sending message"<<endl;
            break;
        }
        if(message=="quit/"){
            cout<<"Stopping the application"<<endl;
            break;
        }
    }

close(s);
    pthread_exit(NULL);


}


void *RecieveMsg(void *arg)
{
    int s=*((int*)arg);
    char buffer[4096];
    int recvlength;
    string msg="";
    while(1){
        recvlength=recv(s,buffer,sizeof(buffer),0);
        if(recvlength<=0)
        {
            cout<<"Disconnected from the server...Enter something to stop applicatioin"<<endl;
            break;
        }
        else
        {
            msg=string(buffer,recvlength);
            cout<<msg<<endl;
        }
    }
    close(s);
    pthread_exit(NULL);
   
    
}

int main()
{

    int s=socket(AF_INET,SOCK_STREAM,0);//create socket IPv4,reliable,tdp;

    if(s==-1)//sockerror
    {
        cout<<"Socket creation failed"<<endl;
        return 1;
    }
    
    int port=12345;
    string serveraddress="127.0.0.1";//server ip //use 127.0.0.1 as loopback
    struct sockaddr_in serveraddr;//create ipv4sockadress
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(port);//host to n/w in big endian form
    inet_pton(AF_INET,serveraddress.c_str(),&(serveraddr.sin_addr));//inet presentation converts string adress to byes

    if(connect(s,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr))==-1)///connects to the socket with the respecttive adress
    {
        cout<<"Not able to connect to server"<<endl;
        close(s);
        return 1;
    }

    cout<<"Successfully connected to server"<<endl;
    pthread_t t1,t2;//create two threads
    pthread_create(&t1,NULL,SendMsg,&s);//tid,null,func,func arguement
    pthread_create(&t2,NULL,RecieveMsg,&s);

    pthread_join(t1,NULL);//waits for thread to finish and join main
    pthread_join(t2,NULL);
    
   
    return 0;
}
