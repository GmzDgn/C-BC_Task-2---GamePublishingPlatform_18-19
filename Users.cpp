////C++ Boot Camp - Task 2 - 2018-19 
////Name: Gamze Dogan
////Student number: b8037358

#include "Users.h"
#include "DatabaseManager.h"

//------------UserBase------------
UserBase::UserBase(const Username& username, const string& password, const string& email)
	: m_username(username)
	, m_password(password)
	, m_email(email)
{}

UserBase::~UserBase() {}

const string UserBase::get_username() const { 
	return m_username; 
}

const string UserBase::get_password() const {
	return m_password; 
}
void UserBase::set_password(const string& val) {
	m_password = val; 
}

const string UserBase::get_email() const { 
	return m_email; 
}

void UserBase::set_email(const string& val) { 
	m_email = val; 
}

const void UserBase::list_of_games() const {
	auto gameVisitorLambda = [](const Game& rGame) {
		cout << "ID: " << rGame.get_game_id() << " " << rGame.get_title() << ": " << rGame.get_desc() << endl;
	};
	DatabaseManager::instance().visit_games(gameVisitorLambda);
}
//------------UserBase------------


//------------AdminBase------------
const UserTypeId AdminUser::get_user_type() const {
	return UserTypeId::kAdminUser;
}

void AdminUser::add_user() {
	string username, password, email, usertype;
	cout << "Add a new user. Please enter an username: ";
	cin >> username;
	while (!(DatabaseManager::instance().find_user(username) == nullptr)) {
		cout << "This username is already taken. Please choose another one." << endl;
		cin >> username;
	}
	cout << "Enter a password: ";
	cin >> password;
	cout << "Enter an email address: ";
	cin >> email;
	while (DatabaseManager::instance().find_email(email)) {
		cout << "This email is already taken. Please choose another one: ";
		cin >> email;
	}
	cout << "Usertype: admin or player? (a/p): ";
	cin >> usertype;
	while (!(usertype != "a" || usertype != "admin" || usertype != "Admin"
		|| usertype != "p" || usertype != "player" || usertype != "Player")) {
		cout << "Please enter a valid usertype (a/p): ";
		cin >> usertype;
	}
	if (usertype == "a" || usertype == "admin" || usertype == "Admin") {
		usertype = "admin";
	}
	else {
		usertype = "player";
	}
	DatabaseManager::instance().store_newUser(username, password, email, usertype);
}

void AdminUser::add_game() {
	int id;
	double price;
	string title, desc;

	cout << "Add a new game. Please enter a title: ";
	cin >> title;
	cout << "Define a price for this game: ";
	cin >> price;
	cout << "Please enter a short description: ";
	cin.ignore();
	getline(cin, desc);
	DatabaseManager::instance().store_newGame(title, desc, price);
}

void AdminUser::list_of_users() const
{
	auto userVisitorLambda = [](const UserBase& rUser) {
		cout << rUser.get_username() << " " << rUser.get_email() << "\n";
	};

	DatabaseManager::instance().visit_users(userVisitorLambda);
}

void AdminUser::modify_game(Game*& game, const int option, const int gameId) {
	string tmp;

	if (option == 1) {
		cout << "Change the description: ";
		cin >> tmp;
		game->set_desc(tmp);
		DatabaseManager::instance().modify_game(game, "", tmp);
		cout << "You changed the description successfully!" << endl;
	}
	else {
		cout << "Define a new price: ";
		cin >> tmp;
		game->set_price(stod(tmp));
		DatabaseManager::instance().modify_game(game, tmp, "");
		cout << "You changed the price successfully!" << endl;
	}

}

void AdminUser::remove_game() {
	string id;

	cout << "Which game do you want to delete?:" << endl;
	list_of_games();
	cout << "Please enter the id of the game to remove: ";
	cin >> id;

	DatabaseManager::instance().delete_game(id);
}
//------------AdminBase------------


//------------PlayerBase------------
const UserTypeId PlayerUser::get_user_type() const {
	return UserTypeId::kPlayerUser;
}

const PlayerUser::GameList& PlayerUser::get_game_list() const {
	return m_ownedGames;
}

double PlayerUser::get_available_funds() const {
	return m_accountFunds;
}
//------------PlayerBase------------