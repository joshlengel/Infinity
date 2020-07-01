#pragma once

#include"Core.h"

#include"ArrayList.h"

#include<utility>

namespace Infinity
{
	template <typename Key, typename Value>
	class INFINITY_API Map
	{
	public:
		struct Entry
		{
			Key key;
			Value value;
		};

	private:
		ArrayList<Entry> m_entries;

	public:
		Map():
			m_entries()
		{}

		Map(std::initializer_list<Entry> list):
			m_entries(list)
		{}

		Map(const Map &map):
			m_entries(map.m_entries)
		{}

		Map(Map &&map):
			m_entries(std::move(map.m_entries))
		{}

		Value &operator[](const Key &key)
		{
			for (Entry &e : m_entries)
			{
				if (e.key == key) return e.value;
			}

			Entry n { key };

			m_entries.Add(n);

			return m_entries[m_entries.GetSize() - 1].value;
		}

		Value &operator[](Key &&key)
		{
			for (Entry &e : m_entries)
			{
				if (e.key == key) return e.value;
			}

			Entry n { std::forward<Key>(key) };

			m_entries.Add(std::move(n));

			return m_entries[m_entries.GetSize() - 1].value;
		}

		void Clear()
		{
			m_entries.Clear();
		}

		void Remove(Entry *itr)
		{
			m_entries.Remove(itr);
		}

		const Entry *begin() const { return m_entries.begin(); }
		const Entry *end() const { return m_entries.end(); }
		Entry *begin() { return m_entries.begin(); }
		Entry *end() { return m_entries.end(); }
	};

	template <typename Key, typename Value>
	inline bool INFINITY_API ContainsKey(const Map<Key, Value> &map, const Key &key)
	{
		for (const typename Map<Key, Value>::Entry &e : map)
		{
			if (e.key == key) return true;
		}

		return false;
	}

	template <typename Key, typename Value>
	inline bool INFINITY_API ContainsValue(const Map<Key, Value> &map, const Value &value)
	{
		for (const typename Map<Key, Value>::Entry &e : map)
		{
			if (e.value == value) return true;
		}

		return false;
	}

	template <typename Key, typename Value>
	inline const typename Map<Key, Value>::Entry INFINITY_API *Find(const Map<Key, Value> &map, const Key &key)
	{
		for (const Map<Key, Value>::Entry &e : map)
		{
			if (e.key == key) return &e;
		}

		return map.end();
	}

	template <typename Key, typename Value>
	inline typename Map<Key, Value>::Entry INFINITY_API *Find(Map<Key, Value> &map, const Key &key)
	{
		for (typename Map<Key, Value>::Entry &e : map)
		{
			if (e.key == key) return &e;
		}

		return map.end();
	}

	template <typename Key, typename Value>
	inline void INFINITY_API Remove(Map<Key, Value> &map, const Key &key)
	{
		map.Remove(Find(map, key));
	}
}