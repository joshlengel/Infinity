#include <Infinity.h>

class Example : public Infinity::Application
{
private:
	Infinity::Renderer2D *m_renderer;

public:
	Example() {}
	~Example() {}

	void OnUserCreate(Infinity::UserCreateEvent *event) override;
	void OnUserRender(Infinity::UserRenderEvent *event) override;
	void OnUserDestroy(Infinity::UserDestroyEvent *event) override;
};