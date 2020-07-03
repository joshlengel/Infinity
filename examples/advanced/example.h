#include <Infinity.h>

class StartState : public Infinity::State
{
private:
	Infinity::Resource<Infinity::Renderer2D> m_renderer;

public:
	INFINITY_DECLARE_STATE_FUNCTIONS()
};

class EndState : public Infinity::State
{
private:
	Infinity::Resource<Infinity::Renderer2D> m_renderer;

public:
	INFINITY_DECLARE_STATE_FUNCTIONS()
};