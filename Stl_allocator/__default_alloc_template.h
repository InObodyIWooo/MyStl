#pragma once


#ifndef __DEFAULT_ALLOC_TEMPLATE_H
#define __DEFAULT_ALLOC_TEMPLATE_H

#include "__malloc_alloc_template.h"

namespace mystl
{
	enum {_CHUNK = 8}; //区块单位
	enum {_MAX_BYTES = 128}; //小区块上限
	enum {_NUM_FREELIST = _MAX_BYTES / _CHUNK}; // freelist 个数


	template<int inst>
	class __default_alloc_template {

	private:
		//obj节点某时刻有且仅有一个状态: 
		//	obj.free_list_link	作为节点指针   
		//	obj.data			作为给客户端的数据域  
		union obj
		{
			union obj* free_list_link;	//freelist节点指针
			char data[1];				//数据域
		};

		//volatile : 从内存中取值，而不是从编译器的缓存中取值
		//free_list 维护一大块内存，由内置函数分配，程序结束后释放		内存池相当于free_list的额外空间
		static obj* volatile free_list[_NUM_FREELIST];

	private:
		// 将bytes上调至 _CHUNK的倍数
		static size_t ROUND_UP(size_t bytes) {
			return (((bytes)+_CHUNK - 1) & ~(_CHUNK - 1));
		}

		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+_CHUNK - 1) / _CHUNK - 1);
		}

		//扩充freelist
		static void* refill(size_t n);

		//为pool配置 size个num_objs 大小的内存
		static char* chunk_alloc(size_t size, int& num_objs);

		static char* start_free;	//内存池开始
		static char* end_free;		//内存池结束
		static size_t pool_size;	//内存池大小

	public:
		static void* allocate(size_t n);
		static void deallocate(void* p, size_t n);
		static void* reallocate(void* p, size_t new_size);

	};

	typedef __default_alloc_template<0> alloc;

	template<int inst>
	char *__default_alloc_template<inst>::start_free = 0;
	template<int inst>
	char* __default_alloc_template<inst>::end_free = 0;
	template<int inst>
	size_t __default_alloc_template<inst>::pool_size = 0;

	template<int inst>
	__default_alloc_template<inst>::obj* volatile __default_alloc_template<inst>::free_list[_NUM_FREELIST] 
		= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


	template<int inst>
	void* __default_alloc_template<inst>::allocate(size_t n) {
		if (n > (size_t)_MAX_BYTES)return malloc_alloc::allocate(n);
		obj* volatile* my_freelist;
		obj* result;
		my_freelist = free_list + FREELIST_INDEX(n);
		result = *my_freelist;
		if (result == 0) {
			void* res = refill(n);
			return res;
		}
		*my_freelist = result->free_list_link;
		return (result);
	}

	template<int inst>
	void __default_alloc_template<inst>::deallocate(void* p, size_t n) {
		if (n > (size_t)_MAX_BYTES)return malloc_alloc::deallocate(p, n);
		obj* rtn = (obj*)p;
		obj* volatile* my_freelist;
		my_freelist = free_list + FREELIST_INDEX(n);
		rtn->free_list_link = *my_freelist;
		*my_freelist = rtn;
	}

	template<int inst>
	void* __default_alloc_template<inst>::reallocate(void* p, size_t new_size) {

	}

	template<int inst>
	char* __default_alloc_template<inst>::chunk_alloc(size_t size, int& num_objs) {

	}

	template<int inst>
	void* __default_alloc_template<inst>::refill(size_t n) {

	}

}



#endif // !__DEFAULT_ALLOC_TEMPLATE_H
