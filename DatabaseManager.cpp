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

void DatabaseManager::load_data(){
	ifstream file;
	string readFile, username, password, email, usertype;
	file.open("listOfUsers.txt");

	if (file.is_open()) {
		while (getline(file, readFile)) {
			istringstream stream(readFile);
			stream >> username >> password >> email >> usertype;
			if (usertype == "admin") {
				add_user(new AdminUser(username, password, email));
			}
			else {
				add_user(new PlayerUser(username, password, email));
			}
		}
		file.close();
	} else {
		cerr << "Couldn't open the file!";
	}

	//add_user(new AdminUser("pascalev", "54321", "p.vacher@shu.ac.uk"));

	//// add some players
	//add_user(new PlayerUser("frank", "frank12345", "frank@unknown.com"));

	//// add some games.
	//add_game(Game(4789, "Bounceback", "A platform puzzle game for PSP"));
	//add_game(Game(5246, "Piecefall", "A tetris like 3d puzzle game for PS4"));
}

void DatabaseManager::store_data(UserBase::Username& user, string& pw, string& mail, string& type)
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
		if (it->second->get_email == mail) {
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
