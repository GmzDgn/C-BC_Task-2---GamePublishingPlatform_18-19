//C++ Boot Camp - Task 2 - 2018-19 
//Name: Gamze Dogan
//Student number: b8037358

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}

void DatabaseManager::load_data_ListUser() {
	ifstream fileList;
	string readFile, username, password, email, usertype;
	fileList.open("listOfUsers.txt");

	if (fileList.is_open()) {
		while (getline(fileList, readFile)) {
			istringstream stream(readFile);
			stream >> username >> password >> email >> usertype;
			if (usertype == "admin") {
				add_user(new AdminUser(username, password, email));
			}
			else {
				add_user(new PlayerUser(username, password, email));
			}
		}
		fileList.close();
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_data_ListGame() {
	ifstream fileList;
	Game::GameId gameId;
	string readFile, title, desc;
	fileList.open("listOfGames.txt");

	if (fileList.is_open()) {
		while (getline(fileList, readFile)) {
			istringstream stream(readFile);
			stream >> gameId >> title >> desc;
			add_game(Game(gameId, title, desc));
		}
		fileList.close();
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_data(){
	load_data_ListUser();
	load_data_ListGame();
}

void DatabaseManager::store_data() {

}

//TODO
void DatabaseManager::store_newGame(Game::GameId& id, string& gameTitle, string& description) {
	ofstream outFile;
	outFile.open("listOfGames.txt", ios::app);
}

void DatabaseManager::store_newUser(UserBase::Username& user, string& pw, string& mail, string& type)
{
	ofstream outFile;
	outFile.open("listOfUsers.txt", ios::app);
	string username = user;
	string password = pw;
	string email = mail;
	string usertype = type;

	if (!outFile) {
		cerr << "Textfile doesn't exist!";
	}
	else {
		outFile << username << ' ' << password << ' ' << email << "\n";
		outFile.close();
	}
}

void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
	if (pUser)
	{
		m_users.insert(std::make_pair(pUser->get_username(), pUser));
	}
}

bool DatabaseManager::find_email(const string& mail){
	auto it = m_users.begin();
	for (it; it != m_users.end(); ++it) {
		//FEHLER? 
		if ((it->second->get_email) == mail) {
			return true;
		}
	}
	return false;
}


UserBase* DatabaseManager::find_user(const UserBase::Username& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}
