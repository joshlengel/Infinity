#pragma once

#include"Core.h"

#include"Rasterizer.h"

#include"utils/data/Resource.h"

namespace Infinity
{
	class Window;

	INFINITY_TEMPLATE template class INFINITY_API ResourceView<Window>;

	class INFINITY_API ContextSettings
	{
	public:
		enum class DepthMode : unsigned char
		{
			LESS,
			LEQUALS,
			GREATER,
			GEQUALS,
			NEVER,
			ALWAYS
		};

	protected:
		ContextSettings() {}

	public:
		virtual bool Init(DepthMode depth_mode = DepthMode::LEQUALS) = 0;

		virtual ~ContextSettings() {}

		static Resource<ContextSettings> CreateContextSettings();
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<ContextSettings>;

	class INFINITY_API Context
	{
	protected:
		ResourceView<Window> m_window;
		Resource<Rasterizer> m_def_rasterizer;

		Resource<ContextSettings> m_settings;

		Context(const Resource<Window> &window): m_window(window), m_def_rasterizer() {}

	public:
		virtual ~Context() {}

		virtual bool Init() = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;

		virtual void SetContextSettings(const Resource<ContextSettings> &settings) = 0;
		Resource<ContextSettings> GetContextSettings() const { return m_settings; }

		ResourceView<Window> GetWindow() const { return m_window; }
	};

	INFINITY_TEMPLATE template class INFINITY_API Resource<Context>;
}