#ifndef MLCONTAINER_H
#define MLCONTAINER_H

//ExportName: Container

#include "misc.h"

namespace Malachite
{

template <typename T>
QList<const T *> constList(const QList<T *> &list)
{
	return blindCast<const QList<const T* > >(list);
}

template <typename T>
QList<T *> nonConstList(const QList<const T *> &list)
{
	return blindCast<const QList<T* > >(list);
}

template <typename Container>
void forwardShiftContainer(Container &container, int start, int end)
{
	Q_ASSERT(0 <= start && start < container.size());
	Q_ASSERT(0 <= end && end < container.size());
	Q_ASSERT(start < end);
	
	typename Container::value_type v = container[end];
	for (int i = start; i != end; ++i)
		container[i+1] = container[i];
	container[start] = v;
}

template <typename Container>
void backwardShiftContainer(Container &container, int start, int end)
{
	Q_ASSERT(0 <= start && start < container.size());
	Q_ASSERT(0 <= end && end < container.size());
	Q_ASSERT(start < end);
	
	typename Container::value_type v = container[start];
	for (int i = end; i != start; --i)
		container[i-1] = container[i];
	container[end] = v;
}

template <typename Container>
void shiftContainer(Container &container, int start, int end, int count)
{
	if (start == end)
		return;
	
	if (start > end)
	{
		qSwap(start, end);
		count = -count;
	}
	
	count = count % (end - start + 1);
	
	if (count > 0)
	{
		while (count--)
			forwardShiftContainer(container, start, end);
	}
	else
	{
		count = -count;
		while (count--)
			backwardShiftContainer(container, start, end);
	}
}

template <typename Container>
class ReverseContainer
{
public:
	
	typedef typename Container::value_type value_type;
	typedef std::reverse_iterator<typename Container::const_iterator> const_iterator;
	
	ReverseContainer(const Container &container) : _container(container) {}
	
	const_iterator begin() const { return const_iterator(_container.end()); }
	const_iterator end() const { return const_iterator(_container.begin()); }
	
	int size() const { return _container.size(); }
	bool empty() const { return _container.empty(); }
	
	const value_type &operator[](int i) const { return _container[size() - i - 1]; }
	value_type at(int i) const { return _container.at(size() - i - 1); }
	value_type front() const { return _container.back(); }
	value_type back() const { return _container.front(); }
	
private:
	
	const Container _container;
};

template <typename Container>
inline ReverseContainer<Container> reverseContainer(const Container &container)
{
	return ReverseContainer<Container>(container);
}

/*
template <typename Container, typename Predicate>
class FilterContainer
{
public:
	
	typedef Container::value_type value_type;
	
	class const_iterator
	{
	public:
		
		typedef iterator_category std::forward_iterator_tag;
		
		const value_type &operator*() const { return *_iter; }
		const value_type *operator->() const { return _iter.operator->(); }
		
		const_iterator &operator++()
		{
			_iter = std::find_if(_iter, _container->end(), _container->_predicate);
		}
		
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			operator++();
			return tmp;
		}
		
		const_iterator &operator+=(int i) { while (i--) operator++(); }
		
		const_iterator operator+(int i) const
		{
			const_iterator result = *this;
			return result += i;
		}
		
		bool operator==(const const_iterator &other) const { return _iter == other._iter; }
		bool operator!=(const const_iterator &other) const { return _iter != other._iter; }
		
	private:
		
		const_iterator(const Container::const_iterator &iter) : _iter(iter) {}
		
		Container::const_iterator _iter;
		const FilterContainer *_container = 0;
	};
	
	FilterContainer(const Container &container, const Predicate &predicate) :
		_container(container),
		_predicate(predicate)
	{
		_begin = const_iterator(_container.begin()) + 1;
	}
	
	const_iterator begin() const { return _begin; }
	const_iterator end() const { return const_iterator(_container.end()); }
	
	bool empty() const { return begin() == end(); }
	bool isEmpty() const { return empty(); }
	
	value_type front() const { return *_begin; }
	value_type first() const { return front(); }
	
private:
	
	const Container _container;
	Predicate _predicate;
	const_iterator _begin;
};*/

}

#endif // MLCONTAINER_H
