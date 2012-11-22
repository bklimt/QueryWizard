
#include "QueryBuilder.h"
#include "Connection.h"
#include <afxwin.h>

CDBSchema* schemaPtr;

CQueryBuilder::CQueryBuilder() {
	schemaPtr = &schema;
	if ( !schema.LoadFromFile( "studentinfo.xml" ) ) {
		MessageBox( 0, "The configuration file (studentinfo.xml) is missing or corrupted!", "", 0 );
		exit( -1 );
	}
	SetWhereAll( vector< vector< string > >( 10, vector< string >( 3, (string)"" ) ) );
	SetWhereAny( vector< vector< string > >( 10, vector< string >( 3, (string)"" ) ) );
}

vector<string> CQueryBuilder::GetFieldList() {
	return schema.GetFieldList();
}

bool CQueryBuilder::HasList( string fieldID ) {
	return schema.HasList( fieldID );
}

vector<string> CQueryBuilder::GetList( string fieldID ) {
	CConnection conn;
	if ( !conn.OpenForReading() ) {
		return vector<string>();
	}

	try {
		vector<string> fieldname = vector<string>( 1, fieldID );
		vector<string> field2list;
		//vector<string> tables;
		//vector<string> clauses;
		vector<string> ans;
		//schema.GetTables( fieldname, field2list, tables, clauses );
		//int i;
		string where_sql;
		string from_sql = ") FROM " + schema.GetTables( fieldname, field2list, where_sql );
		string select_sql = "SELECT DISTINCT(" + fieldname[0];

		if ( where_sql.length() ) {
			where_sql = " WHERE " + where_sql;
		}

		CTable rs = conn.Execute( select_sql+from_sql+where_sql );
		while ( !rs.IsAtEOF() ) {
			ans.push_back( rs[0] );
			rs.MoveNext();
		}
		return ans;
	}
	catch ( ... ) {
	}

	return vector<string>();
	conn.Close();
}

void CQueryBuilder::SetSelect( vector<string> newFields ) {
	fields = newFields;
	for ( int i=0; i<order.size(); i++ ) {
		bool found = false;
		for ( int j=0; j<fields.size(); j++ ) {
			if ( order[i].substr( 0, order[i].length()-15 ) == fields[j] ) {
				found = true;
			}
		}
		if ( !found ) {
			for ( int k=i+1; k<order.size(); k++ ) {
				order[k-1] = order[k];
			}
			order.pop_back();
			i--;
		}
	}
}

vector< string > CQueryBuilder::GetSelect() {
	return fields;
}

void CQueryBuilder::SetWhereAll( vector< vector< string > > newAllClauses ) {
	bool isValid = true;
	if ( newAllClauses.size() < 10 ) {
		isValid = false;
	} else {
		for ( int i=0; i<10; i++ ) {
			if ( newAllClauses.size() < 3 ) {
				isValid = false;
			}
		}
	}
	if ( isValid ) {
		allClauses = newAllClauses;
	}
}

vector< vector< string > > CQueryBuilder::GetWhereAll() {
	return allClauses;
}

void CQueryBuilder::SetWhereAny( vector< vector< string > > newAnyClauses ) {
	bool isValid = true;
	if ( newAnyClauses.size() < 10 ) {
		isValid = false;
	} else {
		for ( int i=0; i<10; i++ ) {
			if ( newAnyClauses.size() < 3 ) {
				isValid = false;
			}
		}
	}
	if ( isValid ) {
		anyClauses = newAnyClauses;
	}
}

vector< vector< string > > CQueryBuilder::GetWhereAny() {
	return anyClauses;
}

void CQueryBuilder::SetOrder( vector< string > newFields ) {
	order = newFields;
}

vector< string > CQueryBuilder::GetOrder() {
	return order;
}

string CQueryBuilder::GetQuery() {
	int i;
	vector<string> fieldlist = fields;
	vector<string> field2list;
	vector<string> clauses;
	vector<string> tables;

	for ( i=0; i<10; i++ ) {
		if ( allClauses[i][0] != "" ) {
			field2list.push_back( allClauses[i][0] );
		}
		if ( anyClauses[i][0] != "" ) {
			field2list.push_back( anyClauses[i][0] );
		}
	}

	string where_sql = "";
	string from_sql = (string)"FROM " + schema.GetTables( fieldlist, field2list, where_sql );//, tables, clauses );

	string select_sql = "SELECT ";
	//string from_sql = " FROM ";
	for ( i=0; i<fieldlist.size(); i++ ) {
		select_sql += fieldlist[i];
		if ( i != fieldlist.size()-1 ) {
			select_sql += ", ";
		} else {
			select_sql += " ";
		}
	}
	//for ( i=0; i<tables.size(); i++ ) {
	//	from_sql += tables[i];
	//	if ( i != tables.size()-1 ) {
	//		from_sql += ", ";
	//	}
	//}
	//for ( i=0; i<clauses.size(); i++ ) {
	//	where_sql = where_sql + "( " + clauses[i] + " )";
	//	if ( i != clauses.size()-1 ) {
	//		where_sql += " AND ";
	//	}
	//}

	string where_all_sql = "";
	string where_any_sql = "";
	for ( i=0; i<10; i++ ) {
		bool allIsEmpty = true;
		bool anyIsEmpty = true;
		for ( int j=0; j<3; j++ ) {
			if ( allClauses[i][j] != "" ) { allIsEmpty = false; }
			if ( anyClauses[i][j] != "" ) { anyIsEmpty = false; }
		}
		if ( !allIsEmpty ) {
			if ( where_all_sql == "" ) {
				where_all_sql = (string)"( " + schema.GetActualField(allClauses[i][0]) +  allClauses[i][1] + "'" + allClauses[i][2] + "' )";
			} else {
				where_all_sql += (string)" AND ( " + schema.GetActualField(allClauses[i][0]) +  allClauses[i][1] + "'" + allClauses[i][2] + "' )";
			}
		}
		if ( !anyIsEmpty ) {
			if ( where_any_sql == "" ) {
				where_any_sql = (string)"( " + schema.GetActualField(anyClauses[i][0]) +  anyClauses[i][1] + "'" + anyClauses[i][2] + "' )";
			} else {
				where_any_sql += (string)" OR ( " + schema.GetActualField(anyClauses[i][0]) +  anyClauses[i][1] + "'" + anyClauses[i][2] + "' )";
			}
		}
	}

	if ( where_sql != "" ) {
		if ( where_all_sql != "" ) {
			if ( where_any_sql != "" ) {
				where_sql = (string)" WHERE ( " + where_sql + " ) AND ( " + where_any_sql + " ) AND ( " + where_all_sql + " ) ";
			} else {
				where_sql = (string)" WHERE ( " + where_sql + " ) AND ( " + where_all_sql + " ) ";
			}
		} else {
			if ( where_any_sql != "" ) {
				where_sql = (string)" WHERE ( " + where_sql + " ) AND ( " + where_any_sql + " ) ";
			} else {
				where_sql = (string)" WHERE ( " + where_sql + " ) ";
			}
		}
	} else {
		if ( where_all_sql != "" ) {
			if ( where_any_sql != "" ) {
				where_sql = (string)" WHERE ( " + where_all_sql + " ) AND ( " + where_any_sql + " ) ";
			} else {
				where_sql = (string)" WHERE ( " + where_all_sql + " ) ";
			}
		} else {
			if ( where_any_sql != "" ) {
				where_sql = (string)" WHERE ( " + where_any_sql + " ) ";
			} else {
				where_sql = "";
			}
		}
	}

	string order_sql = "";
	for ( i=0; i<order.size(); i++ ) {
		string field = schema.GetActualField( order[i].substr( 0, order[i].length()-15 ) );
		string direction = order[i].substr( order[i].length()-15, 15 );
		if ( direction == " ( Descending )" ) {
			field += " DESC";
		}
		if ( i ) {
			order_sql += ", ";
			order_sql += field;
		} else {
			order_sql = field;
		}
	}
	if ( order_sql.length() ) {
		order_sql = (string)" ORDER BY " + order_sql;
	}

	return select_sql+from_sql+where_sql+order_sql;
}
