#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "NonCopyable.h"

class Socket : NonCopyable {
   public:
    Socket();
    // explicit关键字修饰构造函数，用于阻止某些隐式类型转换
    // 当一个构造函数被声明为explicit时，
    // 它将不会被用作隐式类型转换的候选。
    // 这意味着如果一个构造函数是explicit的，
    // 你不能直接使用一个已经初始化的对象来调用这个构造函数，
    // 从而创建一个新对象。
    // 就是它可以直接被创建，但是不能通过int值隐式转换成Socket对象
    explicit Socket(int fd);

    ~Socket();
    int fd() const;
    void shutDownWrite();

   private:
    int _fd;
};

#endif
