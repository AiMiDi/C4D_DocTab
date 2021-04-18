#include "doctab.h"


DocTabDialog::~DocTabDialog() {
	for (DocTabUserArea* i : doc_tab_dialog_arr) {
		if (i) {
			delete i;
		}
	}
	if (addDocTab) {
		delete addDocTab;
	}
}
void DocTabUserArea::DrawMsg(Int32 x1, Int32 y1, Int32 x2, Int32 y2, const BaseContainer& msg) {
	OffScreenOn();	
	if (mode == DOC_TAB) {
		if (doc != nullptr) {
			if (doc==GetActiveDocument()) {
				DrawBitmap(AutoBitmap("active.png"_s), x1, y1, x2, y2, 0, 0, 456, 185, BMP_NORMALSCALED | BMP_ALLOWALPHA);
				SetClippingRegion(x2 * 0.12, y1, x2 * 0.65, y2);
				DrawSetTextCol(Vector(223, 131, 60) / 256, COLOR_TRANS);
				DrawText(doc->GetDocumentName().GetFileString(), x2 * 0.12, y2 * 0.65 - 8);
				ClearClippingRegion();
				DrawBitmap(AutoBitmap("close.png"_s), x2 * 0.75, y2 * 0.6 - 8, 13, 13, 0, 0, 160, 160, BMP_NORMALSCALED | BMP_ALLOWALPHA);
			}
			else {
				DrawBitmap(AutoBitmap("inactive.png"_s), x1, y1, x2, y2, 0, 0, 456, 185, BMP_NORMALSCALED | BMP_ALLOWALPHA);
				SetClippingRegion(x2 * 0.12, y1, x2 * 0.65, y2);
				DrawSetTextCol(Vector(165, 165, 165) / 256, COLOR_TRANS);
				DrawText(doc->GetDocumentName().GetFileString(), x2 * 0.12, y2 * 0.65 - 8);
				ClearClippingRegion();
				DrawBitmap(AutoBitmap("close_in.png"_s), x2 * 0.75, y2 * 0.6 - 8, 13, 13, 0, 0, 160, 160, BMP_NORMALSCALED | BMP_ALLOWALPHA);
			}
		}
	}
	else {
		DrawBitmap(AutoBitmap("inactive.png"_s), x1, y1, x2, y2, 0, 0, 456, 185, BMP_NORMALSCALED | BMP_ALLOWALPHA);
		DrawSetTextCol(Vector(165, 165, 165) / 256, COLOR_TRANS);
		DrawText("+"_s, x2 * 0.3, y2 * 0.65 - 8);	
	}
}
Bool DocTabUserArea::InputEvent(const BaseContainer& msg) {
	if (msg.GetInt32(BFM_INPUT_DEVICE) == BFM_INPUT_MOUSE && msg.GetInt32(BFM_INPUT_CHANNEL) == BFM_INPUT_MOUSELEFT)
	{
		if (mode == DOC_TAB) {
			Int32 ClickX = msg.GetInt32(BFM_INPUT_X);
			Int32 ClickY = msg.GetInt32(BFM_INPUT_Y);
			Global2Local(&ClickX, &ClickY);
			SetActiveDocument(doc);
			if (ClickX >= (Int32)(this->GetWidth() * 0.75) && ClickX <= (Int32)(this->GetWidth() * 0.75 + 16) && ClickY >= (Int32)(this->GetHeight() * 0.6 - 8) && ClickY <= (Int32)(this->GetHeight() * 0.6 + 8)) {
				//GePrint("x"_s);
				if (doc != nullptr) {
					if (IsCommandEnabled(12664)) {
						CallCommand(12664);
					}
					if (!((DocTabDialog*)dlg)->RefreshTab())return false;
				}
			}
		}
		else {
			if (IsCommandEnabled(12094)) {
				CallCommand(12094);
			}
			if (!((DocTabDialog*)dlg)->RefreshTab())return false;
		}
	}
	return true;
}

Bool DocTabDialog::CreateLayout() {
	this->SetTitle("DocTab"_s);
	BaseDocument* doc = GetFirstDocument();
	GroupBeginInMenuLine();
	GroupBegin(1000, BFH_LEFT , 0, 1, ""_s, 0, 0, 0);
	GroupSpace(0, 0);
	while (doc != nullptr)
	{
		Int32 Index = doc_tab_dialog_arr.GetCount();
		DocTabUserArea* doc_tab_user_area = new DocTabUserArea(doc, DOC_TAB);
		C4DGadget* const userAreaGadget = this->AddUserArea(10000 + Index, BFH_LEFT , 160, 14);
		if (userAreaGadget != nullptr)
			this->AttachUserArea((*doc_tab_user_area), userAreaGadget);
		iferr(doc_tab_dialog_arr.Append(doc_tab_user_area))return false;
		doc = doc->GetNext();
	}
	addDocTab = new DocTabUserArea(doc, ADD_TAB);
	C4DGadget* const userAreaGadget = this->AddUserArea(2000, BFH_LEFT, 30, 14);
	if (userAreaGadget != nullptr)
		this->AttachUserArea((*addDocTab), userAreaGadget);
	GroupEnd();
	GroupEnd();
	return true;
}
Bool DocTabDialog::CoreMessage(Int32 id, const BaseContainer& msg) {
	if (id == EVMSG_DOCUMENTRECALCULATED) {
		if (!this->RefreshTab())return false;
	}
	return SUPER::CoreMessage(id, msg);
}

Bool DocTabDialog::RefreshTab() {
	for (DocTabUserArea* i : doc_tab_dialog_arr) {
		if (i) {
			delete i;
		}
	}
	doc_tab_dialog_arr.Reset();
	BaseDocument* doc = GetFirstDocument();
	LayoutFlushGroup(1000);
	while (doc != nullptr)
	{
		Int32 Index = doc_tab_dialog_arr.GetCount();
		DocTabUserArea* doc_tab_user_area = new DocTabUserArea(doc, DOC_TAB);
		C4DGadget* const userAreaGadget = this->AddUserArea(10000 + Index, BFH_LEFT , 160, 14);
		if (userAreaGadget != nullptr)
			this->AttachUserArea((*doc_tab_user_area), userAreaGadget);
		iferr(doc_tab_dialog_arr.Append(doc_tab_user_area))return false;
		doc = doc->GetNext();
	}
	addDocTab = new DocTabUserArea(doc, ADD_TAB);
	C4DGadget* const userAreaGadget = this->AddUserArea(2000, BFH_LEFT , 30, 14);
	if (userAreaGadget != nullptr)
		this->AttachUserArea((*addDocTab), userAreaGadget);
	LayoutChanged(1000);
	return true;
}