#include "all.h"
#include "boost_test.h"

class MockBlock : public Block
{
public:
	MockBlock(void)
	{
		inputs.add("in");
		outputs.add("out");
	}
	
	void reset(void) {}
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( graphTest )
{
	MockBlock block1;
	MockBlock block2;
	Graph dut;
	
	/* No connections yet */
	BOOST_CHECK_THROW(dut.checkGraph(), EmptyGraphException);
	
	/* Add one valid noodle */
	dut.addNoodle({&block1, "out"}, {&block2, "in"});
	BOOST_CHECK_EQUAL(dut.checkGraph(), 0);
}
