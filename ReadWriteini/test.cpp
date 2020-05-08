#include "ReadWriteini.h"
#include <iostream>
using namespace std;
int main() {
	rwini::ReadWriteini* maintest = new rwini::ReadWriteini("E:\\1.ini");
	maintest->show();
	char bbb[100];
	maintest->FindValue("StudentInfo", "xxx", bbb);
	cout << bbb << endl;
	maintest->SetValue("StudentInfo", "Name", "mmmmmm");
	maintest->show();
	maintest->SetKey("StudentInfo", "Name", "name");
	maintest->show();
	maintest->InsertSection("testinsert");
	maintest->show();
	maintest->InsertKey("testinsert", "testinsertkey", "wote");
	maintest->show();
	maintest->DeleteSection("testinsert");
	maintest->show();
	maintest->DeleteKey("StudentInfo", "name");
	maintest->show();
	maintest->Writeini();
	return 0;
}