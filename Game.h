//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358
#pragma once

#include <string>

using namespace std;

class Game
{
public:
	using GameId = int;

	Game(const GameId& gameId, const string& title, const string& desc, const double price, const int ageLimit)
		: m_gameId(gameId)
		, m_title(title)
		, m_description(desc)
		, m_price(price)
		, m_ageLimit(ageLimit)
	{}

	virtual ~Game() {}

	const Game::GameId get_game_id() const { return m_gameId; }

	const string get_title() const { return m_title; }

	const double get_price() const { return m_price;  }

	const string get_desc() const { return m_description;  }

	const int get_ageLimit() const { return m_ageLimit;  }

	void set_title(const string& val) { m_title = val; }

	void set_price(const double val) { m_price = val;  }

	void set_desc(const string& val) { m_description = val; }

	void set_ageLimit(const int val) { m_ageLimit = val;  }

private:
	const GameId m_gameId;	// Numerical identifier for a game  (unique key).
	string m_title; // The game title.
	string m_description; // Some information about the game.
	double m_price;	// The price of the game.
	int m_ageLimit; // Game can have an age limit.
};



