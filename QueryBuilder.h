
#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#pragma warning( disable:4786 )
#include <vector>
#include <string>
using namespace std;

#include "DBSchema.h"

class CQueryBuilder {
	public:
		CQueryBuilder();

		vector<string> GetFieldList();
		bool HasList( string fieldID );
		vector<string> GetList( string fieldID );

		void SetSelect( vector<string> newFields );
		vector< string > GetSelect();

		void SetWhereAll( vector< vector< string > > newAllClauses );
		vector< vector< string > > GetWhereAll();

		void SetWhereAny( vector< vector< string > > newAnyClauses );
		vector< vector< string > > GetWhereAny();

		void SetOrder( vector< string > newFields );
		vector< string > GetOrder();

		string GetQuery();

	private:
		CDBSchema schema;
		vector< string > fields;
		vector< string > order;
		vector< vector< string > > allClauses;
		vector< vector< string > > anyClauses;
};

#endif
