#include "stdafx.hpp"

using namespace ve;

int main() {
	get_script_manager()->add(std::make_unique<script>("Teste 1", [] {}));
	get_script_manager()->add(new script("Teste 2", [] {}));
	std::vector<std::pair<std::string, std::function<void()>>> scripts = {
		{
			"Teste 1", [] {
				std::cout << "Teste 1 Validated\n";
			}
		},
		{
			"Teste 2", [] {
				std::cout << "Teste 2 Validated\n";
			}
		},
		{
			"Teste 4", [] {
				std::cout << "Teste 4 Validated\n";
			}
		}
	};
	get_script_manager()->add(scripts);
	get_script_manager()->add("Teste 1", [] {});
	get_script_manager()->list_active_scripts();
	get_script_manager()->resume("Teste 2");
	get_script_manager()->suspend("Teste 2");
	get_script_manager()->terminate("Teste 2");
	get_script_manager()->find("Teste 2");
	get_script_pool()->queue_job([] {}, 10); //Max priority
	get_script_pool()->queue_job([] {});
	while (true) {
		get_script_manager()->tick();
		get_script_pool()->tick(5);
	}
	get_script_manager()->cleanup();
	get_script_pool()->cleanup();
}