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
#include <ctime>

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

	void load_list_users();

	void load_list_games();

	void load_game_of_user();

	void load_records();

	// Write all the data to storage.
	void modify_game(Game*& rGame, const string& newPrice, const string& newDescription);

	void modify_user(const string& username, const double newFund);

	void store_newGame(const string& gameTitle, const string& description, const double gamePrice, const int rAgeLimit);

	void store_purchased_game(PlayerUser* rPlayer, Game* rGame);

	void store_newUser(const UserBase::Username& user, const string& pw, const string& mail, const string& type, const int age);

	void store_playedGame_records(PlayerUser* rPlayer, const Game* rGame, const string& tDate, const double tLength);

	void delete_game_of_user(PlayerUser* rPlayer, Game*& rGame);

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	void add_guest(UserBase* pUser);

	bool find_email(const string& mail);

	string getVariable(string& readFile);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);

	UserBase* find_guest(const string& email);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func)
	{
		if (!m_users.empty()) {
			for (auto it : m_users) { func(*it.second); }
			cout << endl;
		}
		else {
			cout << "There is no user in the list." << endl;
		}
	}

	// Adds a game to the db.
	void add_game(Game& rGame);

	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	Game* find_game_by_title(string& title);

	string to_lower_string(string& lowerString);

	void delete_game(const string& gameId);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func)
	{
		if (!m_games.empty()) {
			for (auto it : m_games) { func(it.second); }
		} else {
			cout << "The list is empty. No games are defined yet." << endl;
		}
	}

	string getDate();

	const string getTime() const;

	const map<UserBase::Username, UserBase*> get_all_users() const;


private:
	// Constructors are private for singleton pattern.f
	DatabaseManager();
	~DatabaseManager();


private:
	int gameIdCounter = 0;
	// Types
	using UserContainer = map<UserBase::Username, UserBase*>;
	using GameContainer = map<Game::GameId, Game>;
	using GuestContainer = list<UserBase*>;

	UserContainer m_users;
	GameContainer m_games;
	GuestContainer l_guest;
	string a_records[][4];
};

