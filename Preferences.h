
#pragma warning(disable:4786)
#include <afxwin.h>
#include <string>
#include <fstream>
#include <map>
using namespace std;

class CPreferences {
	public:
		CPreferences();
		~CPreferences();
		void LoadPreferences();
		void SavePreferences();
		void UseDefaults();

		string GetHomePath();

		string GetString( string name );
		CRect GetRect( string name );
		bool GetBool( string name );
		int GetInt( string name );

		void SetString( string name, string value );
		void SetRect( string name, CRect value );
		void SetBool( string name, bool value );
		void SetInt( string name, int value );

	private:
		string iniPath;
		map< string, string > stringMap;
		map< string, CRect > rectMap;
		map< string, bool > boolMap;
		map< string, int > intMap;
};

#ifdef DECLARE_PREFERENCES
	CPreferences preferences;
#else
	extern CPreferences preferences;
#endif