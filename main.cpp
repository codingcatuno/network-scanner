#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <chrono>

int main()
{

    // Collect port range and address
    std::string ipadd = "127.0.0.1";
    int startport = 0;
    int endport = 1;
    // Init var
    std::vector<int> openlist;
    int openamount = 0;


    std::cout << "Starting port: ";
    std::cin >> startport;
    std::cout << std::endl;
    std::cout << "Ending port: ";
    std::cin >> endport;
    std::cout << std::endl;

    // Validate start and end port
    if (startport < 1 || endport > 65535 || startport > endport){
        std::cout << "Invalid Start or End port" << std::endl;
        return -1;
    }

    std::cout << "Scanning: " << ipadd << " ports " << startport << "-" << endport << std::endl;

    // Scanner timer start point (turn on for scan time)
    auto start = std::chrono::steady_clock::now();

    // Loop of start to end port scan
    for (int i = startport; i <= endport; i++)
    {
        // Initialize the Sockets
        int clientsocket = socket(AF_INET, SOCK_STREAM, 0);

        // Check to see if the socket was created successfully
        if (clientsocket == -1)
        {
            std::cout << "Failed to create socket." << std::endl;
            return -1;
        }

        // Assign Destination address
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = htons(i);
        inet_pton(AF_INET, ipadd.c_str(), &address.sin_addr);

        // Establish Connection
        int result = connect(clientsocket, (sockaddr *)&address, sizeof(address));

        // Confirm Connection
        if (result == 0)
        {
            std::cout << i << " OPEN" << std::endl;
            openlist.push_back(i);
            openamount++;
        }
        else
        {
            std::cout << i << " CLOSED" << std::endl;
        }

        // Close Connection
        int closing = close(clientsocket);

        // Confirm Connection Close
        if (closing != 0)
        {
            std::cout << "Failed to Close" << std::endl;
        }
    }
    
    // Scanner timer end point (turn on for scan time)
    auto end = std::chrono::steady_clock::now();

    auto elapsed = end - start;
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

    // Scan Summary
    std::cout << std::endl << "Scan complete." << std::endl
    << "Ports scanned: " << endport - startport << std::endl
    << "Open ports: " << openamount << std::endl << std::endl
    << "Open: ";
    
    for (int i : openlist){
        std::cout << i << std::endl;
    }

    std::cout << "Time Elapsed: " << milli.count() << " ms" << std::endl;

    return 0;
}
