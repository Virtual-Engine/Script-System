#include "pool.hpp"

namespace ve {

    script_pool* g_script_pool;

    void script_pool::tick(std::size_t num_fibers) {
        m_num_fibers = num_fibers;
        for (std::size_t i = 0; i < num_fibers; ++i) {
            std::string script_name = "Fiber_" + std::to_string(i);
            get_script_manager()->add(std::make_unique<script>(script_name, [&] { func(); }));
        }
    }

    void script_pool::cleanup() {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& pair : m_scripts) {
            pair.second->set_enabled(false);
        }
        m_scripts.clear();
    }

    void script_pool::queue_job(std::function<void()> func, int priority, std::chrono::steady_clock::duration delay) {
        if (func) {
            std::lock_guard<std::mutex> lock(m_mutex);
            Job job{ std::move(func), std::chrono::steady_clock::now() + delay, priority };
            m_jobs.push(std::move(job));
            m_cv.notify_one();
        }
    }

    void script_pool::initialize() {
        std::unique_lock lock(m_mutex);
        while (m_jobs.empty()) {
            m_cv.wait(lock);
        }

        auto job = std::move(m_jobs.top());
        auto now = std::chrono::steady_clock::now();

        if (job.scheduled_time <= now) {
            m_jobs.pop();
            lock.unlock();
            std::invoke(std::move(job.func));
        }
        else {
            m_cv.wait_until(lock, job.scheduled_time);
        }
    }

    void script_pool::func() {
        while (true) {
            this->initialize();
            script* current_script = script::get_current();
            if (current_script) {
                current_script->yield();
            }
        }
    }

    void* script_pool::get_script(const std::string& script_name) {
        auto it = m_scripts.find(script_name);
        if (it != m_scripts.end()) {
            return it->second->get_script_fiber();
        }
        return nullptr;
    }

    int script_pool::get_total() {
        return m_num_fibers;
    }

    int script_pool::get_used() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_jobs.size();
    }

    void script_pool::reset() {
        std::lock_guard<std::mutex> lock(m_mutex);
        while (!m_jobs.empty()) {
            m_jobs.pop();
        }
    }

    void script_pool::list_active_pool() {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << "Listing active fibers:\n";
        for (const auto& pair : m_scripts) {
            std::cout << pair.first << " - Status: "
                << (pair.second->is_enabled() ? "Active" : "Disabled") << "\n";
        }
    }

    script_pool* get_script_pool() { return g_script_pool; }

}
