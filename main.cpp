#include "main.h"

class Monster
{
	public:
		std::string name;
		int pos;

		Monster()
		{
			std::cout << "Monster Create!" << std::endl;
			name = "";
			pos = 0;
		}

		~Monster()
		{
			std::cout << "Destroy Monster...." << std::endl;
			name = "";
			pos = 0;
		}

		void setMonsterName(std::string val)
		{
			name = val;
			std::cout << "My Name is " << name << "." << std::endl;
		}

		std::string getMonsterName()
		{
			return name;
		}

		void moving()
		{
			std::cout << "Moving the Monster: " << (pos + 1) << std::endl;
			pos++;
		}
};


int main(int argc, char *argv[])
{
	std::cout << "--- [Main Scope Start] ---" << std::endl;
	{
		auto orc = std::make_shared<Monster>();
		orc->setMonsterName("orc");
		std::cout << "ref cnt after create '" << orc->getMonsterName() << "'. [" << orc.use_count() << "]" << std::endl;

		{
			std::cout << "    --- [Inner Scope Start] ---" << std::endl;

			auto orc2 = orc;
			std::cout << "    ref cnt after copy orc2. [" << orc2.use_count() << "]" << "[" << orc.use_count() << "]" << std::endl;

			orc2->moving();

			std::cout << "    --- [Inner Scope End  ] ---" << std::endl;
		}

		std::cout << "ref cnt after escape Inner Scope. [" << orc.use_count() << "]" << std::endl;
		orc->moving();
	}
	std::cout << "--- [Main Scope End  ] ---" << std::endl;

	return 0;
}

