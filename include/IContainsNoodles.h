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
	
protected:
	IContainsNoodles() {}
	
	virtual void add_noodle(NoodleBase *n) final;
	
private:
	vector<NoodleBase *> m_noodles;
	
	typedef vector<NoodleBase *>::iterator       v_iter;
	typedef vector<NoodleBase *>::const_iterator v_citer;
	typedef vector<NoodleBase *>::size_type      v_size;
	
public:
	v_iter noodles_begin()         { return m_noodles.begin(); }
	v_iter noodles_end()           { return m_noodles.end(); }
	
	v_citer noodles_cbegin() const { return m_noodles.cbegin(); }
	v_citer noodles_cend()   const { return m_noodles.cend(); }
	
	v_size noodles_size()    const { return m_noodles.size(); }
	bool   noodles_empty()   const { return m_noodles.empty(); }
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
