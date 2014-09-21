#include "all.h"
#include "boost_test.h"

class MockBlock : public Block
{
	void reset(void) {}
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( graphValidityTests )
{
	MockBlock block1;
	MockBlock block2;
	MockBlock block3;
	MockBlock block4;
	Graph dut;

	/* No connections yet */
	BOOST_CHECK_THROW(dut.checkGraph(), EmptyGraphException);

	/* Add one valid noodle */
	Noodle n1(&block1, &block2);
	dut.addNoodle(&n1);
	BOOST_CHECK_EQUAL(dut.checkGraph(), 0);

	/* Try to add the same noodle again */
	BOOST_CHECK_THROW(dut.addNoodle(&n1), DuplicateNoodleException);

	/* Try to make block1 and block3 be the source for block2 (not ok) */
	Noodle n2(&block3, &block2);
	BOOST_CHECK_THROW(dut.addNoodle(&n2), InputMultipleNoodleException);

	/* Make block2 be the source for block3 and block4 (allowed) */
	Noodle n3(&block2, &block3);
	Noodle n4(&block2, &block4);
	dut.addNoodle(&n3);
	dut.addNoodle(&n4);
	BOOST_CHECK_EQUAL(dut.checkGraph(), 0);
}
