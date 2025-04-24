#include <libdatachannel/peer_connection.h>
#include <libdatachannel/rtc.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// 读取 H.264 文件
std::vector<uint8_t> readH264File(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

// 保存 H.264 数据到文件
void saveH264Data(const std::vector<uint8_t> &data, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
}

// 发送 H.264 数据
void sendH264Data(rtc::RtpSender &sender, const std::vector<uint8_t> &h264Data) {
    rtc::RtpPacket packet;
    packet.setPayloadType(96);
    packet.setTimestamp(0);
    packet.setSequenceNumber(0);
    packet.setPayload(h264Data.data(), h264Data.size());
    sender.send(packet);
}

// 处理接收到的 RTP 数据包
void onRtpPacketReceived(rtc::RtpReceiver &receiver, const rtc::RtpPacket &packet) {
    std::vector<uint8_t> payload(packet.payload(), packet.payload() + packet.payloadLength());
    saveH264Data(payload, "received2.h264");
}

// 对等端类
class Peer {
public:
    Peer(const std::string &h264File) :
        h264Data(readH264File(h264File)) {
        rtc::Configuration config;
        pc = std::make_unique<rtc::PeerConnection>(config);

        pc->onIceGatheringStateChange([](rtc::PeerConnection &pc, rtc::PeerConnection::IceGatheringState newState) {
            std::cout << "ICE gathering state changed to: " << static_cast<int>(newState) << std::endl;
        });

        pc->onIceConnectionStateChange([](rtc::PeerConnection &pc, rtc::PeerConnection::IceConnectionState newState) {
            std::cout << "ICE connection state changed to: " << static_cast<int>(newState) << std::endl;
        });

        pc->onSignalingStateChange([](rtc::PeerConnection &pc, rtc::PeerConnection::SignalingState newState) {
            std::cout << "Signaling state changed to: " << static_cast<int>(newState) << std::endl;
        });

        pc->onTrack([this](rtc::RtpReceiver &receiver) {
            receiver.onRtpPacket([this, &receiver](const rtc::RtpPacket &packet) {
                onRtpPacketReceived(receiver, packet);
            });
        });

        videoTrack = pc->addTrack("video", "video");
        videoTrack->setCodecPreferences({rtc::RtpCodecCapability("H264", 96, rtc::RtpCodecCapability::H264ProfileLevelId("42e01f"), 90000)});
    }

    ~Peer() = default;

    void receiveOfferAndSendAnswer() {
        // 连接到信令服务器接收 offer
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8888);
        inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        if (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            std::cerr << "Failed to connect to signaling server" << std::endl;
            close(sock);
            return;
        }
        char buffer[4096];
        ssize_t bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            std::string offer(buffer, bytes_received);
            rtc::Description remote_desc(offer, rtc::Description::Type::Offer);
            pc->setRemoteDescription(remote_desc, [this, sock](const std::string &error) {
                if (!error.empty()) {
                    std::cerr << "Failed to set remote description: " << error << std::endl;
                    close(sock);
                    return;
                }
                pc->createAnswer([this, sock](const rtc::Description &answer, const std::string &error) {
                    if (!error.empty()) {
                        std::cerr << "Failed to create answer: " << error << std::endl;
                        close(sock);
                        return;
                    }
                    pc->setLocalDescription(answer, [sock, answer](const std::string &error) {
                        if (!error.empty()) {
                            std::cerr << "Failed to set local description: " << error << std::endl;
                        } else {
                            send(sock, answer.sdp().c_str(), answer.sdp().length(), 0);
                        }
                        close(sock);
                    });
                });
            });
        }
    }

    void sendData() {
        auto sender = videoTrack->sender();
        if (sender) {
            sendH264Data(*sender, h264Data);
        }
    }

private:
    std::unique_ptr<rtc::PeerConnection> pc;
    std::shared_ptr<rtc::Track> videoTrack;
    std::vector<uint8_t> h264Data;
};

int main() {
    Peer peer("input2.h264");
    peer.receiveOfferAndSendAnswer();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    peer.sendData();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}