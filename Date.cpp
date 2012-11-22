
#include "Date.h"
#include <time.h>

CDate::CDate() {
	isNull = true;
}

CDate::CDate( int newMonth, int newDay, int newYear ) {
	isNull = false;
	month = newMonth;
	day = newDay;
	year = newYear;
}

CDate::CDate( const CDate& otherDate ) {
	isNull = otherDate.isNull;
	month = otherDate.month;
	day = otherDate.day;
	year = otherDate.year;
}

CDate CDate::operator=( const CDate& otherDate ) {
	isNull = otherDate.isNull;
	month = otherDate.month;
	day = otherDate.day;
	year = otherDate.year;
	return *this;
}

int CDate::operator-( const CDate& otherDate ) const {
	if ( isNull || otherDate.isNull ) {
		return -1;
	}
	if ( otherDate.year > year ) {
		return -1;
	} else {
		if ( otherDate.year < year ) {
			if ( otherDate.month < month ) {
				// this year counts
				return ( year - otherDate.year ) + 1;
			} else {
				if ( otherDate.month == month ) {
					// this year may count
					if ( otherDate.day < day ) {
						// this year counts
						return ( year - otherDate.year ) + 1;
					} else {
						// this year doesn't count
						return year - otherDate.year;
					}
				} else {
					// this year doesn't count
					return year - otherDate.year;
				}
			}
		} else {
			if ( otherDate.year == year ) {
				if ( otherDate.month > month ) {
					return -1;
				} else {
					if ( otherDate.month < month ) {
						return 0;
					} else {
						if ( otherDate.day > day ) {
							return -1;
						} else {
							if ( otherDate.day < day ) {
								return 0;
							} else {
								return 0;
							}
						}
					}
				}
			} else {
				return -1;
			}
		}
	}
}

CDate CDate::operator+( const int days ) {
	CDate temp = (*this);
	temp.day += days;
	while ( temp.day > GetDaysInMonth( temp.month, temp.year ) ) {
		temp.day -= GetDaysInMonth( temp.month, temp.year );
		if ( temp.month == 12 ) {
			temp.month = 1;
			temp.year++;
		} else {
			temp.month++;
		}
	}
	return temp;
}

bool CDate::operator==( const CDate& otherDate ) {
	if ( isNull ) {
		if ( otherDate.isNull ) {
			return true;
		} else {
			return false;
		}
	} else {
		if ( otherDate.isNull ) {
			return false;
		} else {
			return ( month==otherDate.month && day==otherDate.day && year==otherDate.year );
		}
	}
}

CDate::operator string() {
	if ( isNull ) {
		return "";
	} else {
		char temp[100];
		sprintf( temp, "%.2d-%.2d-%.4d", month, day, year );
		return temp;
	}
}

bool CDate::IsLeapYear( int year ) {
	if ( year % 4 == 0 ) {
		if ( year % 100 != 0 ) {
			if ( year % 400 == 0 ) {
				return true;
			}
		}
	}
	return false;
}

int CDate::GetDaysInMonth( int month, int year ) {
	switch ( month ) {
		case  1 :
		case  3 :
		case  5 :
		case  7 :
		case  8 :
		case 10 :
		case 12 : return 31;
		case  4 :
		case  6 :
		case  9 :
		case 11 : return 30;
		case  2 : return ( IsLeapYear( year ) ? 29 : 28 );
	}
	return 0;
}

CDate CDate::Today() {
	time_t t;
	time( &t );
	tm* tmp = localtime( &t );
	return CDate( tmp->tm_mon+1, tmp->tm_mday+1, 1900+tmp->tm_year );
}
