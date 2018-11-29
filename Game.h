//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358
#pragma once
#include <string>
#include <iostream>

using namespace std;

class Game {
public:
	using GameId = int;
	// Conversion constructor
	Game(const GameId& gameId, const string& title, const string& desc, const double price, const int ageLimit, const string& studio, const int version)
		: m_gameId(gameId)
		, m_title(title)
		, m_description(desc)
		, m_price(price)
		, m_ageLimit(ageLimit)
		, m_studio(studio)
		, m_version(version)
	{}
	//Destructor
	virtual ~Game() {}
	// getter for id
	const Game::GameId get_game_id() const { return m_gameId; }
	// getter for title
	const string get_title() const { return m_title; }
	// getter for price
	const double get_price() const { return m_price; }
	// getter for description
	const string get_desc() const { return m_description; }
	// getter for age limit
	const int get_ageLimit() const { return m_ageLimit; }
	// getter for studio name
	const string get_studio() const { return m_studio; }
	// getter for version of the game
	const int get_version() const { return m_version; }
	// getter to get the counter. How often this game was purchased
	const int get_counterPurchased() const { return counterPurchased; }
	// getter to get the counter. How often this game was played
	const int get_counterPlayed() const { return counterPlayed; }
	// setter for title
	void set_title(const string& val) { m_title = val; }
	// setter for price
	void set_price(const double val) { m_price = val; }
	// setter for description
	void set_desc(const string& val) { m_description = val; }
	// setter for age limit
	void set_ageLimit(const int val) { m_ageLimit = val; }
	// setter for studio name
	void set_studio(const int val) { m_studio = val; }
	// setter for the version
	void set_new_version(const int val) { m_version = val; }
	// after a player purchased the game it increments by one
	void increment_counterPurchased() { ++counterPurchased; }
	// after a player played the game it increments by one
	void increment_counterPlayed() { ++counterPlayed; }

private:
	const GameId m_gameId;	// Numerical identifier for a game  (unique key)
	string m_title; // The game title
	string m_description; // Some information about the game
	string m_studio; // The game studio for this game
	double m_price;	// The price of the game
	int m_ageLimit; // Game can have an age limit
	int m_version; // The version of the game
	int counterPurchased = 0; // the counter for purchases, starts by 0
	int counterPlayed = 0; // the counter, starts by 0
};



