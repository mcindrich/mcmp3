#include <iostream>
#include <mcmp3.h>

int main() {
    mp::Server server;
    bool err = server.init("192.168.5.10", 1213);
    if(err) {
        std::cout << "Unable to initialize the server...\nClosing the program...\n";
    } else {
        server.run();
    }
    return 0;
}