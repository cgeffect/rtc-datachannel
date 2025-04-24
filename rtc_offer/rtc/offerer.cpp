/**
 * Copyright (c) 2019 Paul-Louis Ageneau
 * Copyright (c) 2019 Murat Dogan
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "offerer.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace std::chrono_literals;
using std::shared_ptr;
using std::weak_ptr;
template <class T>
weak_ptr<T> make_weak_ptr(shared_ptr<T> ptr) {
    return ptr;
}

offerer::offerer(/* args */) {
}

void offerer::startOffer() {
    rtc::InitLogger(rtc::LogLevel::Warning);

    rtc::Configuration config;
    // config.iceServers.emplace_back("stun.l.google.com:19302");

    pc = std::make_shared<rtc::PeerConnection>(config);

    // 设置 Offer 创建回调
    pc->onLocalDescription([this](rtc::Description description) {
        std::cout << "Local Description (Paste this to the other peer):" << std::endl;
        std::string offerType = description.typeString();
        std::string offerSdp = std::string(description);
        std::cout << offerType << std::endl;
        std::cout << offerSdp << std::endl;
        m_sdpCallback(offerSdp);
    });

    // 设置 ICE 候选回调
    pc->onLocalCandidate([this](rtc::Candidate candidate) {
        std::cout << "Local Candidate (Paste this to the other peer after the local description):"
                  << std::endl;
        std::cout << std::string(candidate) << std::endl
                  << std::endl;
        m_candidateCallback(std::string(candidate));
    });

    // 设置连接状态回调
    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "[State: " << state << "]" << std::endl;
    });

    pc->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        std::cout << "[Gathering State: " << state << "]" << std::endl;
        printConnectionInfo();
    });
        
    // Create a DataChannel, 创建数据通道之后才会生成自己的sdp
    dc = pc->createDataChannel("test"); // this is the offerer, so create a data channel

    dc->onOpen([&]() {
        std::cout << "[DataChannel open: " << dc->label() << "]" << std::endl;
    });

    dc->onClosed([&]() {
        std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl;
    });

    dc->onMessage([](auto data) {
        if (std::holds_alternative<std::string>(data)) {
            std::cout << "[Received: " << std::get<std::string>(data) << "]" << std::endl;
        }
    });

    std::this_thread::sleep_for(1s);

}

void offerer::setRemoteSdp(std::string remoteSdp) {
    // 用于存储处理后的 SDP 字符串
    std::string processedSdp;

    // 使用 std::istringstream 按行读取
    std::istringstream stream(remoteSdp);
    std::string line;
    while (std::getline(stream, line)) {
        // 在每一行的结尾添加 \r\n
        processedSdp += line + "\r\n";
    }
    pc->setRemoteDescription(processedSdp);
}

void offerer::setRemoteCandidate(std::string candidate) {
    pc->addRemoteCandidate(candidate);
}

void offerer::setMessage(std::string message) {
    if (!dc->isOpen()) {
        std::cout << "** Channel is not Open ** ";
        return;
    }

    dc->send(message);
}

void offerer::printConnectionInfo() {
    // Connection Info
    if (!dc || !dc->isOpen()) {
        std::cout << "** Channel is not Open ** " << std::endl;
        return;
    }
    rtc::Candidate local, remote;
    std::optional<std::chrono::milliseconds> rtt = pc->rtt();
    if (pc->getSelectedCandidatePair(&local, &remote)) {
        std::cout << "Local: " << local << std::endl;
        std::cout << "Remote: " << remote << std::endl;
        std::cout << "Bytes Sent:" << pc->bytesSent()
                  << " / Bytes Received:" << pc->bytesReceived() << " / Round-Trip Time:";
        if (rtt.has_value()) {
            std::cout << rtt.value().count();
        } else {
            std::cout << "null";
        }
        std::cout << " ms";
    } else {
        std::cout << "Could not get Candidate Pair Info" << std::endl;
    }
}

void offerer::close() {
    if (dc) {
        dc->close();
    }
    if (pc) {
        pc->close();
    }
}

void offerer::setSdpCallback(std::function<void(std::string)> callback) {
    m_sdpCallback = callback;
}

void offerer::setCandidateCallback(std::function<void(std::string)> callback) {
    m_candidateCallback = callback;
}
