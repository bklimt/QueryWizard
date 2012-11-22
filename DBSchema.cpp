
#include "DBSchema.h"
#include <algorithm>
#include "Preferences.h"
using namespace std;

bool CDBSchema::LoadFromFile( string filename ) {
	tables.clear();
	categories.clear();
	fields.clear();

	CXMLObject obj;
	if ( !obj.LoadFromFile( filename ) ) {
		return false;
	}

	for ( int i=0; i<obj.GetChildCount(); i++ ) {
		if ( obj.GetChild(i)->GetType() == "TABLE" ) {
			if ( CreateTable( obj.GetChild(i) ) < 0 ) {
				return false;
			}
		} else {
			if ( obj.GetChild(i)->GetType() == "CATEGORY" ) {
				if ( CreateCategory( obj.GetChild(i) ) < 0 ) {
					return false;
				}
			}
		}
	}
	return true;
}

vector< string > CDBSchema::GetFieldList() {
	vector<string> ans;
	for ( int i=0; i<fields.size(); i++ ) {
		ans.push_back( fields[i].id );
	}
	return ans;
}

bool CDBSchema::HasList( string fieldID ) {
	int index = FindField( fieldID );
	if ( index >= 0 ) {
		if ( fields[index].hasList ) {
			return true;
		}
	}
	return false;
}

void CDBSchema::DoTable( int t, string& current, string& where, vector<int>& usedtables ) {
	if ( find( usedtables.begin(), usedtables.end(), t ) == usedtables.end() ) {
		for ( set<int>::iterator i=tables[t].dependencies.begin(); i!=tables[t].dependencies.end(); i++ ) {
			DoTable( *i, current, where, usedtables );
		}
		if ( find( usedtables.begin(), usedtables.end(), t ) == usedtables.end() ) {
			usedtables.push_back( t );
			if ( current == "" ) {
				current = tables[t].source + " " + tables[t].name;
			} else {
				if ( current.find("RIGHT JOIN") == -1 ) {
					current = tables[t].source + " " + tables[t].name + " RIGHT JOIN " + current + " ";
				} else {
					current = tables[t].source + " " + tables[t].name + " RIGHT JOIN ( " + current + " ) ";
				}
				set<string>::iterator j;
				if ( tables[t].joinclauses.size() ) {
					current += " ON ";
					for ( j=tables[t].joinclauses.begin(); j!=tables[t].joinclauses.end(); j++ ) {
						current += (string)(*j);
						current += (string)", ";
					}
					current = current.substr( 0, current.length() - 2 );
				}
				if ( tables[t].whereclauses.size() ) {
					if ( where.length() ) {
						where += " AND ( ";
					} else {
						where = " ( ";
					}
					for ( j=tables[t].whereclauses.begin(); j!=tables[t].whereclauses.end(); j++ ) {
						where += (string)(*j);
						where += (string)", ";
					}
					where = where.substr( 0, where.length() - 2 );
					where += " ) ";
				}
			}
		}
	}
}

string CDBSchema::GetTables( vector<string>& fieldlist, vector<string>& field2list, string& whereClause ) {
	int i;
	string current;
	vector<int> usedtables;

	for ( i=0; i<fieldlist.size(); i++ ) {
		int fieldID = FindField(fieldlist[i]);
		if ( fieldID >= 0 ) {
			fieldlist[i] = tables[fields[fieldID].table].name + "." + fields[fieldID].field;
			DoTable( fields[fieldID].table, current, whereClause, usedtables );
		}
	}

	for ( i=0; i<field2list.size(); i++ ) {
		int fieldID = FindField(field2list[i]);
		if ( fieldID >= 0 ) {
			field2list[i] = tables[fields[fieldID].table].name + "." + fields[fieldID].field;
			DoTable( fields[fieldID].table, current, whereClause, usedtables );
		}
	}

	return current;
}

/*void CDBSchema::GetTables( vector<string>& fieldlist, vector<string>& field2list, vector<string>& tablelist, vector<string>& clauselist ) {
	int i;
	set<int> tableset;
	deque<int> unsearched;
	for ( i=0; i<fieldlist.size(); i++ ) {
		int fieldid = FindField( fieldlist[i] );
		if ( fieldid >= 0 ) {
			fieldlist[i] = tables[fields[fieldid].table].name + "." + fields[fieldid].field;
			int tableid = fields[fieldid].table;
			if ( tableset.find( tableid ) == tableset.end() ) {
				tableset.insert( tableid );
				unsearched.push_back( tableid );
			}
		} else {
			fieldlist[i] = "";
		}
	}
	for ( i=0; i<field2list.size(); i++ ) {
		int fieldid = FindField( field2list[i] );
		if ( fieldid >= 0 ) {
			field2list[i] = tables[fields[fieldid].table].name + "." + fields[fieldid].field;
			int tableid = fields[fieldid].table;
			if ( tableset.find( tableid ) == tableset.end() ) {
				tableset.insert( tableid );
				unsearched.push_back( tableid );
			}
		} else {
			field2list[i] = "";
		}
	}
	while ( unsearched.size() ) {
		int current = unsearched.front();
		unsearched.pop_front();
		for ( set<int>::iterator it=tables[current].dependencies.begin(); it!=tables[current].dependencies.end(); it++ ) {
			if ( tableset.find( *it ) == tableset.end() ) {
				tableset.insert( *it );
				unsearched.push_back( *it );
			}
		}
	}
	for ( set<int>::iterator it=tableset.begin(); it!=tableset.end(); it++ ) {
		tablelist.push_back( tables[ *it ].source + " " + tables[ *it ].name );
		for ( set<string>::iterator it2=tables[ *it ].clauses.begin(); it2!=tables[ *it ].clauses.end(); it2++ ) {
			clauselist.push_back( *it2 );
		}
	}
}
*/

string CDBSchema::GetActualField( string fieldID ) {
	int field = FindField( fieldID );
	if ( field < 0 ) {
		return "";
	} else {
		return tables[fields[field].table].name + "." + fields[field].field;
	}
}

int CDBSchema::CreateTable( CXMLObject* obj ) {
	CDBTable table;
	table.name = obj->GetParameter( "NAME" );
	if ( preferences.GetBool( "UsePrefix" ) ) {
		table.source = (string)"StudentInfoSuperUser." + obj->GetParameter( "SOURCE" );
	} else {
		table.source = obj->GetParameter( "SOURCE" );
	}
	for ( int i=0; i<obj->GetChildCount(); i++ ) {
		int dependency = FindTable( obj->GetChild(i)->GetParameter( "TABLE" ) );
		if ( dependency < 0 ) {
			return -1;
		}
		table.dependencies.insert( dependency );
		string clause = obj->GetChild(i)->GetParameter( "CLAUSE" );
		string whereclause = obj->GetChild(i)->GetParameter( "WHERE-CLAUSE" );
		if ( clause != "" ) {
			table.joinclauses.insert( clause );
		}
		if ( whereclause != "" ) {
			table.whereclauses.insert( whereclause );
		}
	}
	tables.push_back( table );
	return tables.size()-1;
}

int CDBSchema::CreateField( CXMLObject* obj ) {
	CDBField field;
	field.id = obj->GetParameter( "ID" );
	field.field = obj->GetParameter( "FIELD" );
	field.hasList = false;
	if ( obj->GetParameter( "HASLIST" ) == "true" ) {
		field.hasList = true;
	}
	int table = FindTable( obj->GetParameter( "TABLE" ) );
	if ( table == -1 ) {
		return -1;
	}
	field.table = table;
	if ( field.field == "" ) {
		for ( int i=0; i<field.id.length(); i++ ) {
			if ( isalpha( field.id[i] ) ) {
				field.field += field.id[i];
			}
		}
	}
	fields.push_back( field );
	return fields.size()-1;
}

int CDBSchema::CreateCategory( CXMLObject* obj ) {
	CDBCategory category;
	category.name = obj->GetParameter( "NAME" );
	for ( int i=0; i<obj->GetChildCount(); i++ ) {
		if ( obj->GetChild( i )->GetType() == "FIELD" ) {
			int field = CreateField( obj->GetChild( i ) );
			if ( field < 0 ) {
				return -1;
			} else {
				category.fields.insert( field );
			}
		} else {
			if ( obj->GetChild( i )->GetType() == "CATEGORY" ) {
				int cat = CreateCategory( obj->GetChild( i ) );
				if ( cat < 0 ) {
					return -1;
				} else {
					category.categories.insert( cat );
				}
			}
		}
	}
	categories.push_back( category );
	return categories.size()-1;
}

int CDBSchema::FindTable( string name ) {
	for ( int i=0; i<tables.size(); i++ ) {
		if ( tables[i].name == name ) {
			return i;
		}
	}
	return -1;
}

int CDBSchema::FindField( string id ) {
	for ( int i=0; i<fields.size(); i++ ) {
		if ( fields[i].id == id ) {
			return i;
		}
	}
	return -1;
}

int CDBSchema::FindCategory( string name ) {
	for ( int i=0; i<categories.size(); i++ ) {
		if ( categories[i].name == name ) {
			return i;
		}
	}
	return -1;
}

