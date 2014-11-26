#include <iostream>
#include <stdio.h>
#include "ppp.h"
#include "message.h"

using namespace std;

int main(){
    PPP host_a("32002", 32001);
    PPP host_b("32001", 32002);

    char* text = "This is a test";
    Message* msg = new Message(text, 14);
    host_a.msg_send(msg, 8);

    while(1){
        continue;
    }
}