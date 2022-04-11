#include <map>
#include <iostream>
#include <functional>

class Server {
    private:
        unsigned int counter = 0;
        std::map<unsigned int, std::function<void(std::string)>> subscribers;
    public:
        void setContent(std::string data) {
            for (auto it = subscribers.begin(); it != subscribers.end(); it++) {
                it->second(data);
            }
        }
        unsigned int subscribe(std::function<void(std::string)> sub) {
            unsigned int id = counter++;
            subscribers[id] = sub;
            
            return id;
        }
        bool unsubscribe(unsigned int id) {
            if (subscribers.contains(id)) {
                subscribers.erase(id);
                return true;
            } else {
                return false;
            }
        }
};

class Publisher {
    private:
        Server server;
    public:
        void registerServer(Server &server) {
            this->server = server;
        }
        void main() {
            while (true) {
                server.setContent("Hello World!");
            }
        }
};

class Subscriber {
    private:
        unsigned int my_id;
    public:
        Subscriber(Server server) {
            std::function<void(std::string)> func(this->receive);
            my_id = server.subscribe(func);
        }
        void receive(std::string);
};