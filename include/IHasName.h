#ifndef IHASNAME_H
#define IHASNAME_H

class IHasName
{
public:
	virtual ~IHasName() {}
	
	virtual void check() const;
	
	virtual const char *get_name() const final;
	virtual void set_name(const char *name) final;
	
protected:
	IHasName() {}
	
private:
	const char *m_name = nullptr;
};

class AlreadyNamedException : public runtime_error
{
public: AlreadyNamedException() : runtime_error(
	"This object has already had a name set") {}
};
class NotNamedException : public runtime_error
{
public: NotNamedException() : runtime_error(
	"This object has not had a name set") {}
};

#endif
