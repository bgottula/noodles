#include "all.h"
#include "boost_test.h"

class MockBlock : public Block
{
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( graphTest )
{
	MockBlock block1;
	MockBlock block2;
	MockBlock block3;
	Graph dut;

	/* No connections yet */
	BOOST_CHECK_THROW(dut.checkGraph(), EmptyGraphException);

	/* Add one valid noodle */
	Noodle n1(&block1, &block2);
	dut.addNoodle(&n1);
	BOOST_CHECK_EQUAL(dut.checkGraph(), 0);

	/* Try to add the same noodle again */
	BOOST_CHECK_THROW(dut.addNoodle(&n1), DuplicateNoodleException);

	/* Try to make block1 and block3 be the source for block2 */
	Noodle n2(&block3, &block2);
	BOOST_CHECK_THROW(dut.addNoodle(&n2), InputMultipleNoodleException);
}
