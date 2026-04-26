#include "main.h"

class Monster;
class Party;

class Party
{
	public:
		std::string name;
		std::weak_ptr<Monster> mon;

		std::mutex mtx;
		std::thread::id mtx_id;

		Party(std::string value)
		{
			name = value;
			std::cout << "Create Party [" << name << "]" << std::endl;
		}

		~Party()
		{
			std::cout << "Destroy Party..." << std::endl;
			name = "";
//			mtx.unlock();
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

		int lockPartyMutex()
		{
			if(mtx.try_lock() == false) {
				std::cout << name << ": Mutex Already Lock..." << std::endl;

				std::thread::id crnt_id = std::this_thread::get_id();
				if(mtx_id != std::thread::id() && crnt_id != mtx_id) {
					std::cerr << name << ": other's Lock...[" << crnt_id << ":" << mtx_id << "]" << std::endl;
					return -1;
				}

				return 0;
			}

			mtx_id = std::this_thread::get_id();
			std::cout << name << ": Mutex Lock" << std::endl;
			return 0;
		}

		int unlockPartyMutex()
		{
			std::thread::id crnt_id = std::this_thread::get_id();
			if(mtx_id != std::thread::id() && crnt_id != mtx_id) {
				std::cerr << name << ": other's Lock...[" << crnt_id << ":" << mtx_id << "]" << std::endl;
				return -1;
			}

			mtx.unlock();
			std::cout << name << ": Mutex Unlock" << std::endl;
			return 0;
		}
};

class Monster
{
	public:
		std::string name;
		std::shared_ptr<Party> party;
		std::atomic<int> pos;

		std::mutex mtx;
		std::thread::id mtx_id;

		Monster(std::string value)
		{
			name  = value;
			party = nullptr;
			pos   = 0;
			std::cout << "Monster '" << value << "' Create!" << std::endl;
		}

		~Monster()
		{
			std::cout << "Destroy Monster..." << std::endl;
			name  = "";
			party = nullptr;
			pos   = 0;
			mtx.unlock();
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

		int lockMonsterMutex()
		{
			if(mtx.try_lock() == false) {
				std::cout << name << ": Mutex Already Lock..." << std::endl;

				std::thread::id crnt_id = std::this_thread::get_id();
				if(mtx_id != std::thread::id() && crnt_id != mtx_id) {
					std::cerr << name << ": other's Lock...[" << crnt_id << ":" << mtx_id << "]" << std::endl;
					return -1;
				}

				return 0;
			}

			mtx_id = std::this_thread::get_id();
			std::cout << name << ": Mutex Lock" << std::endl;
			return 0;
		}

		int unlockMonsterMutex()
		{
			std::thread::id crnt_id = std::this_thread::get_id();
			if(mtx_id != std::thread::id() && crnt_id != mtx_id) {
				std::cerr << name << ": other's Lock...[" << crnt_id << ":" << mtx_id << "]" << std::endl;
				return -1;
			}

			mtx.unlock();
			std::cout << name << ": Mutex Unlock" << std::endl;

			return 0;
		}

		int integrationPos()
		{
			int loop_cnt = 10000;
			int idx;

			// Mutex Lock /////////////////////////////////
			{
//				std::lock_guard<std::mutex> lock(mtx);
				for(idx = 0; idx < loop_cnt; idx++) {
					pos++;
				}
			}
			///////////////////////////////////////////////
			std::osyncstream(std::cout) << "integrationPos.[" << pos << "]" << std::endl;

			return pos;
		}

		int proccedure(std::shared_ptr<Party> other)
		{
//			party->lockPartyMutex();
//			std::this_thread::sleep_for(std::chrono::milliseconds(500));
//			other->lockPartyMutex();

			std::scoped_lock lock(party->mtx, other->mtx);
			std::cout << name << ": Mutexlock Success!" << std::endl;

			return 0;
		}
};

int main(int argc, char *argv[])
{
	std::cout << "--- [Main Scope Start] ---" << std::endl;
	{
		std::cout << "--- [Inner Scope Start] ---" << std::endl;

		auto orc   = std::make_shared<Monster>("orc");
		auto goblin= std::make_shared<Monster>("goblin");

		auto party1= std::make_shared<Party>("party1");
		auto party2= std::make_shared<Party>("party2");

		orc->setMonsterParty(party1);
		goblin->setMonsterParty(party2);

		// Mutex Lock //////////////////////////////////////////////
		std::thread worker1(&Monster::proccedure, orc, party2);
		std::thread worker2(&Monster::proccedure, goblin, party1);

		worker1.join();
		worker2.join();
		////////////////////////////////////////////////////////////

		std::cout << "--- [Inner Scope End  ] ---" << std::endl;
	}
	std::cout << "--- [Main Scope End  ] ---" << std::endl;

	return 0;
}

