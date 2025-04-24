
#include <iostream>
#include <thread>
#include <chrono>
#include "Peer.hpp"

int main() {
    Peer peer("input1.h264");
    peer.createOfferAndSend();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    peer.sendData();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}