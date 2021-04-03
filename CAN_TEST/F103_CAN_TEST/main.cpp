#include "mbed.h"

CAN can1(PB_8, PB_9, 500000);

DigitalOut led1(PC_13);

EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

char counter = 0;

void send() {
    printf("send()\n");
    if(can1.write(CANMessage(69, &counter, sizeof(counter)))){
        //led1 = true;
        //wait_us(20000);
    }
}
 
int main() {

     t.start(callback(&queue, &EventQueue::dispatch_forever));
     queue.call_every(70ms, send);

     CANMessage msg;

     while(1) {
         
         led1 = true;

         if(can1.read(msg)){
             if(msg.id == 1337 && msg.data[0] == 1){
                led1 = false; 
                wait_us(20000);
                led1 = true;
             }

         }
         
     }
}

 
