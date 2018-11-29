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
class DatabaseManager {
public:
	// Singleton instance definition.
	static DatabaseManager& instance();
	// Loads all data from the files.
	void load_data();
	// Loads the users from the file.
	void load_list_users();
	// Loads the games from the file.
	void load_list_games();
	// Loads games from the users from the file.
	void load_game_of_user();
	// Loads all records from the file for the statistics.
	void load_records();
	// Stores new users to the file.
	void store_newUser(const UserBase::Username& username, const string& password, const string& email, const string& usertype, const int age);
	// Stores new games to the file.
	void store_newGame(const string& gameTitle, const string& description, const double gamePrice, const int ageLimit, const string& studio);
	// Stores new purchased games to the file.
	void store_purchased_game(PlayerUser* rPlayer, const Game* rGame);
	// Stores the record of a played game (e.g. length of play) to the file.
	void store_playedGame_records(PlayerUser* rPlayer, Game* rGame, const string& date, const double length);
	// Modifies the game file (e.g. new description defined for a game).
	void modify_game(Game*& pGame, const string& newPrice, const string& newDescription, const string& newVersion);
	// Modifies the user file.
	void modify_user(const UserBase::Username& username, const double newFund);
	// deletes the game from the listOfUserGames file.
	void delete_game_from_listOfUserGames(const string& gameId);
	// deletes the particullary game from listOfGames file.
	void delete_game(const string& gameId);
	// Adds a user to the to file and to the map.
	void add_user(UserBase* pUser);
	// Adds a guest to the list.
	void add_guest(UserBase* pUser);
	// Adds a game to the file.
	void add_game(const Game& rGame);
	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);
	// Finds a guest by email address, return nullptr if the guest is not found.
	UserBase* find_guest(const string& email);
	// Looks whether there is an email address yet or not. 
	bool find_email(const string& email);
	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);
	// Finds the game by title, returns nullptr if the game is not found.
	Game* find_game_by_title(const string& title);
	// Finds the game by the agelimit.
	void find_game_by_ageLimit(const int age);
	// Getter for the map to get all users. 
	const map<UserBase::Username, UserBase*> get_all_users() const;
	// gets the token from the file
	string getVariable(string& readFile);
	// get the current date and returns it as a string.
	const string getDate() const;
	// get the current time and returns it as a string.
	const string getTime() const;
	// returns the map with the games
	const map<Game::GameId, Game> get_gameContainer() const;
	// to lower the string.
	string to_lower_string(const string& lowerString);
	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func) {
		if (!m_users.empty()) {
			for (auto it : m_users) { func(*it.second); }
			cout << endl;
		} else {
			cout << "There is no user in the list." << endl;
		}
	}
	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func) {
		if (!m_games.empty()) {
			for (auto it : m_games) { func(it.second); }
		} else {
			cout << "The list is empty. No games are defined yet." << endl;
		}
	}

private:
	// Constructors are private for singleton pattern.f
	DatabaseManager();
	~DatabaseManager();

private:
	// Types
	using UserContainer = map<UserBase::Username, UserBase*>;
	using GameContainer = map<Game::GameId, Game>;
	using GuestContainer = list<UserBase*>;

	int gameIdCounter = 0; // Id counter for the games
	UserContainer m_users; // Container for all users
	GameContainer m_games; // Container for all games
	GuestContainer l_guest; // List for all guests
};