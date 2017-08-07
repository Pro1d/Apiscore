//
// Created by pgg on 04/08/17.
//

#ifndef APISCORE_POINTERSWITCHER_H
#define APISCORE_POINTERSWITCHER_H

#include <memory>
#include <mutex>

template<typename T>
class PointerSwitcher {
public:
    PointerSwitcher(std::shared_ptr<T> const& p1, std::shared_ptr<T> const& p2) : read(p1), write(p2) {}
    PointerSwitcher(T* p1, T* p2) : read(p1), write(p2) {}
    PointerSwitcher() : read(nullptr), write(nullptr) {}

    void reset(T* p1, T* p2)
    {
        read.reset(p1);
        write.reset(p2);
    }

    std::unique_lock<std::mutex> lockForScope()
    {
        return std::unique_lock<std::mutex>(locker);
    }

    void unsafeSwitchReadWrite()
    {
        std::swap(read, write);
    }

    void safeSwitchReadWrite()
    {
        std::unique_lock<std::mutex> lock(locker);
        std::swap(read, write);
    }

    std::shared_ptr<T>& getRead() { return read; }
    std::shared_ptr<T>& getWrite() { return write; }
private:
    std::shared_ptr<T> read;
    std::shared_ptr<T> write;
    std::mutex locker;
};


#endif //APISCORE_POINTERSWITCHER_H
