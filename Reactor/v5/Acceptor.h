#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <string>

#include "InetAddress.h"
#include "Socket.h"

using std::string;

class Acceptor {
   public:
    Acceptor(const string& ip, unsigned short port);
    ~Acceptor();
    void ready();

   private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();

   public:
    int accept();
    int fd() const;

   private:
    Socket _sock;
    InetAddress _addr;
};

#endif
