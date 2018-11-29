//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358
#pragma once

#include <string>
#include <list>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <iomanip>

#include "Game.h"
#include "Stopwatch.h"

using namespace std;

//--
// UserTypeId represents an identifier for the specific user type.
//--
enum class UserTypeId {
	kInvalid = 0
	, kPlayerUser
	, kAdminUser
	, kGuestUser
	, kGameStudioUser
};

//--
// UserBase represents a user base class for the system.
//--
class UserBase {
public:
	using Username = string;
	// a conversion constructor for the class UserBase. It initializes the data members.
	UserBase(const Username& username, const string& password, const string& email);
	// a destructor to delete an object of type UserBase
	virtual ~UserBase();
	// you can set a new password;
	void set_password(const string& val);
	// you can set a new email;
	void set_email(const string& val);
	// mechanism for identifying the user type at runtime.
	virtual const UserTypeId get_user_type() const = 0;
	// you can get the username of the user
	const string get_username() const;
	// you can get the password of the user
	const string get_password() const;
	// you can get the email of the user
	const string get_email() const;

private:
	const Username m_username; // The users username (unique key)
	string m_password; // Users password
	string m_email; // Users email address
};

//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase {
public:
	// storing all the games that exists
	using RecordList = list<string>;
	using MyGameList = map<Game::GameId, Game*>;
	// calls UserBase constructor and initializes the fund of the player
	PlayerUser(const Username& username, const string& password, const string& email, const int age, const double fund);
	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override;
	// you can get the whole game list
	const PlayerUser::MyGameList& get_game_list() const;
	// you can get the current fund of the player
	const double get_available_funds() const;
	// getter for the purchased time
	const string get_purchased_time() const;
	// returns the age of the player
	const int get_age_of_player() const;
	// returns the list which has all records for the statistics
	const list<string> get_records() const;
	// returns the map. This map contains all purchased games from this user.
	const map<Game::GameId, Game*> get_myGames() const;
	// you can get a output of the games that the current player has.
	void output_game_list();
	// you can add funds
	void add_funds();
	// you can add a game to the list
	void add_game_to_list(Game* game);
	// pushes new recors to the list
	void push_records(const string & game, const string & date, const string & time, const string & length);
	// you can search a game by title
	void search_game_by_title();
	// you can search a game by age limit
	void search_game_by_ageLimit();
	// you can buy a game if your funds are sufficient
	void buy_game();
	// you can choose a game to play
	void play_game();
	// you can gift another player with a game
	void gift_another_player();
	// when an user buys a game, this method sets the date of the purchased time.
	void set_purchased_time(const string& timestemp);
	// you can withdraw money from the fund
	void withdraw_funds(const double val);

private:
	MyGameList m_myGames; // List of owned games
	RecordList l_records; // List of all records
	double m_accountFunds = 0.0; // The players available funds.
	int m_age; // Age of the player
	string time; // time for timestemp
};

//--
// AdminUser represents a system user who has privileges to modify the system.
//--
class AdminUser : public UserBase {
public:
	// inherit the constructor
	using UserBase::UserBase;
	// define the specific user type
	virtual const UserTypeId get_user_type() const override;
	// you can add a user
	void add_user();
	// you can add a game
	void add_game();
	// you can see a list of the current users
	void list_of_users() const;
	// you can modify either the price or the description of a game
	void modify_game(Game*& game, const int option, const int gameId);
	// you can remove a game from the list
	void remove_game();
	// you can view game statistics
	void view_statistics();
};

//--
// Guest represents a system user.
//--
class Guest : public UserBase {
public:
	using UserBase::UserBase;
	virtual const UserTypeId get_user_type() const override;
};

//--
// GameStudio represents a system user.
//--
class GameStudio : public UserBase {
public:
	using GameList = list<Game>;
	using UserBase::UserBase;
	virtual const UserTypeId get_user_type() const override;
	// add game to the list
	void add_game_to_list(const Game& rGame);
	// Sets a new version of the game
	void set_version();
	// Returns the version of the game
	float const get_version(const string& gameId) const;
	// return the game list
	const list<Game> get_gameLIst() const;
	// outputs the list
	const void output_gameList() const;
private:
	GameList l_gameList; // In this list all games from the gamestudio is listet.
};
