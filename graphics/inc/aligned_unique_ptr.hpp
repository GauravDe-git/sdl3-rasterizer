#pragma once
#include <new> // std::bad_alloc
#include <memory>
#include <type_traits>

#ifdef _MSC_VER 
	#define aligned_malloc(size, alignment) _aligned_malloc( (size), (alignment) )
	#define aligned_free					_aligned_free
#else
	#include <cstdlib>
	#define aligned_malloc(size, alignment) std::aligned_alloc((alignment), (size))
	#define aligned_free					std::free 
#endif

struct aligned_deleter
{
	void operator()( void* ptr) const
	{
		aligned_free(ptr);
	}
};

template<typename T>
using aligned_unique_ptr = std::unique_ptr<T, aligned_deleter>;

class bad_aligned_alloc: public std::bad_alloc
{
public:
	const char* what() const noexcept override
  {
		return "aligned memory allocation failed";
  }
};

template<typename T, std::size_t Align>
aligned_unique_ptr<T> make_aligned_unique( std::size_t n)
{
	using T2 = std::remove_extent_t<T>;
	aligned_unique_ptr<T> ptr = aligned_unique_ptr<T>( static_cast<T2*>( aligned_malloc(sizeof(T2) * n, Align)), aligned_deleter());
	if (!ptr)
		throw bad_aligned_alloc();

	// Default construct the elements.
	T2* p = ptr.get();
	for (std::size_t i = 0; i < n; ++i)
		new(p++) T2();
	return ptr;
}