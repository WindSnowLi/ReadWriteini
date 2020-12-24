#include <iostream>
#include "ReadWriteini.hpp"
using namespace std;
int main()
{
	try
	{
		rwini::ReadWriteini *rw = new rwini::ReadWriteini("../test/config.ini");
		string index_url = rw->FindValue("global", "index-url");
		string install = rw->FindValue("install", "trusted-host");
		cout << index_url << endl;
		cout << install << endl;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}

	return 0;
}