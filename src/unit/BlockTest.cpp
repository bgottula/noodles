#include "all.h"
#include "boost_test.h"

class MockBlock : public Block
{
	void reset() {}
	void work() {}
};

BOOST_AUTO_TEST_CASE( blockTest )
{
	BOOST_CHECK(0);
}
