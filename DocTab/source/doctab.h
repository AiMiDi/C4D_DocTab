#ifndef __DOCTAB_H__
#define __DOCTAB_H__

#include "main.h"

enum { DOC_TAB = 0, ADD_TAB ,REC_DOC};

class DocTabUserArea : public GeUserArea
{
	BaseDocument* doc = nullptr;
	Int32 mode = 0;
	INSTANCEOF(DocTabUserArea, GeUserArea)
public:	
	DocTabUserArea(BaseDocument* doc_, Int32 mode_) : doc(doc_), mode(mode_){}
	~DocTabUserArea(){}
	virtual Bool InputEvent(const BaseContainer& msg);
	virtual void DrawMsg(Int32 x1, Int32 y1, Int32 x2, Int32 y2, const BaseContainer& msg);
};

class DocTabDialog : public GeDialog
{
	DocTabUserArea* addDocTab = nullptr;
	DocTabUserArea* recDocTab = nullptr;
	maxon::PointerArray<DocTabUserArea*> doc_tab_dialog_arr;
	INSTANCEOF(DocTabDialog, GeDialog)
public:
	DocTabDialog();
	~DocTabDialog();
	virtual Bool CoreMessage(Int32 id, const BaseContainer& msg);
	virtual Bool CreateLayout();
	Bool RefreshTab();
	//virtual Bool InitValues();
};

class DocTab : public CommandData
{
	DocTabDialog doc_tab_dialog;
	INSTANCEOF(DocTab, CommandData)
public:
	virtual Bool RestoreLayout(void* secret) {
		return doc_tab_dialog.RestoreLayout(ID_DOC_TAB, 0, secret);
	}

	virtual Bool Execute(BaseDocument* doc, GeDialog* parentManager) {
		if (doc_tab_dialog.IsOpen() == false)
			doc_tab_dialog.Open(DLG_TYPE::ASYNC, ID_DOC_TAB,-1,-1,0,30);
		else
			doc_tab_dialog.Close();
		return true;
	}
};
#endif __DOCTAB_H__
