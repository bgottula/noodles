#ifndef BOOST_TEST_H
#define BOOST_TEST_H

#define BOOST_TEST_MAIN

#if USE_BOOST_UTF_DYNAMIC
	#warning using dynamic
	#define BOOST_TEST_DYN_LINK
	#include <boost/test/unit_test.hpp>
#elif USE_BOOST_UTF_STATIC
	#warning using static
	#include <boost/test/unit_test.hpp>
#elif USE_BOOST_UTF_HEADER
	#warning using header
	#include <boost/test/included/unit_test.hpp>
#else
	#error must define one of: USE_BOOST_UTF_{DYNAMIC,STATIC,HEADER}
#endif

#endif
