//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358 
#pragma once

#include <iostream>
#include "DatabaseManager.h"

using namespace std;

class UserBase;

class MenuSystem {
public:
	// Singleton instance definition.
	static MenuSystem& instance();

	// run the system 
	int run();

private:
	// lists all games that are created by the admin
	void list_all_games() const;
	// can modify the game
	void modify_game();
	// run login screen by default
	int run_login_screen();
	// after an admin logs in in this menu runs
	int run_admin_user_menu();
	// after a player logs in in this menu runs
	int run_player_user_menu();
	// after a game studio logs in in this menu runs
	int run_gamestudio_user_menu();
	// after a guest logs in in this menu runs
	int run_login_as_guest();
	// after an unknown user comes this menu runs
	int run_unknown_user_menu();

private:
	UserBase* m_pUser = nullptr;  // the currently logged in user
};

