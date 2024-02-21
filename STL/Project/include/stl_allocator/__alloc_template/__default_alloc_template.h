#pragma once


#ifndef __DEFAULT_ALLOC_TEMPLATE_H
#define __DEFAULT_ALLOC_TEMPLATE_H

#include "__malloc_alloc_template.h"

namespace mystl
{
	enum { _CHUNK = 8 }; //���鵥λ
	enum { _MAX_BYTES = 128 }; //С��������
	enum { _NUM_FREELIST = _MAX_BYTES / _CHUNK }; // freelist ����


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
			void* res = refill(ROUND_UP(n));//�ϵ�nΪ_CHUNK�ı���
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

		//��ȡnobjs��n��С����
		char* chunk = chunk_alloc(n, nobjs);
		if (nobjs == 1)return chunk; //�ڴ��ȡ��һ����ã�ֱ�ӷ��ظ��ͻ���
		//����freelist

		obj* volatile* my_freelist = free_list + FREELIST_INDEX(n);
		obj* result = (obj*)chunk;//�ͻ���
		obj* current, * next;
		*my_freelist = next = (obj*)(chunk + n);

		for (int i = 1; ; i++) // freelist ����nobjs-1�����飬���ظ��ͻ���һ��
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
		//���ڴ��ȡnobjs��n��С���飬����ڴ治���������malloc
		//���mallocʧ�ܣ����freelist�����������ϣ�����freelist
		char* result;
		size_t total = size * nobjs;
		size_t pool_left = end_free - start_free;

		if (pool_left >= total)
		{
			result = start_free;
			start_free += total;
			return result;
		}
		else if (pool_left >= size)//�������ṩһ��size����
		{
			nobjs = pool_left / size;
			total = size * nobjs;
			result = start_free;
			start_free += total;
			return result;
		}
		else
		{
			//����total���ϸ�����
			//(pool_size >> 4 �²�: Ҳ����Ϊ�� Ҫ���ٸ����� �� ���ڴ�ؼ��㹻��ˮ������һ��refillʹ��(��ֹ��ε���chunk_alloc�˷�ʱ��
			size_t new_bytes = 2 * total + ROUND_UP(pool_size >> 4);

			if (pool_left > 0)
			{
				//pool_left һ��Ϊ_CHUNK��������,��Ϊÿ�δ��ڴ���ó������Ĵ�С����_CHUNK��������,��˱�֤����freelist��Ҳ��_CHUNK�������С
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
						return chunk_alloc(size, nobjs);//����nobjs
					}
				}
				end_free = NULL;
				start_free = (char*)_malloc_alloc::allocate(new_bytes);//new_handler����
			}
			pool_size += new_bytes;
			end_free = start_free + new_bytes;
			return chunk_alloc(size, nobjs);
		}
	}

}



#endif // !__DEFAULT_ALLOC_TEMPLATE_H
