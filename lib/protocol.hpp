#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

namespace proto {
    static constexpr uint16_t PORT = 39111;
    static const std::string SHARED_KEY = "GameHackLab-OnlyForEducation";
    inline uint64_t fnv1a64(const std::string& s){ uint64_t h=1469598103934665603ULL; for(unsigned char c:s){ h^=(uint64_t)c; h*=1099511628211ULL;} return h; }
    inline std::string token_for(const std::string& nonce){ uint64_t h=fnv1a64(SHARED_KEY+"|"+nonce); std::ostringstream oss; oss<<std::hex<<std::setw(16)<<std::setfill('0')<<h; return oss.str(); }
}
