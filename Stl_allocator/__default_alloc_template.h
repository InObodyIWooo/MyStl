#pragma once


#ifndef __DEFAULT_ALLOC_TEMPLATE_H
#define __DEFAULT_ALLOC_TEMPLATE_H

#include "__malloc_alloc_template.h"

namespace mystl
{
	enum {_CHUNK = 8}; //���鵥λ
	enum {_MAX_BYTES = 128}; //С��������
	enum {_NUM_FREELIST = _MAX_BYTES / _CHUNK}; // freelist ����


	template<int inst>
	class __default_alloc_template {

	private:
		//obj�ڵ�ĳʱ�����ҽ���һ��״̬: 
		//	obj.free_list_link	��Ϊ�ڵ�ָ��   
		//	obj.data			��Ϊ���ͻ��˵�������  
		union obj
		{
			union obj* free_list_link;	//freelist�ڵ�ָ��
			char data[1];				//������
		};

		//volatile : ���ڴ���ȡֵ�������Ǵӱ������Ļ�����ȡֵ
		//free_list ά��һ����ڴ棬�����ú������䣬����������ͷ�		�ڴ���൱��free_list�Ķ���ռ�
		static obj* volatile free_list[_NUM_FREELIST];

	private:
		// ��bytes�ϵ��� _CHUNK�ı���
		static size_t ROUND_UP(size_t bytes) {
			return (((bytes)+_CHUNK - 1) & ~(_CHUNK - 1));
		}

		static size_t FREELIST_INDEX(size_t bytes) {
			return (((bytes)+_CHUNK - 1) / _CHUNK - 1);
		}

		//����freelist
		static void* refill(size_t n);

		//Ϊpool���� size��num_objs ��С���ڴ�
		static char* chunk_alloc(size_t size, int& num_objs);

		static char* start_free;	//�ڴ�ؿ�ʼ
		static char* end_free;		//�ڴ�ؽ���
		static size_t pool_size;	//�ڴ�ش�С

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
