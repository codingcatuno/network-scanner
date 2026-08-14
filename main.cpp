#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//confirm

using namespace std;

int main()
{
    // Initialize the Socket
    int clientsocket = socket(AF_INET, SOCK_STREAM, 0);

    // Check to see if the socket was created successfully
    if(clientsocket == -1){
        cout << "Failed to create socket." << endl;
        return -1;
    }else { 
    cout << "Socket created successfully." << endl <<
    "Socket descriptor: " << clientsocket << endl;
    }

    // Assign Destination address
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    // Establish Connection
    int result = connect(clientsocket, (sockaddr*)&address,sizeof(address));
    
    if (result == 0){
        cout << "Connection Successful!" << endl;
        return result;
    } else{
        cout << "Connection Failed" << endl;
        return result;
    }

    return 0;
}
