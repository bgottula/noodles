#include "std.h"
#include "boost_test.h"
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
