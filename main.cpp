#include "main.h"

class Monster;
class Party;

class Monster
{
	public:
		std::string name;
		std::shared_ptr<Party> party;

		Monster(std::string value)
		{
			name  = value;
			party = nullptr;
			std::cout << "Monster '" << value << "' Create!" << std::endl;
		}

		~Monster()
		{
			std::cout << "Destroy Monster..." << std::endl;
			name  = "";
			party = nullptr;
		}

		void setMonsterParty(std::shared_ptr<Party> ptr)
		{
			std::cout << "Set Monster Party." << std::endl;
			party = ptr;
		}

		std::string getMonsterName()
		{
			std::cout << "Get Monster Name.[" << name << "]" << std::endl;
			return name;
		}
};

class Party
{
	public:
		std::string name;
		std::weak_ptr<Monster> mon;

		Party(std::string value)
		{
			name = value;
			std::cout << "Create Party [" << name << "]" << std::endl;
		}

		~Party()
		{
			std::cout << "Destroy Party..." << std::endl;
			name = "";
		}

		void setPartyMonster(std::weak_ptr<Monster> ptr)
		{
			std::cout << "Set Party Monster." << std::endl;
			mon = ptr;
		}

		std::string getPartyName()
		{
			std::cout << "Get Party Name.[" << name << "]" << std::endl;
			return name;
		}
};


int main(int argc, char *argv[])
{
	std::cout << "--- [Main Scope Start] ---" << std::endl;
	{
		std::cout << "--- [Inner Scope Start] ---" << std::endl;

		auto orc   = std::make_shared<Monster>("orc");
		auto party = std::make_shared<Party>("mario_party");

		orc->setMonsterParty(party);
		party->setPartyMonster(orc);

		if(party->mon.expired() == true) {
			std::cout << "Party weak_ptr mon is died..." << std::endl;
		}
		else {
			std::cout << "Party weak_ptr mon is alive." << std::endl;

			std::shared_ptr<Monster> s_monster_ptr = party->mon.lock();
			if(s_monster_ptr == nullptr) {
				std::cout << "s_monster_ptr is nullptr..." << std::endl;
			}
			else {
				std::cout << "s_monster_ptr is shared_ptr." << std::endl;
				s_monster_ptr->getMonsterName();
			}
		}

		std::cout << "--- [Inner Scope End  ] ---" << std::endl;
	}
	std::cout << "--- [Main Scope End  ] ---" << std::endl;

	return 0;
}

