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

		bool ContainsKey(const Key &key) const
		{
			for (const Entry &e : m_entries)
			{
				if (e.key == key) return true;
			}

			return false;
		}

		const Entry *find(const Key &key) const
		{
			for (const Entry &e : m_entries)
			{
				if (e.key == key) return &e;
			}

			return end();
		}

		Value &operator[](const Key &key)
		{
			for (Entry &e : m_entries)
			{
				if (e.key == key) return e.value;
			}

			Entry n;
			n.key = key;

			m_entries.Add(n);

			return m_entries[m_entries.GetSize() - 1].value;
		}

		void Clear()
		{
			m_entries.Clear();
		}

		const Entry *begin() const { return m_entries.begin(); }
		const Entry *end() const { return m_entries.end(); }
		Entry *begin() { return m_entries.begin(); }
		Entry *end() { return m_entries.end(); }
	};
}