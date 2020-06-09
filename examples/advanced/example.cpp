#define INFINITY_ENTRY_POINT
#include <Infinity.h>

#include "example.h"

void Example::OnUserCreate(Infinity::UserCreateEvent *event)
{
	const Infinity::Context *context = event->GetContext();
	context->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set background color to red

	m_renderer = new Infinity::Renderer2D;
}

void Example::OnUserRender(Infinity::UserRenderEvent *event)
{
	const Infinity::Context *context = event->GetContext();
	context->Clear(); // Clear screen

	// Initialize a blue quad to be drawn on the screen
	Infinity::Renderer2D::QuadParams quad = {};
	quad.position = { 0.0f, 0.0f };
	quad.size = { 1.0f, 1.0f };

	quad.color = { 0.0f, 1.0f, 0.0f, 1.0f };

	m_renderer->RenderSingle(quad);
}

void Example::OnUserDestroy(Infinity::UserDestroyEvent *event)
{
	delete m_renderer;
}