#ifndef __DOCTAB_H__
#define __DOCTAB_H__

#include "main.h"

enum { DOC_TAB = 0, ADD_TAB };

class DocTabUserArea : public GeUserArea
{
	BaseDocument* doc;	
	Int32 index;
	Int32 mode;
	INSTANCEOF(DocTabUserArea, GeUserArea)
public:	
	DocTabUserArea(BaseDocument* doc_, Int32 index_, Int32 mode_) : doc(doc_), index(index_), mode(mode_){}
	~DocTabUserArea(){}
	virtual Bool InputEvent(const BaseContainer& msg);
	virtual void DrawMsg(Int32 x1, Int32 y1, Int32 x2, Int32 y2, const BaseContainer& msg);
};

class DocTabDialog : public GeDialog
{
	DocTabUserArea* addDocTab;
	maxon::PointerArray<DocTabUserArea*> doc_tab_dialog_arr;
	INSTANCEOF(DocTabDialog, GeDialog)
public:
	static Int32 active;
	DocTabDialog():addDocTab(nullptr){
	}
	~DocTabDialog();
	virtual Bool CreateLayout();
	Bool RefreshTab();
	//virtual Bool InitValues();
};

class DocTab : public CommandData
{
	DocTabDialog doc_tab_dialog;
	INSTANCEOF(DocTab, CommandData)
public:
	virtual Bool Execute(BaseDocument* doc, GeDialog* parentManager) {
		if (doc_tab_dialog.IsOpen() == false)
			doc_tab_dialog.Open(DLG_TYPE::ASYNC, ID_DOC_TAB,-1,-1,0,30);
		else
			doc_tab_dialog.Close();
		return true;
	}
};
#endif __DOCTAB_H__
