#ifndef MessageBox_H
#define MessageBox_H

#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>

template <typename T>
class MessageBox {
   public:
    T receive() {
        // perform queue modification under the lock
        std::unique_lock<std::mutex> uLock(_mutex);
        _cond.wait(uLock, [this] {
            return (int(_message) != -1);
        });  // pass unique lock to condition variable

        // remove last vector element from queue
        T msg = _message;
        _message = T(-1);

        return msg;  // will not be copied due to return value optimization
                     // (RVO) in C++
    }
    void send(T &&msg) {
        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        // add vector to queue
        _message = std::move(msg);
        _cond.notify_one();  // notify client after pushing new Vehicle into
                             // vector
    }

   private:
    std::mutex _mutex;
    std::condition_variable _cond;
    T _message;
};

#endif /*MessageBox_H*/