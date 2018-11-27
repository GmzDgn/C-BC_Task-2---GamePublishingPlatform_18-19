//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358
#pragma once

#include <string>
#include <list>
#include <map>
#include <iostream>

#include "Game.h"
#include "Stopwatch.h"

using namespace std;

//--
// UserTypeId represents an identifier for the specific user type.
//--
enum class UserTypeId
{
	kInvalid = 0
	, kPlayerUser
	, kAdminUser
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
	// you can add funds
	void add_funds();
	// you can withdraw money from the fund
	void withdraw_funds(const double val);
	// you can add a game to the list
	void add_game_to_list(Game* game);
	// you can get a output of the games that the current player has
	void output_game_list();
	// you can search a game by title
	void search_game_by_title();
	// you can buy a game if your funds are sufficient
	void buy_game();
	// you can choose a game to play
	void play_game();
	// you can gift another player with a game
	void gift_another_player();

	void set_purchased_time(const string& timestemp);

	const string get_purchased_time() const;

	map<Game::GameId, Game*> get_myGames();

	list<string> get_records();

	void push_records(const string & game, const string & date, const string & time, const string & length);

	const int get_age_of_player() const;

	//	virtual ~PlayerUser();

private:
	//GameList m_ownedGames; // List of owned games.
	MyGameList m_myGames;
	RecordList l_records;
	double m_accountFunds = 0.0; // The players available funds.
	int m_age;
	string time;
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

	//	virtual ~AdminUser();
};

class Guest : public UserBase {
	using UserBase::UserBase;
	virtual const UserTypeId get_user_type() const override;
};
