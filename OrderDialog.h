
#include "QueryWizardDialog.h"

class COrderDialog : public CQueryWizardDialog {
	public:	
		COrderDialog();
		void Initialize( CQueryBuilder* builder );
		void SaveData( CQueryBuilder* builder );

		void OnAddButtonClicked();
		void OnRemoveButtonClicked();
		void OnUpButtonClicked();
		void OnDownButtonClicked();

		void OnFieldListDblClk();
		void OnChosenListDblClk();
	private:
		DECLARE_MESSAGE_MAP()
};
