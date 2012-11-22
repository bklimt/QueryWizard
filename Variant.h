
#ifndef VARIANT_H
#define VARIANT_H

#include <afx.h>
#include <string>
#include "Date.h"
using namespace std;

class CVariant {
	public:
		CVariant();
		CVariant( const CVariant& val );
		CVariant( int val );
		CVariant( double val );
		CVariant( string val );
		CVariant( bool val );
		CVariant( const CDate& val );

		CVariant operator=( const CVariant& val );
		bool operator==( const CVariant& val );
		bool operator!=( const CVariant& val );

		string GetType();
		void SetNull();

		operator int ();
		operator string ();
		operator double ();
		operator bool ();
		operator CDate ();
	private:
		string type;
		int intval;
		double doubleval;
		bool boolval;
		CDate dateval;
		string stringval;
};

#endif