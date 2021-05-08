#include "mbed.h"
#include "EthernetInterface.h"
#include <cmath>

const int PORT = 5005;

CAN can1(PD_0, PD_1, 1000000);
EthernetInterface eth;

void sendDataMessage(char const *data, unsigned int commandID, unsigned int nodeID) {
    CANMessage msgToSend;
    msgToSend.id = ((nodeID & 63) << 5) + (commandID & 31);
    memcpy(msgToSend.data, data, sizeof(*data));    

    
    if(can1.write(msgToSend)){
        printf("CAN sent data to %u\n", msgToSend.id);
    }
    else {
        printf("CAN send error\n");
    }

}

void sendRemoteFrame(unsigned int commandID, unsigned int nodeID){
    
    unsigned int id = ((nodeID & 63) << 5) + (commandID & 31);
    CANMessage msgToSend(id);       // this constructor creates a remote frame

    if(can1.write(msgToSend)){
        //printf("CAN sent remote frame to %u\n", msgToSend.id);
    }
    else {
        //printf("CAN send error\n");
    }

}


int main()
{

    //UDP stuff    
    eth.set_network("169.254.174.143", "255.255.0.0", "169.254.187.33");
    eth.connect();
    
    UDPSocket sock;
    sock.open(&eth);
    sock.bind(PORT);

    SocketAddress *destAddr = new SocketAddress("169.254.187.33", 5005);

    int input, heartbeat = 420;

    //printf("\nUDP heartbeat send log- %d\n", sock.sendto(*destAddr, &heartbeat, sizeof(heartbeat)));
    sock.sendto(*destAddr, &heartbeat, sizeof(heartbeat));
    
    //printf("UDP input receive log- %d\n", sock.recvfrom(destAddr, &input, sizeof(int)));
    sock.recvfrom(destAddr, &input, sizeof(int));
    

    if(input != 10){
        exit(-10);
    }

    //CAN stuff
    float voltage = 0;

    sendRemoteFrame(0x017, 2);   //0x017 reads voltage

    while(true) {

        
        CANMessage msg;

        if(can1.read(msg)){
             //printf("%u\n", msg.id);
             memcpy(&voltage, msg.data, sizeof(voltage));
             //printf("CAN read result %f\n", voltage);
             //wait_us(100000);

             break;

        }
        else {
            //printf("CAN read error\n");
            continue;
            //wait_us(100000);
        }
        //wait_us(500000);
    }

    //printf("UDP data send log-%d\n", sock.sendto(*destAddr, &voltage, sizeof(voltage)));
    sock.sendto(*destAddr, &voltage, sizeof(voltage));
    wait_us(100000);

    sock.close();
    eth.disconnect();

}