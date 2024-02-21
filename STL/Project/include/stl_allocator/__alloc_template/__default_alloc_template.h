#pragma once


#ifndef __DEFAULT_ALLOC_TEMPLATE_H
#define __DEFAULT_ALLOC_TEMPLATE_H

#include "__malloc_alloc_template.h"

namespace mystl
{
	enum { _CHUNK = 8 }; //区块单位
	enum { _MAX_BYTES = 128 }; //小区块上限
	enum { _NUM_FREELIST = _MAX_BYTES / _CHUNK }; // freelist 个数


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
		static void* reallocate(void* p, size_t old_size, size_t new_size);

	};

	typedef __default_alloc_template<0> _alloc;

	template<int inst>
	char* __default_alloc_template<inst>::start_free = 0;
	template<int inst>
	char* __default_alloc_template<inst>::end_free = 0;
	template<int inst>
	size_t __default_alloc_template<inst>::pool_size = 0;

	template<int inst>
	typename __default_alloc_template<inst>::obj* volatile __default_alloc_template<inst>::free_list[_NUM_FREELIST]
		= { NULL,NULL,NULL,NULL,
			NULL,NULL,NULL,NULL,
			NULL,NULL,NULL,NULL,
			NULL,NULL,NULL,NULL };


	template<int inst>
	void* __default_alloc_template<inst>::allocate(size_t n) {

		if (n > (size_t)_MAX_BYTES)return _malloc_alloc::allocate(n);
		obj* volatile* my_freelist;
		obj* result;
		my_freelist = free_list + FREELIST_INDEX(n);
		result = *my_freelist;
		if (result == NULL) {
			void* res = refill(ROUND_UP(n));//上调n为_CHUNK的倍数
			return res;
		}
		*my_freelist = result->free_list_link;
		return (result);
	}

	template<int inst>
	void __default_alloc_template<inst>::deallocate(void* p, size_t n) {
		if (n > (size_t)_MAX_BYTES)return _malloc_alloc::deallocate(p, n);
		obj* rtn = (obj*)p;
		obj* volatile* my_freelist;
		my_freelist = free_list + FREELIST_INDEX(n);
		rtn->free_list_link = *my_freelist;
		*my_freelist = rtn;

	}

	template<int inst>
	void* __default_alloc_template<inst>::reallocate(void* p, size_t old_size, size_t new_size) {
		deallocate(p, old_size);
		return allocate(new_size);
	}

	template<int inst>
	void* __default_alloc_template<inst>::refill(size_t n) {
		int nobjs = 20;

		//获取nobjs个n大小区块
		char* chunk = chunk_alloc(n, nobjs);
		if (nobjs == 1)return chunk; //内存池取得一块可用，直接返回给客户端
		//调整freelist

		obj* volatile* my_freelist = free_list + FREELIST_INDEX(n);
		obj* result = (obj*)chunk;//客户端
		obj* current, * next;
		*my_freelist = next = (obj*)(chunk + n);

		for (int i = 1; ; i++) // freelist 塞入nobjs-1个区块，返回给客户端一个
		{
			current = next;
			next = (obj*)((char*)next + n);
			if (i == nobjs - 1)
			{
				current->free_list_link = NULL;
				break;
			}
			else current->free_list_link = next;
		}
		return result;
	}

	template<int inst>
	char* __default_alloc_template<inst>::chunk_alloc(size_t size, int& nobjs) {
		//向内存池取nobjs个n大小区块，如果内存不够，则调用malloc
		//如果malloc失败，则从freelist其他区块整合，调整freelist
		char* result;
		size_t total = size * nobjs;
		size_t pool_left = end_free - start_free;

		if (pool_left >= total)
		{
			result = start_free;
			start_free += total;
			return result;
		}
		else if (pool_left >= size)//至少能提供一个size区块
		{
			nobjs = pool_left / size;
			total = size * nobjs;
			result = start_free;
			start_free += total;
			return result;
		}
		else
		{
			//两倍total加上附加量
			//(pool_size >> 4 猜测: 也许是为了 要多少给多少 且 给内存池加足够的水方便下一次refill使用(防止多次调用chunk_alloc浪费时间
			size_t new_bytes = 2 * total + ROUND_UP(pool_size >> 4);

			if (pool_left > 0)
			{
				//pool_left 一定为_CHUNK的整数倍,因为每次从内存池拿出或加入的大小都是_CHUNK的整数倍,因此保证塞入freelist的也是_CHUNK整数块大小
				obj* volatile* my_freelist = free_list + FREELIST_INDEX(pool_left);
				((obj*)start_free)->free_list_link = *my_freelist;
				*my_freelist = (obj*)start_free;
			}

			start_free = (char*)malloc(new_bytes);
			if (start_free == NULL)
			{
				obj* volatile* my_freelist;
				for (int i = size; i <= _MAX_BYTES; i += _CHUNK)
				{
					my_freelist = free_list + FREELIST_INDEX(i);
					obj* t = *my_freelist;
					if (t != NULL)
					{
						*my_freelist = t->free_list_link;
						start_free = (char*)t;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);//修正nobjs
					}
				}
				end_free = NULL;
				start_free = (char*)_malloc_alloc::allocate(new_bytes);//new_handler机制
			}
			pool_size += new_bytes;
			end_free = start_free + new_bytes;
			return chunk_alloc(size, nobjs);
		}
	}

}



#endif // !__DEFAULT_ALLOC_TEMPLATE_H
