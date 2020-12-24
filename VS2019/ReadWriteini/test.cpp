#include <iostream>
#include "ReadWriteini.hpp"
using namespace std;
int main() {
	rwini::ReadWriteini* rw = new rwini::ReadWriteini("C:\\Users\\windSnowLi\\pip\\pip.ini");
	string index_url = rw->FindValue("global", "index-url");
	string install = rw->FindValue("install", "trusted-host");
	cout << index_url << endl;
	cout << install << endl;
	return 0;
}