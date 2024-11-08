#include "repository.h"
#include <fstream>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>

Repository::Repository()
	{
		
	}

Repository::Repository(std::string filename){
		this->current_filename = filename;
	}
void Repository::set_filename(std::string filename){
		this->current_filename = filename;
	}

void Repository::add_user(in_addr_t addr, User user){
		this->users[addr] = user;
	}

void Repository::remove_user(in_addr_t addr){
		if (this->users.find(addr) == this->users.end()){
			return;
		}
		this->users.erase(addr);
	}

User Repository::get_user(in_addr_t addr){
		if (this->users.find(addr) == this->users.end()){
			return User();
		}
		return this->users[addr];
	}

std::unordered_map <in_addr_t, User> Repository::get_users(){
		return this->users;
	}

void Repository::save(){
		std::ofstream file;
		file.open(this->current_filename);
		for (auto it = this->users.begin(); it != this->users.end(); ++it){
			//wip
		}
		file.close();
	}

void Repository::load(){
		std::ifstream file;
		std::unordered_map <in_addr_t, User> newusers;
		file.open(this->current_filename);
		std::string line;
		while (std::getline(file, line)){/* WIP
			std::istringstream iss(line);
			std::string username, email, location;
			std::string saddr, sport;
			in_addr_t addr;
			int port;
			iss >> saddr >> sport >> username >> email >> location;
			addr = inet_addr(saddr.c_str());
			port = std::stoi(sport);
			User user(username, email, location, addr, port);
			newusers[addr] = user;*/
		}
		this->users = newusers;
		file.close();
	}

Repository::~Repository(){
		this->save();
	}
	

