#pragma once

#include "Arena.h"
#include "boost\pool\pool.hpp"
#include "boost\pool\singleton_pool.hpp"
#include "MemoryPool.h"
/*
²��O����t�m���A�Ω�޲zŪ��Wz�ɪ��O�������
@By Wanger.
*/

/*
[+]
for object allocation
*/

//#define AllocObj(instance_type) new instance_type()
//#define FreeObj(pointer) delete pointer
#define AllocObj(instance_type) WvsSingleObjectAllocator<instance_type, (sizeof(instance_type) > 512)>::GetInstance()->Allocate(sizeof(instance_type))
#define AllocObjCtor(instance_type) WvsSingleObjectAllocator<instance_type, (sizeof(instance_type) > 512)>::AllocateWithCtor
#define FreeObj(pointer) WvsSingleObjectAllocator<std::remove_reference<decltype(*pointer)>::type, (sizeof(*pointer) > 512)>::GetInstance()->Free(pointer, (sizeof(*pointer)))
#define FreeObj_T(T, pointer) WvsSingleObjectAllocator<T, (sizeof(T) > 512)>::GetInstance()->Free(pointer, sizeof(T))

#define AllocArray(instance_type, nSize) (instance_type*)WvsArrayAllocator::GetInstance()->Allocate<char>(nSize * sizeof(instance_type))
#define FreeArray(pointer, nSize) WvsArrayAllocator::GetInstance()->Free<char>(pointer, nSize * sizeof(*pointer))

class WzMemoryPoolMan
{
private:

	memt::Arena *pArena = new memt::Arena();
public:
	static WzMemoryPoolMan* GetInstance() 
	{
		static WzMemoryPoolMan* pInstance = new WzMemoryPoolMan;
		return pInstance;
	}

	WzMemoryPoolMan() {};

	/*static MemoryPoolMan *GetInstance()
	{
		static MemoryPoolMan *sMemoryPool = new MemoryPoolMan();
		return sMemoryPool;
	}*/

	//�t�m�@�ӫ��Ъ���A�j�p��size (byte)
	void* AllocateObject(int size)
	{
		return pArena->alloc(size);
	}

#pragma warning(disable:4312)  
#pragma warning(disable:4311)  
#pragma warning(disable:4302) 
	//�t�m�@�Ӱ}�C�A���׬�len bytes
	void* AllocateArray(int len)
	{
		//return singleton_char_pool::ordered_malloc(len);

		auto allocPair = pArena->allocArray<char>(len + 4);
 
		//�j���end�g�b�e�|��bytes
		*((int*)allocPair.first) = (int)allocPair.second;
		return allocPair.first + 4;
	}
#pragma warning(default:4302)  
#pragma warning(disable:4311)  
#pragma warning(disable:4312)  

	//�N���w�� ptr (�@�Ӫ���)  �P��
	void DestructObject(void* ptr)
	{
		pArena->freeTop(ptr);
	}

	//�N���w�� ptr (�@�Ӱ}�C) �P���A�䤤�}�C���׺ݦ�m�O���b���w���Ъ��e�|��byte
	void DestructArray(void* ptr)
	{
		//singleton_char_pool::ordered_free(ptr);
		//g_ptrMemPool->FreeMemory((char*)ptr - 4, (int)(*(int*)(((char*)ptr) - 4)));
		pArena->freeTopArray(((char*)((char*)ptr) - 4), (char*)(*(int*)(((char*)ptr) - 4)));
	}

	//����O�Ц��A�N�Ŷ��k�ٵ�OS
	void Release()
	{
		pArena->freeAllAllocsAndBackingMemory();
	}
};