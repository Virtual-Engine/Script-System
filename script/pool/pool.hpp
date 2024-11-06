#pragma once
#include "../../stdafx.hpp"

namespace ve {

    struct Job {
        std::function<void()> func;
        std::chrono::steady_clock::time_point scheduled_time;
        int priority;

        bool operator<(const Job& other) const {
            return priority < other.priority;
        }
    };
     
    class script_pool {
    public:
        void initialize();
        void tick(std::size_t num_fibers);
        void cleanup();

        void queue_job(std::function<void()> func, int priority = 0, std::chrono::steady_clock::duration delay = std::chrono::milliseconds(0));

        void func();
        void* get_script(const std::string& script_name);
        int get_total();
        int get_used();
        void reset();
        void list_active_pool();

    private:
        std::size_t m_num_fibers;
        std::mutex m_mutex;
        std::condition_variable m_cv;
        std::priority_queue<Job> m_jobs;
        std::unordered_map<std::string, std::unique_ptr<script>> m_scripts; 
    };

    script_pool* get_script_pool();
}
