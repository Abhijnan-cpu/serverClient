#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<vector>



using namespace std;

struct ThreadArgs
{
    vector<int>&clients1;
    int clientsocket;
    
    ThreadArgs(vector<int>& clients, int socket)
            : clients1(clients), clientsocket(socket)
    {
        //refrences initialized using initializer list
    }
};


void* InteractWithClient(void* arg)
{
    ThreadArgs* args =static_cast<ThreadArgs*>(arg);
    cout<<"Client connected"<<endl;
    char buffer[4096];
    while(1)
    {
    int bytesrecvd= recv(args->clientsocket,buffer,sizeof(buffer),0);//blocking<<

    if(bytesrecvd<=0)//client has disconnected
    {
        cout<<"Client disconnected"<<endl;
        break;
    }

    string message(buffer,bytesrecvd);

    cout<<"Message from client : "<<message<<endl;
        int i=0;
        for(int i=0;i<(args->clients1).size();i++)//range based loop
    {
        if(args->clients1[i]!=args->clientsocket)
        {
        send(args->clients1[i],message.c_str(),message.length(),0);
        }
    }
    }
    vector<int>::iterator it=find((args->clients1).begin(),(args->clients1).end(),args->clientsocket);//remove disconnected clientsocket
    if(it !=(args->clients1).end())
    {
        (args->clients1).erase(it);
    }


    close(args->clientsocket);
    delete args;
    pthread_exit(NULL);
}

int main()
{
     
     int listenSocket=socket(AF_INET,SOCK_STREAM,0);//create socket

     if(listenSocket==-1)
     {
        cout<<"Socket creation failed"<<endl;
        return 1;
     }
           
     //create serveraddr structure
     int port=12345;
     string serveraddress="127.0.0.1";
     sockaddr_in serveraddr;
     serveraddr.sin_family=AF_INET;//IPV4
     serveraddr.sin_port=htons(port);//host to n/w  stores in big endian format
     
    //inet presentation converts string adress to binary and .c_str() because func expects const *char;
    if(inet_pton(AF_INET,serveraddress.c_str(),&serveraddr.sin_addr)!=1)
    {
        cout<<"Setting adress structure failed"<<endl;
        close(listenSocket);
        return 1;
    }

    //bind the socket with ip
    if(bind(listenSocket,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr))==-1)
    {
        cout<<"Bind error"<<endl;
        close(listenSocket);
        return 1;
    }
    
    cout<<endl<<endl;
    if(listen(listenSocket,SOMAXCONN)==-1)
    {
        cout<<"Listen Failed"<<endl;
        close(listenSocket);
        return 1;
    }
    cout<<"Server created succesfull"<<endl;
    cout<<"Server has started listening on port : "<<port<<endl;
    vector<int> clients;
    pthread_t t1;
    
    while(1)
    {
        //accept
    int clientSocket=accept(listenSocket,nullptr,nullptr);
    if(clientSocket==-1)
    {
        cout<<"Invalid client socket"<<endl;
        continue;
    }
        ThreadArgs* args=new ThreadArgs(clients,clientSocket);
        clients.push_back(clientSocket);
        pthread_create(&t1,NULL,InteractWithClient,args);
    //thread t1(InteractWithClient,clientSocket, std::ref(clients));//new threads created when clients are connected
        pthread_detach(t1);//thread dies at the endof scope hence thread is dettached
        args=NULL;
    }


    close(listenSocket);
    cout<<"Server shutdown"<<endl;
    return 0;
    

}




