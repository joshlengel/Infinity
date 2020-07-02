#pragma once

#include"Core.h"

namespace Infinity
{
	template <typename T>
	class ResourceFromThis;

	template <typename T>
	class ResourceView;

	template <typename T>
	class INFINITY_API Resource
	{
	private:
		T *m_val;

		struct Control
		{
			unsigned int ref_count = 1;
			unsigned int view_count = 0;
		};

		Control *m_control;

		friend ResourceView<T>;

	public:
		template <typename T, typename ...Args>
		friend Resource<T> INFINITY_API MakeResource(Args&&... args);

		template <typename T1, typename T2>
		friend Resource<T1> INFINITY_API ResourceCast(const Resource<T2> &resource) noexcept;

		template <typename T1, typename T2>
		friend Resource<T1> INFINITY_API ResourceCast(Resource<T2> &&resource) noexcept;

		template <typename T1, typename T2>
		friend Resource<T1> INFINITY_API DynamicResourceCast(const Resource<T2> &resource) noexcept;

		template <typename T1, typename T2>
		friend Resource<T1> INFINITY_API DynamicResourceCast(Resource<T2> &&resource) noexcept;

		Resource() noexcept:
			m_val(nullptr),
			m_control(nullptr)
		{}

		Resource(std::nullptr_t) noexcept:
			Resource()
		{}

		Resource(const Resource &resource):
			m_val(nullptr),
			m_control(nullptr)
		{
			TryConstruct(resource);
		}

		Resource(Resource &&resource) noexcept:
			m_val(resource.m_val),
			m_control(resource.m_control)
		{
			resource.m_val = nullptr;
			resource.m_control = nullptr;
		}

		explicit Resource(const ResourceView<T> &res_view):
			m_val(res_view.m_val),
			m_control(res_view.m_control)
		{
			if (res_view)
			{
				++m_control->ref_count;
			}
			else
			{
				m_val = nullptr;
				m_control = nullptr;
			}
		}

		~Resource()
		{
			TryRelease();
		}

		Resource &operator=(const Resource &resource)
		{
			TryRelease();
			TryConstruct(resource);
			return *this;
		}

		Resource &operator=(Resource &&resource)
		{
			TryRelease();

			m_val = resource.m_val;
			m_control = resource.m_control;
			resource.m_val = nullptr;
			resource.m_control = nullptr;

			return *this;
		}

		Resource &operator=(std::nullptr_t)
		{
			TryRelease();
			return *this;
		}

		virtual const T *operator->() const { return m_val; }
		virtual T *operator->() { return m_val;}

		virtual const T &operator*() const { return *m_val; }
		virtual T &operator*() { return *m_val; }

		explicit operator bool() const { return m_val; }

		bool operator==(const Resource &resource) const { return m_val == resource.m_val; }
		bool operator!=(const Resource &resource) const { return m_val != resource.m_val; }
		bool operator<(const Resource &resource) const { return m_val < resource.m_val; }
		bool operator>(const Resource &resource) const { return m_val > resource.m_val; }
		bool operator<=(const Resource &resource) const { return m_val <= resource.m_val; }
		bool operator>=(const Resource &resource) const { return m_val >= resource.m_val; }

		T *Get() const { return m_val; }

	private:
		void TryRelease()
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (!m_control->ref_count)
				{
					if (!m_control->view_count)
					{
						delete m_control;
					}

					delete m_val;
				}

				m_val = nullptr;
				m_control = nullptr;
			}
		}

		void TryConstruct(const Resource &resource)
		{
			if (resource)
			{
				m_val = resource.m_val;
				m_control = resource.m_control;
				++m_control->ref_count;
			}
		}
	};

	template <typename T, typename ...Args>
	inline Resource<T> INFINITY_API MakeResource(Args&&... args)
	{
		T *val = new T(std::forward<Args>(args)...);
		typename Resource<T>::Control *control = new typename Resource<T>::Control;

		Resource<T> res;
		res.m_val = val;
		res.m_control = control;

		if (std::is_base_of<ResourceFromThis<T>, T>::value)
		{
			((ResourceFromThis<T>*)val)->m_resource_view = ResourceView<T>(res);
		}

		return res;
	}

	class INFINITY_API AnyResource
	{
	private:
		Resource<char> m_resource; // any resource

	public:
		AnyResource():
			m_resource()
		{}

		AnyResource(std::nullptr_t):
			m_resource(nullptr)
		{}

		template <typename T>
		AnyResource(const Resource<T> &resource):
			m_resource(ResourceCast<char>(resource))
		{}

		template <typename T>
		AnyResource(Resource<T> &&resource):
			m_resource(ResourceCast<char>(resource))
		{}

		template <typename T>
		Resource<T> Get() const { return ResourceCast<T>(m_resource); }
	};

	template <typename T1, typename T2>
	inline Resource<T1> INFINITY_API ResourceCast(const Resource<T2> &resource) noexcept
	{
		Resource<T1> res;
		res.m_val = (T1*)resource.m_val;
		res.m_control = (typename Resource<T1>::Control*)resource.m_control;
		if (res) ++res.m_control->ref_count;
		return res;
	}

	template <typename T1, typename T2>
	inline Resource<T1> INFINITY_API ResourceCast(Resource<T2> &&resource) noexcept
	{
		Resource<T1> res;
		res.m_val = (T1*)resource.m_val;
		res.m_control = (typename Resource<T1>::Control*)resource.m_control;
		resource.m_val = nullptr;
		resource.m_control = nullptr;
		return res;
	}

	template <typename T1, typename T2>
	inline Resource<T1> INFINITY_API DynamicResourceCast(const Resource<T2> &resource) noexcept
	{
		if (!dynamic_cast<T1*>(resource.m_val)) return Resource<T1>();

		Resource<T1> res;
		res.m_val = (T1*)resource.m_val;
		res.m_control = (typename Resource<T1>::Control*)resource.m_control;
		if (res) ++res.m_control->ref_count;
		return res;
	}

	template <typename T1, typename T2>
	inline Resource<T1> INFINITY_API DynamicResourceCast(Resource<T2> &&resource) noexcept
	{
		if (!dynamic_cast<T1*>(resource.m_val)) return Resource<T1>();

		Resource<T1> res;
		res.m_val = (T1*)resource.m_val;
		res.m_control = (typename Resource<T1>::Control*)resource.m_control;
		resource.m_val = nullptr;
		resource.m_control = nullptr;
		return res;
	}

	template <typename T>
	class ResourceFromThis
	{
	private:
		ResourceView<T> m_resource_view;

		template <typename T, typename ...Args>
		friend Resource<T> MakeResource(Args&&... args);

	protected:
		Resource<T> GetResourceFromThis() const { return Resource<T>(m_resource_view); }

	public:
		virtual ~ResourceFromThis() {}
	};

	template <typename T>
	class INFINITY_API ResourceView
	{
	private:
		typename Resource<T>::Control *m_control;
		T *m_val;

		friend Resource<T>;

	public:
		ResourceView():
			m_control(nullptr),
			m_val(nullptr)
		{}

		ResourceView(std::nullptr_t):
			ResourceView()
		{}

		ResourceView(const ResourceView &res_view):
			m_control(res_view.m_control),
			m_val(res_view.m_val)
		{
			if (m_control)
			{
				++m_control->view_count;
			}
		}

		ResourceView(ResourceView &&res_view):
			m_control(res_view.m_control),
			m_val(res_view.m_val)
		{
			res_view.m_control = nullptr;
			res_view.m_val = nullptr;
		}

		explicit ResourceView(const Resource<T> &resource):
			m_control(resource.m_control),
			m_val(resource.m_val)
		{
			if (m_control)
			{
				++m_control->view_count;
			}
		}

		~ResourceView()
		{
			if (m_control)
			{
				--m_control->view_count;

				if (!m_control->view_count && !m_control->ref_count)
				{
					delete m_control;
				}
			}
		}

		ResourceView &operator=(const ResourceView &res_view)
		{
			if (m_control)
			{
				--m_control->view_count;

				if (!m_control->view_count && !m_control->ref_count)
				{
					delete m_control;
				}
			}

			m_control = res_view.m_control;
			m_val = res_view.m_val;

			if (m_control)
			{
				++m_control->view_count;
			}

			return *this;
		}

		ResourceView &operator=(ResourceView &&res_view)
		{
			if (m_control)
			{
				--m_control->view_count;

				if (!m_control->view_count && !m_control->ref_count)
				{
					delete m_control;
				}
			}

			m_control = res_view.m_control;
			m_val = res_view.m_val;

			res_view.m_control = nullptr;
			res_view.m_val = nullptr;

			return *this;
		}

		ResourceView &operator=(std::nullptr_t)
		{
			if (m_control)
			{
				--m_control->view_count;

				if (!m_control->view_count && !m_control->ref_count)
				{
					delete m_control;
				}
			}

			m_control = nullptr;
			m_val = nullptr;

			return *this;
		}

		const T *operator->() const { return m_val; }
		T *operator->() { return m_val;}

		const T &operator*() const { return *m_val; }
		T &operator*() { return *m_val; }

		explicit operator bool() const { return m_control && m_control->view_count; }

		bool operator==(const ResourceView &resource) const { return m_val == resource.m_val; }
		bool operator!=(const ResourceView &resource) const { return m_val != resource.m_val; }
		bool operator<(const ResourceView &resource) const { return m_val < resource.m_val; }
		bool operator>(const ResourceView &resource) const { return m_val > resource.m_val; }
		bool operator<=(const ResourceView &resource) const { return m_val <= resource.m_val; }
		bool operator>=(const ResourceView &resource) const { return m_val >= resource.m_val; }

		T *Get() const { return m_val; }
	};
}