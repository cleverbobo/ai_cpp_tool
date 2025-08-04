#pragma once

#include <queue>
#include <mutex>

template <typename T>
class SafeQueue {
    private:

    std::queue<T> m_queue;
    std::mutex m_mutex;
    size_t m_max_size;

    public:

    SafeQueue(size_t max_size = 20):m_max_size(max_size){}

    ~SafeQueue(){}

    // 非负值，建议使用size_t
    // size() 函数命名更符合一般规范。
    size_t size(){
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    bool empty(){
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    bool push_back(T& value){
        std::unique_lock<std::mutex> lock(m_mutex);
        if( m_queue.size() < m_max_size ){
            m_queue.push(value);
            return true;
        }
        return false;      
    }

    bool pop(T& valuse){
        std::unique_lock<std::mutex> lock(m_mutex);
        if( m_queue.empty() ){
            return false;
        }
        // 这里要先拿出来，然后再pop
        auto res = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }


};


