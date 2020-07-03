#define INFINITY_ENTRY_POINT
#include <Infinity.h>

#include "example.h"

// ------------------ Start state --------------------------
void StartState::OnStateEntered(Infinity::StateEnteredEvent &event)
{
	auto context = Infinity::Window::GetContext();
	context->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set background color to red

	m_renderer = Infinity::MakeResource<Infinity::Renderer>();

	if (!m_renderer.Init())
	{
		INFINITY_CLIENT_ERROR("Error initializing renderer");
		RequestExit();
	}
}

void StartState::OnStateUpdated(Infinity::StateUpdatedEvent &event) {}

void StartState::OnStateRendered(Infinity::StateRenderedEvent &event)
{
	auto context = Infinity::Window::GetContext();
	context->Clear(); // Clear screen

	m_renderer.StartScene(nullptr);

	// Initialize a blue quad to be drawn on the screen
	Infinity::Renderer2D::QuadParams quad;
	quad.position = { 0.0f, 0.0f };
	quad.size = { 1.0f, 1.0f };

	quad.color = { 0.0f, 0.0f, 1.0f, 1.0f };

	m_renderer->DrawQuad(quad);

	m_renderer.EndScene();
}

void StartState::OnStateExited(Infinity::StateExitedEvent &event)
{
	auto &resources = event.GetResources();
	resources["renderer"] = m_renderer; // Give renderer to next state

	event.SetNextState<EndState>();
}

// -------------------- End state --------------------------
void EndState::OnStateEntered(Infinity::StateEnteredEvent &event)
{
	auto &resources = event.GetResources();
	m_renderer = resources["renderer"].Get<Infinity::Renderer2D>();
}

void EndState::OnStateUpdated(Infinity::StateUpdatedEvent &event) {}

void EndState::OnStateRendered(Infinity::StateRenderedEvent &event)
{
	auto context = Infinity::Window::GetContext();
	context->Clear(); // Clear screen

	m_renderer.StartScene(nullptr);

	// Initialize a green quad to be drawn on the screen
	Infinity::Renderer2D::QuadParams quad;
	quad.position = { 0.0f, 0.0f };
	quad.size = { 1.0f, 1.0f };

	quad.color = { 0.0f, 1.0f, 0.0f, 1.0f };

	m_renderer->DrawQuad(quad);

	m_renderer.EndScene();
}

void EndState::OnStateExited(Infinity::StateExitedEvent &event) {}

// Tell infinity which state to start with
void Infinity::SetClientStartState()
{
	Infinity::SetStartState<StartState>();
}