#define INFINITY_ENTRY_POINT
#include <Infinity.h>

#include "example.h"

void StartState::OnStateEntered(Infinity::StateEnteredEvent &event)
{
	auto context = Infinity::Window::GetContext();
	context->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set background color to red
}

void StartState::OnStateUpdated(Infinity::StateUpdatedEvent &event) {}

void StartState::OnStateRendered(Infinity::StateRenderedEvent &event)
{
	auto context = Infinity::Window::GetContext();
	context->Clear(); // Clear screen
}

void StartState::OnStateExited(Infinity::StateExitedEvent &event) {}

void Infinity::SetClientStartState()
{
	Infinity::SetStartState<StartState>();
}