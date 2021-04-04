#include "mbed.h"

CAN can1(PB_8, PB_9, 500000);
// CAN can2(PB_5, PB_13, 500000);
DigitalOut led1(PH_1);
DigitalOut led2(PC_15);

EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

char counter = 1;

void send() {
    // printf("send()\n");
    if(can1.write(CANMessage(1337, &counter, sizeof(counter)))){
        led1 = true;
        wait_us(10000);
    }
}
 
int main() {

     t.start(callback(&queue, &EventQueue::dispatch_forever));
     queue.call_every(50ms, send);

     CANMessage msg;

     while(1) {
         
         led1 = false;
         led2 = false;

         if(can1.read(msg)){
             if(msg.id == 69){
                printf("\n nice %d \n", msg.data[0]);
                led2 = true;
                wait_us(10000);
             }
         
         }

     }
}

 
