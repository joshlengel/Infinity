#pragma once

#include"Core.h"

#include<stdexcept>

namespace Infinity
{
	template <typename T>
	class ResourceFromThis;

	class AnyResource;

	template <typename T>
	class ResourceView;

	template <typename T>
	class Resource
	{
	private:
		T *m_val;

		struct Control
		{
			unsigned int ref_count = 1;
			unsigned int view_count = 0;
		};

		Control *m_control;

		friend AnyResource;
		friend ResourceView<T>;

	public:
		template <typename T, typename ...Args>
		friend Resource<T> MakeResource(Args&&... args);

		template <typename T1, typename T2>
		friend Resource<T1> ResourceCast(const Resource<T2> &resource) noexcept;

		template <typename T1, typename T2>
		friend Resource<T1> ResourceCast(Resource<T2> &&resource) noexcept;

		template <typename T1, typename T2>
		friend Resource<T1> DynamicResourceCast(const Resource<T2> &resource) noexcept;

		template <typename T1, typename T2>
		friend Resource<T1> DynamicResourceCast(Resource<T2> &&resource) noexcept;

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

		Resource(const AnyResource &resource):
			m_val(nullptr),
			m_control(nullptr)
		{
			*this = resource.Get<T>();
		}

		Resource(AnyResource &&resource):
			m_val(nullptr),
			m_control(nullptr)
		{
			*this = resource.Get<T>();

			if (m_control)
			{
				--m_control->ref_count;
			}

			resource.m_val = nullptr;
			resource.m_control = nullptr;
			resource.m_deleter = nullptr;
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

		Resource &operator=(const AnyResource &resource)
		{
			return *this = Resource(resource);
		}

		Resource &operator=(AnyResource &&resource)
		{
			return *this = Resource(std::forward<AnyResource>(resource));
		}

		Resource &operator=(std::nullptr_t)
		{
			TryRelease();
			return *this;
		}

		unsigned int GetRefCount() const  { return m_control? m_control->ref_count : 0; }
		unsigned int GetViewCount() const  { return m_control? m_control->view_count : 0; }

		virtual const T *operator->() const { return m_val; }
		virtual T *operator->() { return m_val;}

		virtual const T &operator*() const { return *m_val; }
		virtual T &operator*() { return *m_val; }

		explicit operator bool() const { return m_val; }

		bool operator!() const { return !m_val; }

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
	inline Resource<T> MakeResource(Args&&... args)
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

	class WrongAnyResourceException : public std::runtime_error
	{
	public:
		WrongAnyResourceException():
			std::runtime_error("Any resource was created with different type. AnyResource::Get<T> must be called with correct type")
		{}
	};

	class AnyResource
	{
	private:
		void *m_val; // any resource

		Resource<char>::Control *m_control;

		void (*m_deleter)(void *val, void *control);

		template <typename T>
		struct Deleter
		{
			static void Delete(void *val, void *control)
			{
				delete (T*)val;
				delete (Resource<T>::Control*)control;
			}
		};

		template <typename T>
		friend class Resource;

	public:
		AnyResource() noexcept:
			m_val(nullptr),
			m_control(nullptr),
			m_deleter(nullptr)
		{}

		AnyResource(std::nullptr_t) noexcept:
			m_val(nullptr),
			m_control(nullptr),
			m_deleter(nullptr)
		{}

		AnyResource(const AnyResource &resource):
			m_val(resource.m_val),
			m_control(resource.m_control),
			m_deleter(resource.m_deleter)
		{
			if (m_control)
			{
				++m_control->ref_count;
			}
		}

		AnyResource(AnyResource &&resource) noexcept:
			m_val(resource.m_val),
			m_control(resource.m_control),
			m_deleter(resource.m_deleter)
		{
			resource.m_val = nullptr;
			resource.m_control = nullptr;
			resource.m_deleter = nullptr;
		}

		template <typename T>
		AnyResource(const Resource<T> &resource):
			m_val(resource.m_val),
			m_control((Resource<char>::Control*)resource.m_control),
			m_deleter(&Deleter<T>::Delete)
		{
			if (resource)
			{
				++m_control->ref_count;
			}
		}

		template <typename T>
		AnyResource(Resource<T> &&resource) noexcept:
			m_val(resource.m_val),
			m_control((Resource<char>::Control*)resource.m_control),
			m_deleter(&Deleter<T>::Delete)
		{
			resource.m_val = nullptr;
			resource.m_control = nullptr;
		}

		template <typename T>
		AnyResource &operator=(const Resource<T> &resource)
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (!m_control->ref_count)
				{
					if (!m_control->view_count)
					{
						m_deleter(m_val, m_control);
					}
					else
					{
						m_deleter(m_val, nullptr);
					}
				}
			}

			m_val = resource.m_val;
			m_control = (Resource<char>::Control*)resource.m_control;
			m_deleter = &Deleter<T>::Delete;

			if (m_control)
			{
				++m_control->ref_count;
			}

			return *this;
		}

		template <typename T>
		AnyResource &operator=(Resource<T> &&resource)
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (!m_control->ref_count)
				{
					if (!m_control->view_count)
					{
						m_deleter(m_val, m_control);
					}
					else
					{
						m_deleter(m_val, nullptr);
					}
				}
			}

			m_val = resource.m_val;
			m_control = (Resource<char>::Control*)resource.m_control;
			m_deleter = &Deleter<T>::Delete;

			resource.m_val = nullptr;
			resource.m_control = nullptr;

			return *this;
		}

		AnyResource &operator=(const AnyResource &resource)
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (!m_control->ref_count)
				{
					if (!m_control->view_count)
					{
						m_deleter(m_val, m_control);
					}
					else
					{
						m_deleter(m_val, nullptr);
					}
				}
			}

			m_val = resource.m_val;
			m_control = (Resource<char>::Control*)resource.m_control;
			m_deleter = resource.m_deleter;

			if (m_control)
			{
				++m_control->ref_count;
			}

			return *this;
		}

		AnyResource &operator=(AnyResource &&resource)
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (!m_control->ref_count)
				{
					if (!m_control->view_count)
					{
						m_deleter(m_val, m_control);
					}
					else
					{
						m_deleter(m_val, nullptr);
					}
				}
			}

			m_val = resource.m_val;
			m_control = (Resource<char>::Control*)resource.m_control;
			m_deleter = resource.m_deleter;

			resource.m_val = nullptr;
			resource.m_control = nullptr;
			resource.m_deleter = nullptr;

			return *this;
		}

		AnyResource &operator=(std::nullptr_t)
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (!m_control->ref_count)
				{
					if (!m_control->view_count)
					{
						m_deleter(m_val, m_control);
					}
					else
					{
						m_deleter(m_val, nullptr);
					}
				}

				m_val = nullptr;
				m_control = nullptr;
				m_deleter = nullptr;
			}

			return *this;
		}

		~AnyResource()
		{
			if (m_control)
			{
				--m_control->ref_count;

				if (!m_control->ref_count)
				{
					if (!m_control->view_count)
					{
						m_deleter(m_val, m_control);
					}
					else
					{
						m_deleter(m_val, nullptr);
					}
				}
			}
		}

		template <typename T>
		Resource<T> Get() const
		{
			if (!m_control) return Resource<T>();

			if (&Deleter<T>::Delete == m_deleter)
			{
				Resource<T> res;
				res.m_val = (T*)m_val;
				res.m_control = (Resource<T>::Control*)m_control;
		
				++m_control->ref_count;

				return res;
			}
			else
			{
				throw WrongAnyResourceException();
			}
		}

		unsigned int GetRefCount() const  { return m_control? m_control->ref_count : 0; }
		unsigned int GetViewCount() const  { return m_control? m_control->view_count : 0; }

		template <typename T>
		operator Resource<T>() const { return Get<T>(); }

		explicit operator bool() const { return !m_val; }

		bool operator!() const { return !m_val; }

		bool operator==(const AnyResource &resource) const { return m_val == resource.m_val; }
		bool operator!=(const AnyResource &resource) const { return m_val != resource.m_val; }
		bool operator>(const AnyResource &resource) const { return m_val > resource.m_val; }
		bool operator<(const AnyResource &resource) const { return m_val < resource.m_val; }
		bool operator>=(const AnyResource &resource) const { return m_val >= resource.m_val; }
		bool operator<=(const AnyResource &resource) const { return m_val <= resource.m_val; }
	};

	template <typename T1, typename T2>
	inline Resource<T1> ResourceCast(const Resource<T2> &resource) noexcept
	{
		Resource<T1> res;
		res.m_val = (T1*)resource.m_val;
		res.m_control = (typename Resource<T1>::Control*)resource.m_control;
		if (res) ++res.m_control->ref_count;
		return res;
	}

	template <typename T1, typename T2>
	inline Resource<T1> ResourceCast(Resource<T2> &&resource) noexcept
	{
		Resource<T1> res;
		res.m_val = (T1*)resource.m_val;
		res.m_control = (typename Resource<T1>::Control*)resource.m_control;
		resource.m_val = nullptr;
		resource.m_control = nullptr;
		return res;
	}

	template <typename T1, typename T2>
	inline Resource<T1> DynamicResourceCast(const Resource<T2> &resource) noexcept
	{
		if (!dynamic_cast<T1*>(resource.m_val)) return Resource<T1>();

		Resource<T1> res;
		res.m_val = (T1*)resource.m_val;
		res.m_control = (typename Resource<T1>::Control*)resource.m_control;
		if (res) ++res.m_control->ref_count;
		return res;
	}

	template <typename T1, typename T2>
	inline Resource<T1> DynamicResourceCast(Resource<T2> &&resource) noexcept
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
	class ResourceView
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

		unsigned int GetRefCount() const  { return m_control? m_control->ref_count : 0; }
		unsigned int GetViewCount() const  { return m_control? m_control->view_count : 0; }

		const T *operator->() const { return m_val; }
		T *operator->() { return m_val;}

		const T &operator*() const { return *m_val; }
		T &operator*() { return *m_val; }

		explicit operator bool() const { return m_control && m_control->ref_count; }

		bool operator==(const ResourceView &resource) const { return m_val == resource.m_val; }
		bool operator!=(const ResourceView &resource) const { return m_val != resource.m_val; }
		bool operator<(const ResourceView &resource) const { return m_val < resource.m_val; }
		bool operator>(const ResourceView &resource) const { return m_val > resource.m_val; }
		bool operator<=(const ResourceView &resource) const { return m_val <= resource.m_val; }
		bool operator>=(const ResourceView &resource) const { return m_val >= resource.m_val; }

		T *Get() const { return m_val; }
	};
}