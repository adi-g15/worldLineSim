#include "communication/interthread/messagequeue.hpp"

int main() {
        interthread::Communicator c1, c2;
        interthread::MessageQueue m(c1,c2);
}
