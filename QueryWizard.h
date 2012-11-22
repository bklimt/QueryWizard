
#include <afxwin.h>
#include "QueryBuilder.h"

class CQueryWizard : public CWinApp {
	public:
		void ExecuteQuery( CQueryBuilder& builder );
		BOOL InitInstance();
	private:
};
