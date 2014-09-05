#include "std.h"
#include "boost_test.h"
#include "Noodles.h"
#include "Block.h"

class MockBlock : public Block
{
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( noodlesTest )
{
	MockBlock block1;
	MockBlock block2;
	MockBlock block3;
	Noodles dut;

	/* No connections yet */
	BOOST_CHECK(dut.checkGraph() >= 1);

	/* Add one valid connection */
	Connection c1(&block1, &block2);
	dut.addConnection(&c1);
	BOOST_CHECK_EQUAL(dut.checkGraph(), 0);

	/* Try to add the same connection again */
	BOOST_CHECK_THROW(dut.addConnection(&c1), std::runtime_error);

	/* Try to make block1 and block3 be the source for block2 */
	Connection c2(&block3, &block2);
	BOOST_CHECK_THROW(dut.addConnection(&c1), std::runtime_error);
}
