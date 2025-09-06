#include <iostream>
#include <sstream>
#include "../lib/net.hpp"
#include "../lib/protocol.hpp"
struct GameState{ int health=100; int coins=25; int level=1; };
static void handle_client(int csock, GameState& st){
    try{
        std::string nonce="123456789";
        net::send_line(csock, "WELCOME "+nonce);
        std::string line=net::recv_line(csock);
        if(line.rfind("AUTH ",0)!=0){ net::send_line(csock,"ERR expected AUTH"); net::close_socket(csock); return; }
        std::string token=line.substr(5);
        if(token!=proto::token_for(nonce)){ net::send_line(csock,"ERR bad token"); net::close_socket(csock); return; }
        net::send_line(csock,"OK");
        while(true){
            std::string cmd=net::recv_line(csock);
            if(cmd=="GET"){
                net::send_line(csock, "STATE health="+std::to_string(st.health)+" coins="+std::to_string(st.coins)+" level="+std::to_string(st.level));
            } else if(cmd.rfind("PATCH ",0)==0){
                std::istringstream iss(cmd.substr(6)); std::string f; int v;
                if(!(iss>>f>>v)){ net::send_line(csock,"ERR usage"); continue; }
                if(f=="coins" && v>=0 && v<=1000000){ st.coins=v; net::send_line(csock,"OK"); }
                else if(f=="health" && v>=0 && v<=1000){ st.health=v; net::send_line(csock,"OK"); }
                else if(f=="level" && v>=1 && v<=200){ st.level=v; net::send_line(csock,"OK"); }
                else net::send_line(csock,"ERR invalid");
            } else if(cmd=="BYE"){ net::send_line(csock,"BYE"); break; }
            else net::send_line(csock,"ERR");
        }
    }catch(...){}
    net::close_socket(csock);
}
int main(){ try{ GameState st; int srv=net::make_server(proto::PORT); std::cout<<"Target on 127.0.0.1:"<<proto::PORT<<"\n";
    while(true){ int c=net::accept_client(srv); handle_client(c,st); } } catch(const std::exception& e){ std::cerr<<"Error "<<e.what()<<"\n"; return 1; } return 0; }
