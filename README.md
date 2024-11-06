# Fiber System
- A lightweight script system in C++ for creating, managing, and executing scripts. This system provides functionalities such as script creation, suspension, resuming, and termination.

# Table of Contents
- Features
- Installation
- Usage
  - Code Examples
- API
- Contributing
- License

# Features
- Create scripts with unique names.
- Add multiple scripts using vectors.
- Manage script states (suspended, resumed, terminated).
- Enumerate active scripts.
- Easy integration with lambda functions.

# Installation
To use this fiber system, simply include the provided header files in your C++ project and ensure your environment is set up to use Windows fiber functionalities (if required).

# Usage

- Here’s a sample of how to use the script system:

```c++
#include "stdafx.hpp"

int main() {
    // Adding individual scripts with lambdas
    get_script_manager()->add(std::make_unique<script>("Teste 1", [] {
        std::cout << "Teste 1 Executed\n";
    }));

    get_script_manager()->add(new script("Teste 2", [] {
        std::cout << "Teste 2 Executed\n";
    }));

    // Adding multiple scripts via a vector
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

    // Adding a single script by name and function
    get_script_manager()->add("Teste 1", [] {});

    // Listing active scripts
    get_script_manager()->list_active_scripts();

    // Suspending, resuming, and terminating a script
    get_script_manager()->resume("Teste 2");
    get_script_manager()->suspend("Teste 2");
    get_script_manager()->terminate("Teste 2");

    // Finding a script by name
    get_script_manager()->find("Teste 2");

    // Managing jobs in the script pool
    get_script_pool()->queue_job([] {
        std::cout << "High-priority Job Executed\n";
    }, 10); // Max priority

    get_script_pool()->queue_job([] {
        std::cout << "Normal-priority Job Executed\n";
    });

    // Main loop to execute scripts and manage the script pool
    while (true) {
        get_script_manager()->tick();  // Update the script manager
        get_script_pool()->tick(5);   // Update the script pool with priority management
    }

    // Cleaning up scripts and script pool
    get_script_manager()->cleanup();
    get_script_pool()->cleanup();

    return 0;
}
```

# Features Breakdown
- Script Creation: Add scripts using `add()`, passing a name and a function.
- Script Management: Use `suspend()`, `resume()`, and `terminate()` to manage script states.
- Active Script Enumeration: Use `list_active_scripts()` to view currently active scripts.
- Integration with a Script Pool: Create and manage a pool of scripts for efficient task handling.

# API
`get_script_manager()`
Returns the global script manager, allowing access to all script management methods.

`add(std::unique_ptr<script>)`
Adds a unique script to the manager.

`add(new script(name, func))` 
Adds a new script using the new operator.

`add(const std::vector<std::pair<std::string, std::function<void()>>>& scripts)` 
Adds multiple scripts in a single operation.

`add(const std::string& name, std::function<void()> func)` 
Adds a script by specifying its name and function.

`suspend(const std::string& name)` 
Suspends the specified script by name.

`resume(const std::string& name)` 
Resumes the execution of the specified script.

`terminate(const std::string& name)` 
Terminates the specified script.

`find(const std::string& name)` 
Returns information about the specified script.

`list_active_scripts()` 
Displays all active scripts.

# Contributing
- Contributions are welcome! Please submit a pull request or open an issue to discuss improvements.
