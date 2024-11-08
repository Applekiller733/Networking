#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unordered_map>
#include "../model/user.h"

class Repository {
	private:
	std::unordered_map <in_addr_t, User> users;
	std::string current_filename = "";

	public:
	Repository();
	Repository(std::string filename);
	~Repository();

	void add_user(in_addr_t ip, User);
	void remove_user(in_addr_t ip);
	User get_user(in_addr_t ip);
	std::unordered_map <in_addr_t, User> get_users();
	void set_filename(std::string filename);
	void save();
	void load();

};
