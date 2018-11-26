//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358

#include "MenuSystem.h"

MenuSystem& MenuSystem::instance()
{
	static MenuSystem s_instance;
	return s_instance;
}

void MenuSystem::list_all_games() const
{
	auto gameVisitorLambda = [](const Game& rGame) {
		cout << "ID: " << rGame.get_game_id() << " / " << rGame.get_title() << ": " << rGame.get_desc() << " / Price: " << rGame.get_price() << endl;
	};
	DatabaseManager::instance().visit_games(gameVisitorLambda);
}

int MenuSystem::run_login_screen()
{
	m_pUser = nullptr;

	// in this menu we get the username and password.
	string username;
	string password;

	cout << "Login Menu\n";
	cout << "Username: ";
	cin >> username;

	cout << "Password: ";
	cin >> password;

	// find the user and check password
	auto pUser = DatabaseManager::instance().find_user(username);
	if (pUser && pUser->get_password() == password)
	{
		m_pUser = pUser;
		cout << "Hi " << m_pUser->get_username() << "\n";
	}
	else
	{
		cout << "No such username or password!\n";
	}
	return 0;
}

int MenuSystem::run_admin_user_menu()
{
	AdminUser* pAdminUser = static_cast<AdminUser*>(m_pUser);
	int result = 0;
	do
	{
		cout << "Admin Menu (" << m_pUser->get_username() << ")\n";
		cout << "(1) List All Games\n";
		cout << "(2) List All Users\n";
		cout << "(3) Add Game\n";
		cout << "(4) Add User\n";
		cout << "(5) Modify Game\n";
		cout << "(6) Remove Game\n";
		cout << "(7) View Statistics";
		cout << "(q) Logout\n";

		char option;
		cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': pAdminUser->list_of_users(); break;
		case '3': pAdminUser->add_game(); break;
		case '4': pAdminUser->add_user(); break;
		case '5': modify_game(); break;
		case '6': pAdminUser->remove_game(); break;
		case '7': pAdminUser->view_statistics(); break;
		case 'q': result = -1; break;
		default:  cout << "INAVLID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;
	pAdminUser = nullptr;

	return 0;
}

void MenuSystem::modify_game() {

	//ES gibt eine Methode find_game für das
	Game* m_pGame = nullptr;
	AdminUser* pAdminUser = static_cast<AdminUser*>(m_pUser);

	cout << "Which game do you want to modify?:" << endl;
	list_all_games();
	cout << "Please enter the id of the game to modify: " << endl;
	string gameId;
	cin >> gameId;

	//WENN GEFUNDEN WURDE
	auto pGame = DatabaseManager::instance().find_game(stoi(gameId));
	if (pGame != nullptr) {
		m_pGame = pGame;

		cout << "What do you want to change?" << endl;
		cout << "(1) Description" << endl;
		cout << "(2) Price" << endl;

		char option;
		cin >> option;

		switch (option)
		{
		case '1': pAdminUser->modify_game(m_pGame, 1, stoi(gameId)); 
				  cout << "You changed the description successfully!" << endl << endl; break;
		case '2': pAdminUser->modify_game(m_pGame, 2, stoi(gameId)); 
			      cout << "You changed the price successfully!" << endl << endl; break;
		default:  cout << "INAVLID OPTION\n"; break;
		}
	} else {
		cout << "There is no game with this Id!" << endl;
	}	
}

int MenuSystem::run_player_user_menu()
{
	PlayerUser* pPlayerUser = static_cast<PlayerUser*>(m_pUser);
	int result = 0;
	do
	{
		cout << "Player Menu (" << m_pUser->get_username() << ")\n";
		cout << "Wallet \x9C" << pPlayerUser->get_available_funds() << "\n";
		cout << "(1) List All Games\n";
		cout << "(2) List My Games\n";
		cout << "(3) Buy Game\n";
		cout << "(4) Add Funds\n";
		cout << "(5) Search game\n";
		cout << "(6) Play game\n";
		cout << "(7) Gift another player\n";
		cout << "(q) Logout\n";

		char option;
		cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': pPlayerUser->output_game_list(); break;
		case '3': pPlayerUser->buy_game(); break;
		case '4': pPlayerUser->add_funds(); break;
		case '5': pPlayerUser->search_game_by_title(); break;
		case '6': pPlayerUser->play_game(); break;
		case '7': pPlayerUser->gift_another_player(); break;
		case 'q': result = -1; break;
		default:  cout << "INAVLID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;
	pPlayerUser = nullptr;

	return 0;
}

int MenuSystem::run_login_as_guest()
{
	m_pUser = nullptr;

	// in this menu we get the username and password.
	string email;

	cout << "Login Menu\n";
	cout << "Email address: ";
	cin >> email;

	auto pGuest = DatabaseManager::instance().find_guest(email);
	if (pGuest != nullptr) {
		cout << "Hi guest!" << endl;
		cout << "Maybe you want to sign up and play games?" << endl;
	}
	else {
		//IN DIE FILELISTE EINFÜGEN
	}

	return 0;
}

int MenuSystem::run_unknown_user_menu()
{
	// in this menu we get the username and password.
	int result = 0;

	cout << "Main Menu\n";
	cout << "(1) List All Games\n";
	cout << "(2) Login\n";
	cout << "(3) Login as a guest\n";
	cout << "(q) Quit\n";

	char option;
	cin >> option;

	switch (option)
	{
	case '1': list_all_games(); break;
	case '2': run_login_screen(); break;
	case '3': run_login_as_guest(); break;
	case 'q': result = -1;  break;
	default:  cout << "INAVLID OPTION\n"; break;
	}

	return result;
}

int MenuSystem::run()
{
	int result = 0;
	do
	{
		if (!m_pUser)
		{
			result = run_unknown_user_menu();
		}
		else
		{
			switch (m_pUser->get_user_type())
			{
				case UserTypeId::kPlayerUser: result = run_player_user_menu(); break;
				case UserTypeId::kAdminUser: result = run_admin_user_menu(); break;
				default: result = -1; break;
			}
		}
	} while (result == 0);

	return 0;
}
