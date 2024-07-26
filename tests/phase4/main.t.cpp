#include <phase4/engine/moves/magic/magic_bitboards.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char **argv) {
	using namespace phase4::engine;

	doctest::Context context;

	context.applyCommandLine(argc, argv);

	int res = context.run();

	if (context.shouldExit()) {
		return res;
	}

	return res;
}