#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include <queue>
#include <mutex>

int main()
{

    // Collect port range and address
    std::string ipadd = "127.0.0.1";
    int startport = 0;
    int endport = 1;
    // Init var
    std::vector<int> openlist;
    int openamount = 0;
    std::mutex queuemutex;
    std::mutex resultmutex;
    std::queue<int> portlist;
    std::vector<std::thread> workers;
    int workercount = 4;


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

    // Push ports into queue
    for (int i = startport; i <= endport;i++){
        portlist.push(i);
    }

    ///////////////////////////////////////////////
    // Scanner timer start point (turn on for scan time)
    auto start = std::chrono::steady_clock::now();
    ///////////////////////////////////////////////
    
    // Create Thread
    for (int i = 0; i < workercount; i++)
    {
        workers.emplace_back([&]()
                             {

    // Loop of start to end port scan
    while (true)
    {
        int port;

        {
            // Mutex Lock Guard
            const std::lock_guard<std::mutex> lock(queuemutex);
            
            // Confirm valid portlist
            if (portlist.empty())
            {
                return;
            }

            port = portlist.front();
            portlist.pop();
        }

        // Initialize the Sockets
        int clientsocket = socket(AF_INET, SOCK_STREAM, 0);

        // Check to see if the socket was created successfully
        if (clientsocket == -1)
        {
            std::cout << "Failed to create socket." << std::endl;
            return;
        }

        // Assign Destination address
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_pton(AF_INET, ipadd.c_str(), &address.sin_addr);

        // Establish Connection
        int result = connect(clientsocket, (sockaddr *)&address, sizeof(address));

        // Confirm Connection
        if (result == 0)
        {
            std::cout << port << " OPEN" << std::endl;
            {
                // Result lock guard
                const std::lock_guard<std::mutex> lock(resultmutex);

                openlist.push_back(port);
                openamount++;
            }
        }

        // Close Connection
        int closing = close(clientsocket);

        // Confirm Connection Close
        if (closing != 0)
        {
            std::cout << "Failed to Close" << std::endl;
        }
    } });
    }

    // Join all threads
    for (std::thread& worker : workers){
        worker.join();
    }

    // Scanner timer end point (turn on for scan time)
    auto end = std::chrono::steady_clock::now();

    auto elapsed = end - start;
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

    // Scan Summary
    std::cout << std::endl << "Scan complete." << std::endl
    << "Ports scanned: " << endport - startport << std::endl
    << "Open ports: " << openamount << std::endl << std::endl
    << "Open: "; for (int i : openlist){std::cout << i << std::endl;}

    std::cout << "Time Elapsed: " << milli.count() << " ms" << std::endl;

    return 0;
}
