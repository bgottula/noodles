#include "std.h"
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include "Block.h"

class MockBlock : public Block
{
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( blockTest )
{
	MockBlock dut;

	BOOST_CHECK_EQUAL(dut.outputEmpty(), true);
}
