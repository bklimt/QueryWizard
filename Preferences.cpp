
#define DECLARE_PREFERENCES

#include "Preferences.h"

CPreferences::CPreferences() {
	char temp[10000];
	::GetCurrentDirectory( 9999, temp );
	iniPath = temp;
	if ( iniPath[iniPath.size()-1] != '\\' ) {
		iniPath += '\\';
	}
	LoadPreferences();
}

CPreferences::~CPreferences() {
	SavePreferences();
}

void CPreferences::LoadPreferences() {
	UseDefaults();
	try {
		ifstream in( ( iniPath + "querywizard.ini" ).c_str() );
		string name = "";
		string type = "";
		string value = "";
		while ( type != "end" ) {
			if ( !in.good() ) {
				UseDefaults();
				return;
			}
			getline( in, name );
			getline( in, type );
			if ( type == "string" ) {
				getline( in, value );
				for ( int i=0; i<value.size()-1; i++ ) {
					if ( value[i] == '\\' && value[i+1] == 'n' ) {
						value = value.substr( 0,i ) + "\n" + value.substr( i+2, value.length()-(i+2) );
					}
				}
				stringMap[name] = value;
			} else {
				if ( type == "rect" ) {
					CRect temp;
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.bottom );
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.left );
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.right );
					getline( in, value );
					sscanf( value.c_str(), "%d", &temp.top );
					rectMap[name] = temp;
				} else {
					if ( type == "bool" ) {
						getline( in, value );
						if ( value == "true" ) {
							boolMap[name] = true;
						} else {
							boolMap[name] = false;
						}
					} else {
						if ( type == "int" ) {
							int temp;
							getline( in, value );
							sscanf( value.c_str(), "%d", &temp );
							intMap[name] = temp;
						}
					}
				}
			}
			if ( type != "end" ) {
				getline( in, value );
			}
		}
		in.close();
	}
	catch ( ... ) {
		UseDefaults();
	}
}

void CPreferences::SavePreferences() {
	try {
		FILE* f = fopen(  ( iniPath + "querywizard.ini" ).c_str(), "w" );
		map<string,string>::iterator strIt;
		map<string,CRect>::iterator rectIt;
		map<string,bool>::iterator boolIt;
		map<string,int>::iterator intIt;

		for ( strIt = stringMap.begin(); strIt != stringMap.end(); strIt++ ) {
			fprintf( f, "%s\n", (*strIt).first.c_str() );
			fprintf( f, "string\n" );
			string value = (*strIt).second;
			while ( value.find( '\n' ) != -1 ) {
				int pos = value.find( '\n' );
				value = value.substr(0,pos) + "\\n" + value.substr(pos+1,value.size()-(pos+1));
			}
			fprintf( f, "%s\n\n", value.c_str() );
		}

		for ( rectIt = rectMap.begin(); rectIt != rectMap.end(); rectIt++ ) {
			fprintf( f, "%s\n", (*rectIt).first.c_str() );
			fprintf( f, "rect\n" );
			CRect value = (*rectIt).second;
			fprintf( f, "%d\n", value.bottom );
			fprintf( f, "%d\n", value.left );
			fprintf( f, "%d\n", value.right );
			fprintf( f, "%d\n\n", value.top );
		}

		for ( boolIt = boolMap.begin(); boolIt != boolMap.end(); boolIt++ ) {
			fprintf( f, "%s\n", (*boolIt).first.c_str() );
			fprintf( f, "bool\n" );
			if ( (*boolIt).second ) {
				fprintf( f, "true\n\n" );
			} else {
				fprintf( f, "false\n\n" );
			}
		}

		for ( intIt = intMap.begin(); intIt != intMap.end(); intIt++ ) {
			fprintf( f, "%s\n", (*intIt).first.c_str() );
			fprintf( f, "int\n" );
			fprintf( f, "%d\n\n", (*intIt).second );
		}

		fprintf( f, "end\nend\nend\n" );

		fclose(f);
	}
	catch ( ... ) {
	}
}

void CPreferences::UseDefaults() {
	stringMap["DSN"] = "Driver={SQL Server};Server=misd-sqlserver;Address=misd-sqlserver,1433;Network=DBMSSOCN;Database=StudentInfo2;";

	stringMap["ConnectionType"] = "SQL";
	stringMap["DSNDatabase"] = "StudentInfo2";
	stringMap["AccessDatabase"] = "studentinfo.mdb";
	stringMap["SQLServer"] = "misd-sqlserver";
	stringMap["SQLAddress"] = "misd-sqlserver,1433";
	stringMap["SQLDatabase"] = "StudentInfo2";

	boolMap["UsePrefix"] = true;
}

string CPreferences::GetHomePath() {
	return iniPath;
}

string CPreferences::GetString( string name ) {
	if ( stringMap.find( name ) == stringMap.end() ) {
		return "";
	}
	return stringMap[name];
}

CRect CPreferences::GetRect( string name ) {
	if ( rectMap.find( name ) == rectMap.end() ) {
		return CRect(0,0,0,0);
	}
	return rectMap[name];
}

bool CPreferences::GetBool( string name ) {
	if ( boolMap.find( name ) == boolMap.end() ) {
		return false;
	}
	return boolMap[name];
}

int CPreferences::GetInt( string name ) {
	if ( intMap.find( name ) == intMap.end() ) {
		return 0;
	}
	return intMap[name];
}

void CPreferences::SetString( string name, string value ) {
	stringMap[name] = value;
}

void CPreferences::SetRect( string name, CRect value ) {
	rectMap[name] = value;
}

void CPreferences::SetBool( string name, bool value ) {
	boolMap[name] = value;
}

void CPreferences::SetInt( string name, int value ) {
	intMap[name] = value;
}
