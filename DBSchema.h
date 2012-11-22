
#pragma warning(disable:4786)
#include <set>
#include <string>
#include <vector>
#include <deque>
using namespace std;

#include "XMLObject.h"

struct CDBTable {
	string name;
	string source;
	set<int> dependencies;
	set<string> joinclauses;
	set<string> whereclauses;
};

struct CDBField {
	string id;
	string field;
	bool hasList;
	int table;
};

struct CDBCategory {
	string name;
	set<int> fields;
	set<int> categories;
};

class CDBSchema {
	public:
		bool LoadFromFile( string filename );
		vector< string > GetFieldList();
		bool HasList( string fieldID );
		void DoTable( int t, string& current, string& where, vector<int>& usedtables );
		string GetTables( vector<string>& fieldlist, vector<string>& field2list, string& whereClause );
		//void GetTables( vector<string>& fieldlist, vector<string>& field2list, vector<string>& tablelist, vector<string>& clauselist );
		string GetActualField( string fieldID );
	private:
		int CreateTable( CXMLObject* obj );
		int CreateField( CXMLObject* obj );
		int CreateCategory( CXMLObject* obj );

		int FindTable( string name );
		int FindField( string id );
		int FindCategory( string name );

		vector< CDBTable > tables;
		vector< CDBField > fields;
		vector< CDBCategory > categories;
};