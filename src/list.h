#ifndef LIST_H
#define LIST_H

#include <QList>

namespace Malachite
{

template <typename T>
class List : public QList<T>
{
public:
	
	List() : QList<T>() {}
	List(const QList<T> &other) : QList<T>(other) {}
	List(std::initializer_list<T> args) : QList<T>(args) {}
	
#ifdef ML_DEBUG_MEMORY
	
	class iterator
	{
	public:
		
		typedef std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		
		value_type &operator*() const { return _list[_index]; }
		value_type *operator->() const { return &_list[_index]; }
		value_type &operator[](int i) const { return _list[_index + i]; }
		
		bool operator<(const iterator &o) const { return _index < o._index; }
		bool operator<=(const iterator &o) const { return _index <= o._index; }
		
		bool operator>(const iterator &o) const { return _index > o._index; }
		bool operator>=(const iterator &o) const { return _index >= o._index; }
		
		iterator &operator+=(int i) { _index += i; }
		iterator operator+(int i) const { auto ret = *this; ret += i; return ret; }
		
		iterator &operator-=(int i) { _index -= i; }
		iterator operator-(int i) const { auto ret = *this; ret -= i; return ret; }
		
		iterator &operator++() { return *this + 1; }
		iterator operator++(int) { auto ret = *this; ++i; return ret; }
		
		iterator &operator--() { return *this - 1; }
		iterator operator--(int) { auto ret = *this; --i; return ret; }
		
		List *_list = 0;
		int _index = 0;
	};
	
	class const_iterator
	{
	public:
		
		typedef std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		
		const value_type &operator*() const { return _list[_index]; }
		const value_type *operator->() const { return &_list[_index]; }
		const value_type &operator[](int i) const { return _list[_index + i]; }
		
		bool operator<(const const_iterator &o) const { return _index < o._index; }
		bool operator<=(const const_iterator &o) const { return _index <= o._index; }
		
		bool operator>(const const_iterator &o) const { return _index > o._index; }
		bool operator>=(const const_iterator &o) const { return _index >= o._index; }
		
		const_iterator &operator+=(int i) { _index += i; }
		const_iterator operator+(int i) const { auto ret = *this; ret += i; return ret; }
		
		const_iterator &operator-=(int i) { _index -= i; }
		const_iterator operator-(int i) const { auto ret = *this; ret -= i; return ret; }
		
		const_iterator &operator++() { return *this + 1; }
		const_iterator operator++(int) { auto ret = *this; ++i; return ret; }
		
		const_iterator &operator--() { return *this - 1; }
		const_iterator operator--(int) { auto ret = *this; --i; return ret; }
		
		const List *_list = 0;
		int _index = 0;
	};
	
	iterator begin()
	{
		iterator iter = {this, 0};
		return iter;
	}

	const_iterator begin() const
	{
		const_iterator iter = {this, 0};
		return iter;
	}
	
	iterator end()
	{
		iterator iter = {this, size()};
		return iter;
	}
	
	const_iterator end() const
	{
		const_iterator iter = {this, size()};
		return iter;
	}
	
#endif
	
	T head() const { return first(); }
	List tail() const
	{
		List ret = *this;
		ret.removeFirst();
		return ret;
	}
	List init() const
	{
		List ret = *this;
		ret.removeLast();
		return ret;
	}
	
	List take(int count) const
	{
		return mid(0, count);
	}
	List drop(int count) const
	{
		return mid(count, -1);
	}
	List dropRight(int count) const
	{
		return mid(0, size() - count);
	}
	
	template <typename Predicate>
	List filter(Predicate pred) const
	{
		List ret;
		
		for (const T &item : *this)
		{
			if (pred(item))
				ret << item;
		}
		
		return ret;
	}
	
	template <typename Predicate>
	List find(Predicate pred) const
	{
		for (const T &item : *this)
		{
			if (pred(item))
				return {item};
		}
		
		return List();
	}
};

}

#endif // LIST_H
