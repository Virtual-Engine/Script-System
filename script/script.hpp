#pragma once
#include <string>
#include <optional>
#include <functional>
#include <chrono>
#include <windows.h>

namespace ve {

    using func_t = std::function<void()>;

    class script {
    public:
        script(const std::string& name, func_t func, bool toggleable = false, std::optional<std::size_t> stack_size = std::nullopt)
            : m_name(name), m_toggleable(toggleable), m_func(func) {
            m_script_fiber = CreateFiber(
                stack_size.value_or(0),
                [](void* param) {
                    auto this_script = static_cast<script*>(param);
                    this_script->fiber_func();
                },
                this
            );

            if (!m_script_fiber) {
                throw std::runtime_error("Failed to create fiber");
            }
        }

        ~script() {
            m_enabled = false;
            if (m_script_fiber) {
                DeleteFiber(m_script_fiber);
            }
        }

        const char* name() const {
            return m_name.c_str();
        }

        bool is_enabled() const {
            return m_enabled;
        }

        void set_enabled(bool toggle) {
            if (m_toggleable) {
                m_enabled = toggle;
            }
        }

        bool* toggle_ptr() {
            return &m_enabled;
        }

        bool is_toggleable() const {
            return m_toggleable;
        }

        bool is_done() const {
            return m_done;
        }

        void tick() {
            if (m_main_fiber == nullptr) {
                m_main_fiber = GetCurrentFiber();
            }
            if ((!m_wake_time || m_wake_time <= std::chrono::high_resolution_clock::now()) && m_script_fiber) {
                SwitchToFiber(m_script_fiber);
            }
        }

        void yield(int milliseconds = 0) {
            if (milliseconds > 0) {
                m_wake_time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(milliseconds);
            }
            else {
                m_wake_time = std::nullopt;
            }
            SwitchToFiber(m_main_fiber);
        }

        static script* get_current() {
            return static_cast<script*>(GetFiberData());
        }

        void* get_script_fiber() {
            return m_script_fiber;
        }

    private:
        void fiber_func() {
            m_func();
            m_done = true;

            while (true) {
                yield();
            }
        }

        bool m_enabled{ true };
        bool m_toggleable{ false };
        bool m_done{ false };
        std::string m_name;
        func_t m_func;
        void* m_script_fiber{ nullptr };
        void* m_main_fiber{ nullptr };
        std::optional<std::chrono::high_resolution_clock::time_point> m_wake_time;
    };
}
