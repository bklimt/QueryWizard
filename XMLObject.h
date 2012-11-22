
#pragma warning( disable:4786 )
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct XMLTag {
	string name;
	vector< pair< string, string > > parameters;
	vector< string > options;
	bool empty;
	bool end;
};

class CXMLObject {
	public:
		bool LoadFromFile( string filename );
		string GetType();
		string GetParameter( string name );
		bool GetOption( string name );
		int GetChildCount();
		CXMLObject* GetChild( int i );
	private:
		bool CreateObject( vector< XMLTag >& tags, int& index );
		void SetError( string newError );
		string error;
		string type;
		map< string, string > parameters;
		set< string > options;
		vector< CXMLObject > children;
};

