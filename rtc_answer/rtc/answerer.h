#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "rtc/rtc.hpp"

class answerer {
public:
    answerer();
    ~answerer() = default;
    void startAnswer();
    void close();
    
    void setSdpCallback(std::function<void(std::string)> callback);
    void setCandidateCallback(std::function<void(std::string)> callback);

    void setRemoteSdp(std::string remoteSdp);
    void setRemoteCandidate(std::string candidate);
    void setMessage(std::string message);
    void printConnectionInfo();

    std::string m_answer;
    std::vector<std::string> m_answer_lines;
private:
    std::function<void(std::string)> m_sdpCallback = nullptr;
    std::function<void(std::string)> m_candidateCallback = nullptr;
    std::shared_ptr<rtc::PeerConnection> pc = nullptr;
    std::shared_ptr<rtc::DataChannel> dc = nullptr;

};
