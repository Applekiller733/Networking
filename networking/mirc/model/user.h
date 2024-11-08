#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

class User{
	private:
	std::string name;
	std::string email;
	std::string location;
	in_addr_t ip;
	int port;

	public:
	User(){
		name = "Unknown";
		email = "Unknown";
		location = "Unknown";
		ip = 0;
		port = 0;
	}
	User(std::string name, std::string email, std::string location,
			in_addr_t ip, int port){
		this->name = name;
		this->email = email;
		this->location = location;
		this->ip = ip;
		this->port = port;
	}

	std::string getName(){
		return name;
	}
	
	std::string getEmail(){
		return email;
	}

	std::string getLocation(){
		return location;
	}

	in_addr_t getIp(){
		return ip;
	}

	int getPort(){
		return port;
	}

	void setName(std::string name){
		this->name = name;
	}

	void setEmail(std::string email){
		this->email = email;
	}

	void setLocation(std::string location){
		this->location = location;
	}

	void setIp(in_addr_t ip){
		this->ip = ip;
	}

	void setPort(int port){
		this->port = port;
	}

	std::string toString(){
		return "Name: " + name + " Email: " + email + " Location: " + location;
	}

};

