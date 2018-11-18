//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358
#pragma once

#include "Game.h"
#include "Users.h"
#include "MenuSystem.h"

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//--
// DatabaseManager is the one central database for the whole system
// In this example it has been designed using the singleton pattern.
//--
class DatabaseManager
{
public:
	// Singleton instance definition.
	static DatabaseManager& instance();

	// Initialize the database from storage.
	void load_data();

	void load_data_ListUser();

	void load_data_ListGame();

	// Write all the data to storage.
	void store_data();

	void store_newUser(UserBase::Username& username, string& password, string& email, string& usertype);

	void store_newGame(Game::GameId& id, string& gameTitle, string& description);

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	// 
	bool find_email(const string& mail);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// Adds a game to the db.
	void add_game(Game& rGame);

	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(it.second); }
	}

private:
	// Constructors are private for singleton pattern.f
	DatabaseManager();
	~DatabaseManager();


private:
	// Types
	using UserContainer = map<UserBase::Username, UserBase*>;
	using GameContainer = map<Game::GameId, Game>;

	UserContainer m_users;
	GameContainer m_games;

};

