#include <iostream>
#include "../lib/net.hpp"
#include "../lib/protocol.hpp"
int main(){ try{ int s=net::connect_to("127.0.0.1", proto::PORT);
    std::string banner=net::recv_line(s); if(banner.rfind("WELCOME ",0)!=0){ std::cerr<<"bad banner\n"; return 1; }
    std::string nonce=banner.substr(8); std::string tk=proto::token_for(nonce);
    net::send_line(s,"AUTH "+tk); std::string ok=net::recv_line(s); if(ok!="OK"){ std::cerr<<"auth failed\n"; return 1; }
    net::send_line(s,"GET"); std::cout<<net::recv_line(s)<<"\n";
    net::send_line(s,"PATCH coins 999"); std::cout<<net::recv_line(s)<<"\n";
    net::send_line(s,"GET"); std::cout<<net::recv_line(s)<<"\n";
    net::send_line(s,"BYE"); net::recv_line(s); net::close_socket(s); net::cleanup(); return 0;
} catch(const std::exception& e){ std::cerr<<"err "<<e.what()<<"\n"; return 1; } }
