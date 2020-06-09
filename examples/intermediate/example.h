#include <Infinity.h>

class Example : public Infinity::Application
{
public:
	Example() {}
	~Example() {}

	void OnUserCreate(Infinity::UserCreateEvent *event) override;
	void OnUserRender(Infinity::UserRenderEvent *event) override;
};