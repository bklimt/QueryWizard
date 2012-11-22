
#include "QueryWizardDialog.h"

class CWhereDialog : public CQueryWizardDialog {
	public:	
		CWhereDialog( bool isAllDialog=false );
		void Initialize( CQueryBuilder* builder );
		void SaveData( CQueryBuilder* builder );

		void OnFieldChosen1();
		void OnFieldChosen2();
		void OnFieldChosen3();
		void OnFieldChosen4();
		void OnFieldChosen5();
		void OnFieldChosen6();
		void OnFieldChosen7();
		void OnFieldChosen8();
		void OnFieldChosen9();
		void OnFieldChosen10();
	private:
		bool isAll;
		DECLARE_MESSAGE_MAP()
};
