#define INFINITY_ENTRY_POINT
#include <Infinity.h>

class Example : public Infinity::Application
{
public:
	Example() {}
	~Example() {}
};

Infinity::Application *Infinity::CreateApplication()
{
	return new Example;
}