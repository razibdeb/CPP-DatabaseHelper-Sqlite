#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <string>
#include "Vehicle.h"
using namespace std;
class DbHelper
{
private:
	DbHelper();
	~DbHelper();
	sqlite3 *database;
	bool openDatabase();
	bool createTable();

public:
	static DbHelper * getInstance();
	bool initializeDatabase();
	void insertData();

	vector<Vehicle> getVehicleList(string conditions);
	void DbHelper::insertSearchLog(string searchText);
};

