#pragma once
#include "../../stdafx.hpp"

namespace ve {

	class script_manager {
	public:
		void tick();
		void tick_internal();
		void add(std::unique_ptr<script> script);
		void add(script* script);
		void add(const std::vector<std::pair<std::string, std::function<void()>>>& scripts);
		void add(const std::string& name, std::function<void()> func);
		void cleanup();
		void suspend(const std::string& name);
		void resume(const std::string& name);
		void terminate(const std::string& name);
		void list_active_scripts();
		script* find(const std::string& name);

	private:
		std::vector<std::unique_ptr<script>> m_scripts;
		std::mutex m_Mutex;
	};

	script_manager* get_script_manager();
}
