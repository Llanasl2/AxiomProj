#include <iostream>
#include <string>
using namespace std;

class Fruit
{
	//protected variable only made to be accessed by itself and child classes
protected:
	string fruit_type;
public:
	///Constructor
	Fruit()
	{
		fruit_type = "Fruit";
	}
	//Identify function to be inhereted by children
	void Identify()
	{
		cout << "I am a " << fruit_type << endl;
	}
};

//Child classes only need constructor function
class Banana : public Fruit
{
public:
	Banana()
	{
		fruit_type = "Banana";
	}
};

class Tangerine : public Fruit
{
public:
	Tangerine()
	{
		fruit_type = "Tangerine";
	}
};

class Pear : public Fruit
{
public:
	Pear()
	{
		fruit_type = "Pear";
	}
};

