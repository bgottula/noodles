#include "all.h"
#include "boost_test.h"

class MockBlock : public Block
{
	void reset(void) {}
	void work(void) {}
};

BOOST_AUTO_TEST_CASE( noodleTest )
{
	MockBlock b1, b2;
	
	Endpoint from = {&b1, "out"};
	Endpoint to   = {&b1, "in"};
	
	Noodle n(from, to);
	
	/* should be empty */
	BOOST_CHECK(n.empty());
	
	/* should not be empty */
	n.push(0);
	BOOST_CHECK(!n.empty());
	
	/* should be empty once again */
	BOOST_CHECK_EQUAL(n.pop(), 0);
	BOOST_CHECK(n.empty());
	
	/* what goes in one end should come out the other */
	srand(time(nullptr));
	int samples[100];
	for (size_t i = 0; i < 100; ++i)
	{
		samples[i] = rand();
		n.push(samples[i]);
	}
	for (size_t i = 0; i < 100; ++i)
	{
		BOOST_CHECK_EQUAL(n.pop(), samples[i]);
	}
	
	/* should be empty yet again */
	BOOST_CHECK(n.empty());
}
