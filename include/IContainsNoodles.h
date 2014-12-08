#ifndef ICONTAINSNOODLES_H
#define ICONTAINSNOODLES_H

class IContainsNoodles
{
public:
	virtual ~IContainsNoodles()
	{
		for_each(m_noodles.cbegin(), m_noodles.cend(),
			[](const NoodleBase *n) {
				delete n;
			});
	}
	
	virtual void check() const;
	
	vector<NoodleBase *>::iterator noodles_begin()
	{ return m_noodles.begin(); }
	vector<NoodleBase *>::iterator noodles_end()
	{ return m_noodles.end(); }
	
	vector<NoodleBase *>::const_iterator noodles_cbegin() const
	{ return m_noodles.cbegin(); }
	vector<NoodleBase *>::const_iterator noodles_cend() const
	{ return m_noodles.cend(); }
	
	vector<NoodleBase *>::size_type noodles_size() const
	{ return m_noodles.size(); }
	bool noodles_empty() const
	{ return m_noodles.empty(); }
	
protected:
	IContainsNoodles() {}
	
	virtual void add_noodle(NoodleBase *n);
	
private:
	vector<NoodleBase *> m_noodles;
};

class DuplicateNoodleException : public runtime_error
{
public: DuplicateNoodleException() : runtime_error(
	"Noodle instances may only be registered once") {}
};

#define ADD_QNOODLE(_type, _max, _b_from, _p_from, _b_to, _p_to) \
	add_noodle(new QNoodle<_type>(_max, (_b_from).find_port(#_p_from), \
		(_b_to).find_port(#_p_to)))
#define ADD_RNOODLE(_type, _init, _b_from, _p_from, _b_to, _p_to) \
	add_noodle(new RNoodle<_type>(_init, (_b_from).find_port(#_p_from), \
		(_b_to).find_port(#_p_to)))

#endif
