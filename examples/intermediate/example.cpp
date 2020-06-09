#define INFINITY_ENTRY_POINT
#include <Infinity.h>

#include "example.h"

void Example::OnUserCreate(Infinity::UserCreateEvent *event)
{
	const Infinity::Context *context = event->GetContext();
	context->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set background color to red
}

void Example::OnUserRender(Infinity::UserRenderEvent *event)
{
	const Infinity::Context *context = event->GetContext();
	context->Clear(); // Clear screen
}