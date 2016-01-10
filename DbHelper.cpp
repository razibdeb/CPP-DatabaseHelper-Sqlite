#include "DbHelper.h"

static DbHelper * instance = NULL;

DbHelper::DbHelper() :database(NULL)
{
	std::cout << "Constructor Called " <<std::endl;
	
}


DbHelper::~DbHelper()
{
}

bool DbHelper::initializeDatabase()
{
	
	instance->createTable();
	return false;
}

DbHelper * DbHelper::getInstance()
{
	if (instance == nullptr)
	{
		instance = new DbHelper;
		instance->openDatabase();
	}
	return instance;
}

bool DbHelper::openDatabase()
{
	//sqlite3 *data;
	int result = sqlite3_open("Database.db", &database);
	if (result){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return true;
	}
	else{
		fprintf(stderr, "Opened database successfully\n");
		//database = data;
	}
	return false;
}

bool DbHelper::createTable()
{
	char * sqlQuery = "CREATE TABLE IF NOT EXISTS Vehicle("  \
		"ID INTEGER PRIMARY KEY  AUTOINCREMENT   NOT NULL  ," \
		"Vehicle_Registration	CHAR(50)    NOT NULL," \
		"Vehicle_Make	INT     NOT NULL," \
		"Vehicle_Model	CHAR(50) NOT NULL," \
		"Vehicle_Owner	TEXT NOT NULL );";
	char *zErrMsg = 0;
	/* Execute SQL statement */
	int result = sqlite3_exec(database, sqlQuery, NULL, 0, &zErrMsg);
	if (result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}
	else{
		fprintf(stdout, "Vehicle Table created successfully\n");
	}
	char * sqlQuery2 = "CREATE TABLE IF NOT EXISTS Log("  \
		"ID INTEGER PRIMARY KEY  AUTOINCREMENT   NOT NULL  ," \
		"Search_Text	TEXT NOT NULL );";
	char *zErrMsg2 = 0;
	/* Execute SQL statement */
	result = sqlite3_exec(database, sqlQuery2, NULL, 0, &zErrMsg2);
	if (result != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg2);
		return false;
	}
	else{
		fprintf(stdout, "Log Table created successfully\n");
	}
	return true;
}

void DbHelper::insertData()
{
	/* Create SQL statement */
	char * sql = "INSERT INTO Vehicle (Vehicle_Registration,Vehicle_Make,Vehicle_Model,Vehicle_Owner) "  \
		"VALUES ('REG-1', 2015, 'FORD-15', 'Paul'); " \
		"INSERT INTO Vehicle (Vehicle_Registration,Vehicle_Make,Vehicle_Model,Vehicle_Owner) "  \
		"VALUES ('REG-2', 2014, 'Lamborgini-15', 'Alen'); "     \
		"INSERT INTO Vehicle (Vehicle_Registration,Vehicle_Make,Vehicle_Model,Vehicle_Owner)" \
		"VALUES ('REG-3', 2015, 'Fx-15', 'Teddy' );" \
		"INSERT INTO Vehicle (Vehicle_Registration,Vehicle_Make,Vehicle_Model,Vehicle_Owner)" \
		"VALUES ('REG-4', 2010, 'M-11', 'Mark' );";

	/* Execute SQL statement */
	char * zErrMsg;
	int rc = sqlite3_exec(database, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Records created successfully\n");
	}
}

void DbHelper::insertSearchLog(string searchText)
{
	string sqlStr = "INSERT INTO Log (Search_Text) VALUES ('" + searchText + "')";
	const char * sql = searchText.c_str();
	/* Execute SQL statement */
	char * zErrMsg;
	int rc = sqlite3_exec(database, sql, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Records created successfully\n");
	}
}

vector<Vehicle> DbHelper::getVehicleList(string conditions)
{
	string selectQueryStr = "SELECT * FROM Vehicle ";
	if (!conditions.empty())
	{
		selectQueryStr += " WHERE ";
		selectQueryStr += conditions;
	}

	const char * selectQuery = selectQueryStr.c_str();
	std::wcout << "Query : " << selectQuery << std::endl;
	sqlite3_stmt *selectStmt;
	sqlite3_prepare_v2(database, selectQuery, strlen(selectQuery) + 1, &selectStmt, NULL);


	vector<Vehicle> vehicleList;
	while (1) {
		int s;
		printf("in select while\n");
		s = sqlite3_step(selectStmt);
		if (s == SQLITE_ROW) {
			int bytes;
			const unsigned char * reg;
			int id = sqlite3_column_int(selectStmt, 0);
			reg = sqlite3_column_text(selectStmt, 1);
			const unsigned int make = sqlite3_column_int(selectStmt, 2);
			const unsigned char * model = sqlite3_column_text(selectStmt, 3);
			const unsigned char * owner = sqlite3_column_text(selectStmt, 4);

			Vehicle v;
			v.id = id;
			v.Make = make;
			v.Registration = string(reinterpret_cast<const char*>(reg));
			v.Model = string(reinterpret_cast<const char*>(model));
			v.Owner = string(reinterpret_cast<const char*>(owner));

			vehicleList.push_back(v);
		}
		else if (s == SQLITE_DONE) {
			break;
		}
		else {
			fprintf(stderr, "Failed.\n");	
			break;
		}
	}
	sqlite3_finalize(selectStmt);

	return vehicleList;
}