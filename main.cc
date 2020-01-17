#include <iostream>
#include <string>

class test 
{
	public:
        test(std::string data);
        std::string getData();
    private:
        std::string data_;
};
test::test(std::string data):
	data_(data)
{
}

std::string test::getData()
{
	return data_;
}
int main()
{
	test thing("Hello World");
	std::cout << thing.getData() << std::endl;
	return 0;
}
