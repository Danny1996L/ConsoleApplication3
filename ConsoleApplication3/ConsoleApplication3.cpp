#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

void attackMonster();
void attackPlayer();

class Creature
{
protected:
	std::string m_name;
	char m_simbol;
	int m_HitPoints;
	int m_damage;
	int m_gold;
public:
	Creature(std::string name, char simbol, int HitPoints, int damage, int gold) : m_name(name), m_simbol(simbol), m_HitPoints(HitPoints),
		m_damage(damage), m_gold(gold) {}
	std::string getName() { return m_name; }
	char getSymbol() { return m_simbol; }
	int getHitPoints() { return m_HitPoints; }
	int getDamage() { return m_damage; }
	int  getGold() { return m_gold; }
	void reduceHelth(int damage)
	{
		m_HitPoints -= damage;
	}
	bool isDead()
	{
		if (m_HitPoints <= 0)
			return true;
		else
			return false;
	}
	void addGold(int gold)
	{
		m_gold += gold;
	}
};

class Player : public Creature
{
private:
	int m_level;
public:
	Player(std::string name = "", int level = 1, char simbol = '@', int HitPoints = 10, int damage = 1, int gold = 0) : Creature(name, simbol, HitPoints, damage, gold),
		m_level(level)
	{

	}
	void leveUp()
	{
		m_level += 1;
		m_damage += 1;
	}
	int getLevel() { return m_level; }
	bool hasWon()
	{
		if (m_level == 20)
			return true;
		else
			return false;
	}
};

class Monster : public Creature
{
public:
	enum Type
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES
	};
private:
	struct MonsterData
	{
		std::string name;
		char simbol;
		int health;
		int damage;
		int gold;
	};
	static MonsterData monsterData[MAX_TYPES];
public:
	Monster(Type type) : Creature(monsterData[type].name, monsterData[type].simbol,
		monsterData[type].health, monsterData[type].damage, monsterData[type].gold) {}
	static Type getRandomMonster()
	{
		int random = getRandomNumber(0, (MAX_TYPES - 1));
		return static_cast<Type>(random);
	}

};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};

char fightMonster()
{
tryAgain:
	std::cout << "(R) run or (F) fight ? ";
	char answer;
	std::cin >> answer;
	switch (answer)
	{
	case 'R':
		return 'R';
		break;
	case 'F':
		return 'F';
		break;
	default:
		goto tryAgain;
		break;
	}
}

void attackMonster(Player& p, Monster& m)
{
	m.reduceHelth(p.getDamage());
	if (m.getHitPoints() == 0)
	{
		p.leveUp();
		p.addGold(m.getGold());
	}
	else
	{
		p.reduceHelth(m.getDamage());
	}
}

void attackPlayer(Player& p, Monster& m)
{
	std::cout << "Your health before hitting = " << p.getHitPoints() << std::endl;
	p.reduceHelth(m.getDamage());
	std::cout << "Y have been damage = " << m.getDamage() << " the amount of your health = " << p.getHitPoints() << std::endl;
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	rand();

	std::string playerName;
	std::cout << "Enter your name: ";
	std::cin >> playerName;
	Player p1(playerName);
	std::cout << "Welcome, " << p1.getName() << std::endl;
	Monster m1 = Monster::getRandomMonster();
	std::cout << "You have encountered a " << m1.getName() << std::endl;
	while (!p1.hasWon()) // нормальное условие
	{
		if (p1.getHitPoints() <= 0)
		{
			std::cout << "Your lost\n"; // вы проиграли
			goto EXIT;
		}
		switch (fightMonster())
		{
		case 'R':
			if (getRandomNumber(1, 2) == 1)
			{
				std::cout << "You successfully fled.\n"; // успешно сбежали
				m1 = Monster::getRandomMonster();
				std::cout << "You have encountered a " << m1.getName() << std::endl; // имя вашего соперника
			}
			else if (getRandomNumber(1, 2) == 2)
			{
				attackPlayer(p1, m1);
			}
			break;
		case 'F':
			attackMonster(p1, m1);
			break;
		default:
			break;
		}
	}

	{
	EXIT:
		std::cout << "You`ve earned : " << p1.getGold() << std::endl;
		std::cout << "You have reached : " << p1.getLevel() << std::endl;
	}
	return 0;
}