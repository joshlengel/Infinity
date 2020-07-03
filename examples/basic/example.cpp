#define INFINITY_ENTRY_POINT
#include <Infinity.h>

class StartState : public Infinity::State
{
public:
	INFINITY_DEFINE_STATE_FUNCTIONS()
};

void Infinity::SetClientStartState()
{
	Infinity::SetStartState<StartState>();
}