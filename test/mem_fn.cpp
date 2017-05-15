#include <gmock/gmock.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <chrono>

#ifdef _MSC_VER
#define __noinline         __declspec(noinline)
#else
#define __noinline         __attribute__ ((noinline))
#endif

class Employee
{
public:
	Employee() = default;


	__noinline int raise() 
	{
		//std::cout << "Raise\n";
		return 0;
	}

	void setId(int id){
		std::cout << "id=" << id << "\n";
	}
};

TEST(mem_fn, test)
{
	std::vector<Employee> emps;
	emps.push_back(Employee());
	std::for_each(emps.begin(), emps.end(), std::mem_fun_ref(&Employee::raise)); // deprecated. (just like std::mem_fun)
	std::for_each(emps.begin(), emps.end(), std::mem_fn(&Employee::raise));

	std::function<void(Employee*, int)> f = std::mem_fn(&Employee::setId);
	f(&emps[0], 3);
}

TEST(mem_fn, performance)
{
	std::vector<Employee> emps;
	emps.push_back(Employee());

	auto start = std::chrono::high_resolution_clock::now();
	for( size_t i = 0; i < 1000000000; ++i ) {
		std::for_each(emps.begin(), emps.end(), std::mem_fn(&Employee::raise));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto delta = std::chrono::duration_cast< std::chrono::duration< double >>( end - start );
    std::cout << "seconds = " << delta.count() << std::endl;
}

TEST(bind, test)
{
	std::vector<Employee> emps;
	emps.push_back(Employee());
	std::for_each(emps.begin(), emps.end(), std::bind(&Employee::raise, std::placeholders::_1));
	std::for_each(emps.begin(), emps.end(), std::bind(&Employee::setId, std::placeholders::_1, 2));
}

TEST(bind, performance)
{
	//http://stackoverflow.com/questions/3088058/whats-the-point-of-using-boostmem-fn-if-we-have-boostbind

	std::vector<Employee> emps;
	emps.push_back(Employee());

	auto start = std::chrono::high_resolution_clock::now();
	for( size_t i = 0; i < 1000000000; ++i ) {
		std::for_each(emps.begin(), emps.end(), std::bind(&Employee::raise, std::placeholders::_1));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto delta = std::chrono::duration_cast< std::chrono::duration< double >>( end - start );
    std::cout << "seconds = " << delta.count() << std::endl;
}