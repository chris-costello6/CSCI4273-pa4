#include "PerProtocol.h"
#include <iostream>

using namespace std;

int main()
{
    PerProtocol host_a("32002", "32001");
    PerProtocol host_b("32001", "32002");

    char* text = "This is a test";
    Message* msg = new Message(text, 14);

    Message* msg2 = new Message(text, 14);

    Message* msg3 = new Message(text, 14);

    Message* msg4 = new Message(text, 14);

    char test[1024];
    msg->msgFlat(test);
    cout << "Testing created message: " << test << endl;

    host_a.msg_send(msg, 8);
    host_b.msg_send(msg2, 7);
    host_a.msg_send(msg3, 6);
    host_b.msg_send(msg4, 5);

    while(1){
        continue;
    }
}