#include <cassert>
#include <boost/concept_check.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/mpl/vector.hpp>

template <typename H>
struct HolderConcept : boost::Assignable<H>
	, boost::CopyConstructible<H>
{
	BOOST_CONCEPT_USAGE(HolderConcept)
	{
		h.store(i);
		i = load(h);
	}

private:
	H h;
	int i;
};

struct SomeHolder
{
	int val = 0;
	void store(int i) { val = i; }
};
int load(SomeHolder& h) { return h.val; }

BOOST_CONCEPT_ASSERT((HolderConcept<SomeHolder>));

// Type erased interface
BOOST_TYPE_ERASURE_MEMBER((has_member_store), store, 1)
BOOST_TYPE_ERASURE_FREE((has_free_load), load, 1)

namespace te = boost::type_erasure;

using Holder = boost::mpl::vector<
	te::copy_constructible<>,
	has_member_store<void(int)>,
	has_free_load<int(te::_self&)>,
	te::relaxed
>;

using AnyHolder = te::any<Holder>;

BOOST_CONCEPT_ASSERT((HolderConcept<AnyHolder>));

int test()
{
	AnyHolder h{ SomeHolder{} };
	h.store(2);
	int i = load(h);
	assert(i == 2);
}