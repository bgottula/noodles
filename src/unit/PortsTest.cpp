#include "all.h"
#include "boost_test.h"


BOOST_AUTO_TEST_CASE( portsTest )
{
	Endpoint e;
	Noodle n(e, e);
	int sample;

	/* Attempt multiple abuses of InputPorts */
	InputPorts ip;
	BOOST_CHECK_THROW(ip.connect("foo", &n), NonexistentPortException);
	BOOST_CHECK_THROW(ip.get("foo", &sample), NonexistentPortException);
	ip.add("foo");
	BOOST_CHECK_THROW(ip.add("foo"), DuplicatePortException);
	BOOST_CHECK_THROW(ip.get("foo", &sample), InputNotConnectedException);
	ip.connect("foo", &n);
	BOOST_CHECK_THROW(ip.connect("foo", &n), InputMultipleNoodleException);

	/* Attempt multiple abuses of OutputPorts */
	OutputPorts op;
	BOOST_CHECK_THROW(op.connect("foo", &n), NonexistentPortException);
	BOOST_CHECK_THROW(op.put("foo", sample), NonexistentPortException);
	op.add("foo");
	BOOST_CHECK_THROW(op.add("foo"), DuplicatePortException);
	BOOST_CHECK_THROW(op.put("foo", sample), OutputNotConnectedException);
}
