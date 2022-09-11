#include <iostream>
#include <ctime>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

class ID {
	static set<size_t> allids;
	size_t id;
public:
	ID(size_t id) {
		if (ID::allids.find(id) == ID::allids.end()) {
			this->id = id;
			ID::allids.insert(id);
		} else {
			ID(++id);
		}
	}
	size_t get() {
		return this->id;
	}
	~ID() {
		ID::allids.erase(this->id);
	}
};

set<size_t> ID::allids;

class CarAbstract {
public:
	virtual void show()=0;
	virtual size_t getID()=0;
	virtual string getModel()=0;
	virtual size_t getPrice()=0;
	virtual ~CarAbstract() {
	}
};

class Car: public CarAbstract {
	ID *id;
	string model;
	size_t price;
public:
	Car(string model, size_t price) :
			price { price } {
		this->model = model;
		this->id = new ID(rand());
	}
	Car() :
			Car("Nothing", 0) {
		cout << "ERROR: This constructor should never be called!" << endl;
	}

	void show() {
		cout << this->id->get() << endl;
		cout << "Model: " << this->model << endl;
		cout << "Price: " << this->price << endl;
	}
	size_t getID() {
		return this->id->get();
	}
	string getModel() {
		return this->model;
	}
	size_t getPrice() {
		return this->price;
	}
};

class CarSold: public CarAbstract {
	CarAbstract *car;
public:
	CarSold(CarAbstract *_car) {
		this->car = _car;
	}
	void show() {
		cout << "[Sold] ";
		this->car->show();
	}
	size_t getID() {
		return this->car->getID();
	}
	string getModel() {
		return this->car->getModel();
	}
	size_t getPrice() {
		return this->car->getPrice();
	}
};

template<class UnaryPredicate>
list<CarAbstract*> filter(list<CarAbstract*>::iterator start,
		list<CarAbstract*>::iterator end, UnaryPredicate crit) {
	list<CarAbstract*> ret;
	while (start != end) {
		start = find_if(start, end, crit);
		if (start != end) {
			ret.push_back(*start);
			++start;
		}
	}
	return ret;
}

template<class UnaryPredicate>
void sell_by_filter(list<CarAbstract*>::iterator start,
		list<CarAbstract*>::iterator end, UnaryPredicate crit) {
	while (start != end) {
		start = find_if(start, end, crit);
		if (start != end) {
			*start = new CarSold(*start);
			++start;
		}
	}
}

int main() {
	srand(time(NULL));
	vector<string> models;
	models.push_back("Lada Kalina");
	models.push_back("Daewoo Lanos");
	models.push_back("Reault Logan");
	models.push_back("Ford Focus");
	models.push_back("Skoda Octavia");
	models.push_back("Toyota Corolla");

	list<CarAbstract*> carlist(10);
	generate(carlist.begin(), carlist.end(),
			[models]() {
				return new Car(models[rand() % models.size()],
						(90 + rand() % 111) * 1000);
			});
	list<CarAbstract*> filteredlist = filter(carlist.begin(), carlist.end(),
			[](CarAbstract *car) {
				return (car->getModel() == "Ford Focus");
			});
	//carlist.front()->show();
	for (auto c : filteredlist) {
		c->show();
		cout << endl;
	}

	Car *car = new Car("Zaporozhets", 55000);
	car->show();

	CarAbstract *carsold = new CarSold(car);
	carsold->show();

	cout << "---------" << endl;

	sell_by_filter(carlist.begin(), carlist.end(), [](CarAbstract *car) {
		return car->getPrice() < 150000;
	});

	for (auto c : carlist) {
		c->show();
		cout << endl;
	}

	sell_by_filter(filteredlist.begin(), filteredlist.end(),
			[](CarAbstract *c) {
				return true;
			});
	for (auto c : filteredlist) {
		c->show();
		cout << endl;
	}

	return 0;
}
