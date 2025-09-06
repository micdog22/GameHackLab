#pragma once
#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
#endif
#include <string>
#include <stdexcept>

namespace net {
    inline void init() {
    #ifdef _WIN32
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) throw std::runtime_error("WSAStartup failed");
    #endif
    }
    inline void cleanup() {
    #ifdef _WIN32
        WSACleanup();
    #endif
    }
    inline int close_socket(int s) {
    #ifdef _WIN32
        return closesocket(s);
    #else
        return close(s);
    #endif
    }
    inline int make_server(uint16_t port) {
        init();
        int s = (int)::socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) throw std::runtime_error("socket failed");
        int opt = 1;
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_port = htons(port); addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (::bind(s,(sockaddr*)&addr,sizeof(addr))<0) { close_socket(s); throw std::runtime_error("bind failed"); }
        if (::listen(s,1)<0) { close_socket(s); throw std::runtime_error("listen failed"); }
        return s;
    }
    inline int accept_client(int srv) {
        sockaddr_in cli{}; socklen_t len=sizeof(cli);
        int c=(int)::accept(srv,(sockaddr*)&cli,&len);
        if (c<0) throw std::runtime_error("accept failed");
        return c;
    }
    inline int connect_to(const std::string& host, uint16_t port) {
        init();
        int s=(int)::socket(AF_INET,SOCK_STREAM,0); if(s<0) throw std::runtime_error("socket failed");
        sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_port=htons(port); addr.sin_addr.s_addr=inet_addr(host.c_str());
        if(::connect(s,(sockaddr*)&addr,sizeof(addr))<0){ close_socket(s); throw std::runtime_error("connect failed"); }
        return s;
    }
    inline void send_all(int s, const void* buf, size_t len){
        const char* p=(const char*)buf; size_t left=len;
        while(left>0){
        #ifdef _WIN32
            int n=::send(s,p,(int)left,0);
        #else
            ssize_t n=::send(s,p,left,0);
        #endif
            if(n<=0) throw std::runtime_error("send failed"); left-=n; p+=n;
        }
    }
    inline void send_line(int s, const std::string& line){ std::string d=line+"\n"; send_all(s,d.data(),d.size()); }
    inline std::string recv_line(int s){ std::string out; char ch;
        while(true){
        #ifdef _WIN32
            int n=::recv(s,&ch,1,0);
        #else
            ssize_t n=::recv(s,&ch,1,0);
        #endif
            if(n<=0) throw std::runtime_error("recv failed");
            if(ch=='\n') break; out.push_back(ch);
        }
        return out;
    }
}
