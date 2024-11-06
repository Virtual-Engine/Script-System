#include "manager.hpp"

namespace ve {

	script_manager g_script_manager;

	void script_manager::tick() {
		std::lock_guard<std::mutex> lock(m_Mutex);
		for (const auto& script : m_scripts) {
			if (script->is_enabled()) {
				script->tick();
			}
		}
	}

	void script_manager::tick_internal() {
		// for games ex: gta v rdr2 and more its not util for app
	}

	void script_manager::add(std::unique_ptr<script> script) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		if (!script) throw std::invalid_argument("Script is null.");
		std::cout << std::format("Create script: {}\n", script->name());
		m_scripts.push_back(std::move(script));
	}

	void script_manager::add(script* script) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		if (!script) {
			throw std::invalid_argument("Script is null.");
		}
		std::cout << std::format("Create script: {}\n", script->name());
		m_scripts.emplace_back(script);
	}

	void script_manager::add(const std::vector<std::pair<std::string, func_t>>& scripts) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		for (const auto& [name, func] : scripts) {
			auto scripts = std::make_unique<script>(name, func);
			std::cout << std::format("Create script: {}\n", scripts->name());
			m_scripts.push_back(std::move(scripts));
		}
	}

	void script_manager::add(const std::string& name, func_t func) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		auto scripts = std::make_unique<script>(name, func);
		std::cout << "Create script: " << scripts->name() << std::endl;
		m_scripts.push_back(std::move(scripts));
	}

	void script_manager::cleanup() {
		std::lock_guard<std::mutex> lock(m_Mutex);
		for (auto& script : m_scripts) {
			if (!script->is_done()) {
				script->set_enabled(false);
			}
		}
		m_scripts.clear();
	}

	void script_manager::suspend(const std::string& name) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		auto* target_script = find(name);
		if (target_script && target_script->is_enabled()) {
			target_script->set_enabled(false);
			std::cout << std::format("Script suspended: {}\n", target_script->name());
		}
	}

	void script_manager::resume(const std::string& name) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		auto* target_script = find(name);
		if (target_script && !target_script->is_enabled()) {
			target_script->set_enabled(true);
			std::cout << std::format("Script resumed: {}\n", target_script->name());
		}
	}

	void script_manager::terminate(const std::string& name) {
		std::lock_guard<std::mutex> lock(m_Mutex);
		auto* target_script = find(name);
		if (target_script && target_script->is_enabled()) {
			target_script->set_enabled(false);
			std::cout << std::format("Script terminated: {}\n", target_script->name());
		}
	}

	void script_manager::list_active_scripts() {
		std::lock_guard<std::mutex> lock(m_Mutex);
		std::cout << "Listing all active scripts:\n";
		for (const auto& script : m_scripts) {
			if (script->is_enabled()) {
				std::cout << std::format("Active script: {}\n", script->name());
			}
		}
	}

	script* script_manager::find(const std::string& name) {
		for (auto& script : m_scripts) {
			if (script->name() == name) {
				return script.get();
			}
		}
		return nullptr;
	}

	script_manager* get_script_manager() { return &g_script_manager; }

}
