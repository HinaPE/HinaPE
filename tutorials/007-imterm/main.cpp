#include "backends/platform.h"
#include <imterm/terminal.hpp>
#include <imterm/terminal_helpers.hpp>

struct Helper
{
	bool should_close = false;
};

struct TerminalCommands : public ImTerm::basic_terminal_helper<TerminalCommands, Helper>
{
	TerminalCommands() = default;

	static void echo(argument_type &) {}
	static void ls(argument_type &) {}
	static void cd(argument_type &) {}
	static void exit(argument_type &) {}
	static void help(argument_type &) {}
};

auto main() -> int
{
	Helper helper;
	ImTerm::terminal<TerminalCommands> terminal_log(helper);

	Kasumi::Platform platform(800, 600);
	platform.launch(
			[&]() -> int
			{
				terminal_log.show();
				return 0;
			}
	);
	return 0;
}
