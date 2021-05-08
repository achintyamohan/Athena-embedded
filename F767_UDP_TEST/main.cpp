#include "mbed.h"
#include "EthernetInterface.h"
 
const int PORT = 5005;
 
int main() {
    EthernetInterface eth;

    eth.set_network("169.254.174.143", "255.255.255.0", "169.254.1.1");
    eth.connect();
    
    UDPSocket sock;
    sock.open(&eth);
    sock.bind(PORT);

    SocketAddress *destAddr = new SocketAddress("169.254.174.110", PORT);

    
    //char out_buffer[] = "very important data";
    int *out_buffer = new int(69); 
    
    while (true) {
        printf("Sending...\n");
        printf("\n%d\n",sock.sendto(*destAddr, out_buffer, sizeof(out_buffer)));
        wait_us(1000000);
    }
}