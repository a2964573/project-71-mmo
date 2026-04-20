#include "main.h"

class Car
{
	public:
		int pos = 0;

		Car()
		{
			std::cout << "New Car!" << std::endl;
		}

		~Car()
		{
			std::cout << "Destroy Car..." << std::endl;
		}

		void moving()
		{
			std::cout << "Moving the Car : " << (pos + 1) << std::endl;
			pos++;
		}
};


int main(int argc, char *argv[])
{
	{
		auto mycar = std::make_unique<Car>();
		mycar->moving();

		auto newcar = std::move(mycar);
		newcar->moving();

		if(mycar == nullptr) {
			std::cerr << "mycar is empty..." << std::endl;
		}
	}

	return 0;
}

