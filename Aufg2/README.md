### Vorbereitung

#### 1.

 - static: Methoden/Variablen die von dem Klassenobjekt unabhängig verwendet werden (können).
 - public: Zugriff von überall
 - protected: Zugriff nur durch Klasse, Unterklasse und friend Klassen
 - private: Zugriff nur durch Klasse und friend Klassen
 - function pointer: pointer auf eine Funktion mit ganz spezieller Layout ?????
 - std::function<>: "Funktionsklasse" mit speizifiziertem Rückgabewert
 - std::shared_ptr<>: pointer, welcher auch außerhalb der seiner Scope verwendet werden kann
 - std::unique_ptr<>: pointer, welcher nur 1x und innerhalb seiner Scope verwendet werden kann
 - std::weak_ptr<>: pointer, welcher genutzt wird, um temporär shared_ptr zu speichern, falls diese während des trackings verloren gehen

 #### 2.

 ##### Singleton

 -> Objekt bleibt beständig "am Leben", da nur eine Instanz davon erschaffen wird.

 ```c++
class Singleton;

Singleton _instance;

class Singleton {
    private:
        static bool exists = false;
        Singleton();
    public:
        Singleton getInstance() {
            if (!Singleton::exists) {
                _instance = Singleton();
                Singleton::eixts = true;
            }

            return _instance;
        }
};
 ```

 ##### Publish/Subscribe

```c++
#include <map>

typedef void (*subsciberFunc)(std::string);

class Server {
    private:
        unsigned int counter = 0;
        std::map<unsigned int, subscriberFunc> subcribers;
    public:
        void setContent(std::string data) {
            for (auto it = subscribers.begin(); it != subscribers.end(); it++) {
                it->second(data);
            }
        }
        unsigned int subscribe(subsriberFunc sub) {
            unsigned int id = counter++;
            subscribers[i] = sub;
            
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
            my_id = server.subscribe(this->receive);
        }
        void receive(std::string);
};
 ```

 ##### Factory

 ```c++
class Matrix {
    public:
        static Matrix getOnes(size_t rows, size_t cols);
        static Matrix getDiagonal(size_t size, int value);
        static Matrix getUnitMatrix(size_t size);
};
 ```

 ##### MVC

 ```c++
class Model {
    //data
};
class View {
    //visualization
};
class Controller {
    //controlls model and view
}
 ```

 #### Antipatterns

 Verhaltensmuster bei wiederkehrenden Problemen, die auf Vertrautheit beruhen, aber im Zweifel ineffektiv bis schädlich sind.
 Bsp. golden hammer: Man kennt sich mit einem Tool gut aus und denkt, mit diesem alles machen zu können. (Aka. jeder Pythonprogrammierer.)