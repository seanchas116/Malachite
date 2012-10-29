#ifndef MLCONTAINER_H
#define MLCONTAINER_H

//ExportName: Container

#include "misc.h"

namespace Malachite
{

template <typename T>
QList<const T *> constList(const QList<T *> &list)
{
	return blindCast<QList<const T* > >(list);
}

template <typename T>
QList<T *> nonConstList(const QList<const T *> &list)
{
	return blindCast<QList<T* > >(list);
}

template <typename Container>
class ReverseContainer
{
public:
	
	typedef Container::value_type value_type;
	typedef std::reverse_iterator<Container::const_iterator> const_iterator;
	
	ReverseContainer(Container &container) : _container(&container) {}
	
	const_iterator begin() const { return const_iterator(_container->end()); }
	const_iterator end() const { return const_iterator(_container->begin()); }
	
	int size() const { return _container->size(); }
	void resize(int size) { _container->resize(size); }
	bool empty() const { return _container->empty(); }
	void reserve(int size) { _container->reserve(size); }
	
	value_type &operator[](int i) { return *_container[size() - i - 1]; }
	value_type at(int i) const { return _container->at(size() - i - 1); }
	value_type front() const { return _container->front(); }
	value_type back() const { return _container->back(); }
	
	
	
private:
	
	Container *_container;
};

template <typename Container, typename Predicate>
class FilterContainer
{
public:
	
	typedef Container::value_type value_type;
	
	class const_iterator
	{
	public:
		
		typedef iterator_category std::forward_iterator_tag;
		
		const_iterator() {}
		const_iterator(const const_iterator &other) : _iter(other._iter) {}
		
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
		FilterContainer *_container;
	};
	
	FilterContainer(const Container &container, const Predicate &predicate) :
		_container(&container),
		_predicate(predicate)
	{
		_begin = const_iterator(_container->begin()) + 1;
	}
	
	const_iterator begin() const { return _begin; }
	const_iterator end() const { return const_iterator(_container->end()); }
	
	bool empty() const { return begin() == end(); }
	bool isEmpty() const { return empty(); }
	
	value_type front() const { return *_begin; }
	value_type first() const { return front(); }
	
private:
	
	const Container *_container;
	Predicate _predicate;
	const_iterator _begin;
};

}

#endif // MLCONTAINER_H
