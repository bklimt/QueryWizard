
#ifndef DATE_H
#define DATE_H

//#include <afx.h>
#include <string>
using namespace std;

class CDate {
	public:
		CDate();
		CDate( int newMonth, int newDay, int newYear );
		CDate( const CDate& otherDate );
		CDate operator=( const CDate& otherDate );
		int operator-( const CDate& otherDate ) const;
		CDate operator+( const int days );
		bool operator==( const CDate& otherDate );
		operator string();
		bool IsLeapYear( int year );
		int GetDaysInMonth( int month, int year );
		static CDate Today();
	private:
		bool isNull;
		int month, day, year;
};

#endif