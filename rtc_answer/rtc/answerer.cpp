/**
 * Copyright (c) 2019 Paul-Louis Ageneau
 * Copyright (c) 2019 Murat Dogan
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "answerer.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <stdexcept>  // 包含 std::logic_error

using namespace std::chrono_literals;
using std::shared_ptr;
using std::weak_ptr;
template <class T>
weak_ptr<T> make_weak_ptr(shared_ptr<T> ptr) {
    return ptr;
}

answerer::answerer() {
}
void answerer::startAnswer() {
    rtc::InitLogger(rtc::LogLevel::Warning);

    rtc::Configuration config;
    // config.iceServers.emplace_back("stun.l.google.com:19302");

    pc = std::make_shared<rtc::PeerConnection>(config);

    pc->onLocalDescription([this](rtc::Description description) {
        std::cout << "Local Description (Paste this to the other peer):" << std::endl;
        std::string answerType = description.typeString();
        std::string answerSdp = std::string(description);
        std::cout << "answerType: " << answerType << std::endl;
        std::cout << answerSdp << std::endl;
        m_sdpCallback(answerSdp);
    });

    pc->onLocalCandidate([this](rtc::Candidate candidate) {
        std::cout << "Local Candidate (Paste this to the other peer after the local description):"
                  << std::endl;
        std::cout << std::string(candidate) << std::endl
                  << std::endl;
        m_candidateCallback(std::string(candidate));
    });

    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "[State: " << state << "]" << std::endl;
    });

    pc->onGatheringStateChange([this](rtc::PeerConnection::GatheringState state) {
        std::cout << "[Gathering State: " << state << "]" << std::endl;
        printConnectionInfo();
    });

    // Receive a DataChannel
    pc->onDataChannel([&](shared_ptr<rtc::DataChannel> _dc) {
        std::cout << "[Got a DataChannel with label: " << _dc->label() << "]" << std::endl;
        dc = _dc;

        dc->onClosed([&]() {
            std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl;
        });

        dc->onMessage([](auto data) {
            if (std::holds_alternative<std::string>(data)) {
                std::cout << "[Received message: " << std::get<std::string>(data) << "]"
                          << std::endl;
            }
        });
    });

    //
    //    bool exit = false;
    //    while (!exit) {
    //        std::cout
    //            << std::endl
    //            << "**********************************************************************************"
    //               "*****"
    //            << std::endl
    //            << "* 0: Exit /"
    //            << " 1: Enter remote description /"
    //            << " 2: Enter remote candidate /"
    //            << " 3: Send message /"
    //            << " 4: Print Connection Info *" << std::endl
    //            << "[Command]: ";
    //
    //        int command = -1;
    //        std::cin >> command;
    //        std::cin.ignore();
    //
    //        switch (command) {
    //        case 0: {
    //            exit = true;
    //            break;
    //        }
    //        case 1: {
    //            // Parse Description
    //            std::cout << "[Description]: ";
    //            std::string sdp, line;
    //            while (getline(std::cin, line) && !line.empty()) {
    //                sdp += line;
    //                sdp += "\r\n";
    //            }
    //            std::cout << sdp;
    //            pc->setRemoteDescription(sdp);
    //            break;
    //        }
    //        case 2: {
    //            // Parse Candidate
    //            std::cout << "[Candidate]: ";
    //            std::string candidate;
    //            getline(std::cin, candidate);
    //            pc->addRemoteCandidate(candidate);
    //            break;
    //        }
    //        case 3: {
    //            // Send Message
    //            if (!dc || !dc->isOpen()) {
    //                std::cout << "** Channel is not Open ** ";
    //                break;
    //            }
    //            std::cout << "[Message]: ";
    //            std::string message;
    //            getline(std::cin, message);
    //            dc->send(message);
    //            break;
    //        }
    //        case 4: {
    //            // Connection Info
    //            if (!dc || !dc->isOpen()) {
    //                std::cout << "** Channel is not Open ** ";
    //                break;
    //            }
    //            rtc::Candidate local, remote;
    //            std::optional<std::chrono::milliseconds> rtt = pc->rtt();
    //            if (pc->getSelectedCandidatePair(&local, &remote)) {
    //                std::cout << "Local: " << local << std::endl;
    //                std::cout << "Remote: " << remote << std::endl;
    //                std::cout << "Bytes Sent:" << pc->bytesSent()
    //                          << " / Bytes Received:" << pc->bytesReceived() << " / Round-Trip Time:";
    //                if (rtt.has_value())
    //                    std::cout << rtt.value().count();
    //                else
    //                    std::cout << "null";
    //                std::cout << " ms";
    //            } else {
    //                std::cout << "Could not get Candidate Pair Info" << std::endl;
    //            }
    //            break;
    //        }
    //        default: {
    //            std::cout << "** Invalid Command ** " << std::endl;
    //            break;
    //        }
    //        }
    //    }
}

void answerer::setRemoteSdp(std::string remoteSdp) {
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

void answerer::setRemoteCandidate(std::string candidate) {
    try {
        pc->addRemoteCandidate(candidate);
    } catch (const std::logic_error& e) {
        // 捕获 std::logic_error 并处理
        std::cerr << "Caught std::logic_error: " << e.what() << std::endl;
        // 在这里可以执行一些错误处理逻辑，例如：
        // - 日志记录
        // - 通知用户
        // - 尝试恢复或跳过当前操作
    } catch (const std::exception& e) {
        // 捕获其他标准异常
        std::cerr << "Caught other std::exception: " << e.what() << std::endl;
    } catch (...) {
        // 捕获其他未知异常
        std::cerr << "Caught unknown exception" << std::endl;
    }
}

void answerer::setMessage(std::string message) {
    if (!dc || !dc->isOpen()) {
        std::cout << "** Channel is not Open ** ";
        return;
    }

    dc->send(message);
}

void answerer::printConnectionInfo() {
    // Connection Info
    if (!dc || !dc->isOpen()) {
        std::cout << "** Channel is not Open ** " << std::endl;
        return;
        ;
    }
    rtc::Candidate local, remote;
    std::optional<std::chrono::milliseconds> rtt = pc->rtt();
    if (pc->getSelectedCandidatePair(&local, &remote)) {
        std::cout << "Local: " << local << std::endl;
        std::cout << "Remote: " << remote << std::endl;
        std::cout << "Bytes Sent:" << pc->bytesSent()
                  << " / Bytes Received:" << pc->bytesReceived() << " / Round-Trip Time:";
        if (rtt.has_value())
            std::cout << rtt.value().count();
        else
            std::cout << "null";
        std::cout << " ms";
    } else {
        std::cout << "Could not get Candidate Pair Info" << std::endl;
    }
}

void answerer::close() {
    if (dc)
        dc->close();

    if (pc)
        pc->close();
}

void answerer::setSdpCallback(std::function<void(std::string)> callback) {
    m_sdpCallback = callback;
}

void answerer::setCandidateCallback(std::function<void(std::string)> callback) {
    m_candidateCallback = callback;
}
