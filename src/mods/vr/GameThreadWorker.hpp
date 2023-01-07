#pragma once

#include <deque>
#include <mutex>
#include <functional>

// Class that executes functions on the game thread.
class GameThreadWorker {
public:
    static GameThreadWorker& get() {
        static GameThreadWorker instance{};
        return instance;
    }

public:
    void execute() {
        std::scoped_lock _{m_mutex};
        if (m_queue.empty()) {
            return;
        }

        for (auto& func : m_queue) {
            func();
        }

        m_queue.clear();
    }

    void enqueue(std::function<void()> func) {
        std::scoped_lock _{m_mutex};
        m_queue.push_back(func);
    }

private:
    std::recursive_mutex m_mutex{};
    std::deque<std::function<void()>> m_queue{};
};