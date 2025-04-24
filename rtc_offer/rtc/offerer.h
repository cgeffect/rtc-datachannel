
#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <memory>
#include "rtc/rtc.hpp"

class offerer {
public:
    offerer(/* args */);
    void startOffer();
    void close();
    ~offerer() = default;
    void setSdpCallback(std::function<void(std::string)> callback);
    void setCandidateCallback(std::function<void(std::string)> callback);

    void setRemoteSdp(std::string remoteSdp);
    void setRemoteCandidate(std::string candidate);
    void setMessage(std::string message);
    void printConnectionInfo();

    void setMessageCallback(std::function<void(std::string)> callback);
    void setConnectionInfoCallback(std::function<void(std::string)> callback);
    void setExitCallback(std::function<void()> callback);
    void setCommandCallback(std::function<void(int)> callback);
    void setConnectionInfo(std::string connectionInfo);
    void setExit();
    void setCommand(int command);
    std::string m_offer;
    std::vector<std::string> m_offer_lines;
    std::function<void(std::string)> m_messageCallback;
    std::function<void(std::string)> m_connectionInfoCallback;
    std::function<void()> m_exitCallback;
    std::function<void(int)> m_commandCallback;
private:
    std::function<void(std::string)> m_sdpCallback = nullptr;
    std::function<void(std::string)> m_candidateCallback = nullptr;
    std::shared_ptr<rtc::PeerConnection> pc = nullptr;
    std::shared_ptr<rtc::DataChannel> dc = nullptr;
};
