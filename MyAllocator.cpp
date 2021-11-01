// LearnCpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <atomic>
#include <memory>
#include <Windows.h>
#include <vector>
#include <list>

using namespace std;


//实现自己的 Allocator

std::atomic_int g_memory_used(0);

/*****
继承自定义实现 Allocator
****/
template<typename T>
class MyAllocator : public std::allocator<T>
{
public:
	using Base = std::allocator<T>;
	using Pointer = typename std::allocator_traits<Base>::pointer;
	using SizeType = typename std::allocator_traits<Base>::size_type;

public:
	
	SizeType count = 0;

	//类型转换
	template<typename U>
	struct rebind
	{
		using other = MyAllocator<U>;
	};

	//分配内存
	Pointer allocate(SizeType n)
	{	
		count += n;
		std::cout << "管老子要内存来了？  ======>  " << count << std::endl;
		g_memory_used.fetch_add(n * sizeof(T));
		return (Base::allocate(n));
	}

	//回收内存
	void deallocate(Pointer p, SizeType n)
	{
		std::cout << "老子不伺候你了！" << std::endl;
		g_memory_used.fetch_add(n * sizeof(T));
		Base::deallocate(p, n);
	}

	MyAllocator() = default;

	template<typename U>
	MyAllocator(const MyAllocator<U>& other) : Base(other) { }
	~MyAllocator() = default;
};


int main()
{	
	//用vector测试
	std::vector<int, MyAllocator<int>> test_my_allocator(1);

	while (true) {
		Sleep(10);
		test_my_allocator.push_back(1);
	}

	getchar();
}

