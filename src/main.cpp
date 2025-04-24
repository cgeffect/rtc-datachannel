#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "rtc/rtc.hpp"
#include "openssl/ssl.h"

#include <iostream>

int main() {
    std::cout << "OpenSSL version: " << SSLeay_version(SSLEAY_VERSION) << std::endl;

    auto ws = std::make_shared<rtc::WebSocket>();

    std::cout << "Hello World!" << std::endl;
    return 0;
}
