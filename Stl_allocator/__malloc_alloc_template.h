#pragma once

#ifndef __MALLOC_ALLOC_TEMPLATE_H
#define __MALLOC_ALLOC_TEMPLATE_H

#if 0
#include <new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include <iostream>
#define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(0);
#endif

namespace mystl
{
	template<int inst>
	class __malloc_alloc_template {

	private:
		//oom : out of memory
		typedef void(*handler)();
		static void* oom_malloc(size_t);
		static void* oom_realloc(void*, size_t);

		static void (*__malloc_alloc_oom_handler)();

	public:

		static void* allocate(size_t n) {
			void* result = malloc(n);
			if (result == 0) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void* p) {
			free(p);
		}

		static void* reallocate(void* p, size_t new_size) {
			void* result = realloc(p, new_size);
			if (result == 0)result = oom_realloc(p, new_size);
			return result;
		}


		template<int inst>
		void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;


		static handler set_malloc_hander(handler f) {
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return (old);
		}

		//Ä£Äâ	new-handler

		template<int inst>
		static void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {

			void (*my_malloc_handler)();
			void* result;

			for (;;) {
				my_malloc_handler = __malloc_alloc_oom_handler;
				if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
				(*my_malloc_handler)();
				result = malloc(n);
				if (result) return result;
			}
		}

		template<int inst>
		static void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
			void(*my_malloc_handler)();
			void* result;
			for (;;) {
				my_malloc_handler = __malloc_alloc_oom_handler;
				if (my_malloc_handler == nullptr) { __THROW_BAD_ALLOC; }
				result = realloc(p, n);
				if (result) return result;
			}
		}
	};

	typedef __malloc_alloc_template<0> malloc_alloc;
}

#endif // !__MALLOC_ALLOC_TEMPLATE_H
