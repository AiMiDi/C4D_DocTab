#include "doctab.h"
#include "c4d_symbols.h"

DocTabDialog::~DocTabDialog() //释放分配的内存
{
	for (DocTabUserArea* i : doc_tab_dialog_arr) {
		if (i) {
			delete i;
		}
	}
	if (addDocTab) {
		delete addDocTab;
	}
	if (recDocTab) {
		delete recDocTab;
	}
}
void DocTabUserArea::DrawMsg(Int32 x1, Int32 y1, Int32 x2, Int32 y2, const BaseContainer& msg) {
	OffScreenOn(); //防止屏幕闪烁
	if (mode == DOC_TAB) //标签
	{
		if (doc != nullptr) {
			if (doc==GetActiveDocument()) //如果是活动文档则高亮
			{
				DrawBitmap(AutoBitmap("active.png"_s), x1, y1, x2, y2, 0, 0, 456, 185, BMP_NORMALSCALED | BMP_ALLOWALPHA);
				SetClippingRegion(x1 + 10, y1, x2 * 0.8, y2);
				DrawSetTextCol(Vector(223, 131, 60) / 256, COLOR_TRANS);
				DrawText(doc->GetDocumentName().GetFileString(), x1 + 10, y1 + 5);
				ClearClippingRegion();
				DrawBitmap(AutoBitmap("close.png"_s), x2 - 21, y1  + 5, 13, 13, 0, 0, 160, 160, BMP_NORMALSCALED | BMP_ALLOWALPHA);
			}
			else //不是活动文档
			{
				DrawBitmap(AutoBitmap("inactive.png"_s), x1, y1, x2, y2, 0, 0, 456, 185, BMP_NORMALSCALED | BMP_ALLOWALPHA);
				SetClippingRegion(x1 + 10, y1, x2 * 0.8, y2);
				DrawSetTextCol(Vector(165, 165, 165) / 256, COLOR_TRANS);
				DrawText(doc->GetDocumentName().GetFileString(), x1 + 10, y1 + 5);
				ClearClippingRegion();
				DrawBitmap(AutoBitmap("close_in.png"_s), x2 - 21, y1 + 5, 13, 13, 0, 0, 160, 160, BMP_NORMALSCALED | BMP_ALLOWALPHA);
			}
		}
	}
	else if (mode == ADD_TAB) //添加按钮
	{
		DrawBitmap(AutoBitmap("inactive.png"_s), x1, y1, x2, y2, 0, 0, 456, 185, BMP_NORMALSCALED | BMP_ALLOWALPHA);
		DrawSetTextCol(Vector(165, 165, 165) / 256, COLOR_TRANS);
		DrawText("+"_s, x2 * 0.3, y2 * 0.65 - 8);	
	}if (mode == REC_DOC) {
		DrawBitmap(AutoBitmap("inactive.png"_s), x1, y1, x2, y2, 0, 0, 456, 185, BMP_NORMALSCALED | BMP_ALLOWALPHA);
		DrawSetTextCol(Vector(165, 165, 165) / 256, COLOR_TRANS);
		DrawText("R"_s, x2 * 0.3, y2 * 0.65 - 8);
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
			if (ClickX >= (Int32)(this->GetWidth() - 21) && ClickX <= (Int32)(this->GetWidth() - 8) && ClickY >= 5 && ClickY <= 18) {
				//判断点击是在×的位置
				if (doc != nullptr) {
					if (msg.GetInt32(BFM_INPUT_QUALIFIER) == 1)//QUALIFIER::SHIFT = 1，判断是否同时按shift
					{
						KillDocument(doc);
					}
					else {
						if (IsCommandEnabled(12664))//call关闭项目的命令
						{
							CallCommand(12664);
						}
					}
					if (!((DocTabDialog*)dlg)->RefreshTab())return false;
				}
			}
		}
		else if (mode == ADD_TAB) {
			if (IsCommandEnabled(12094)) //call打开文档的命令
			{
				CallCommand(12094);
			}
			if (!((DocTabDialog*)dlg)->RefreshTab())return false;
		}
		else  if (mode == REC_DOC) {
			maxon::BaseArray<maxon::Url> RecDocArr = GetRecentDocumentsList(false).GetValue();
			BaseContainer bc;
			Int32 index = 1;//从1开始，防止与0冲突，最后的返回值得-1
			for (maxon::Url i : RecDocArr) //迭代最近文档，并设置索引
			{
				bc.SetString(index, i.GetPath());			
				index++;
			}
			Int32 openDoc = ShowPopupMenu(nullptr, MOUSEPOS, MOUSEPOS, bc);
			if (openDoc!=0) //返回0就是没有选择
			{
				if (!LoadFile(MaxonConvert(RecDocArr[openDoc - 1]))) {
					MessageDialog(GeLoadString(MSG_LOADFILE_FAIL));
				}
				if (!((DocTabDialog*)dlg)->RefreshTab())return false;
			}
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
	while (doc != nullptr)//遍历全部的项目，添加到标签
	{
		Int32 Index = doc_tab_dialog_arr.GetCount();
		DocTabUserArea* doc_tab_user_area = new DocTabUserArea(doc, DOC_TAB);
		Int32 TabW = 40;
		String& DocName = doc->GetDocumentName().GetFileString();
		for (Char i : DocName) {
			if (maxon::IsAlphanumeric(i) || maxon::IsSpace(i)) {
				TabW += 12;
			}
			else {
				TabW += 15;
			}
		}
		C4DGadget* const userAreaGadget = this->AddUserArea(10000 + Index, BFH_LEFT, TabW, 14);
		if (userAreaGadget != nullptr)
			this->AttachUserArea((*doc_tab_user_area), userAreaGadget);
		iferr(doc_tab_dialog_arr.Append(doc_tab_user_area))return false;
		doc = doc->GetNext();
	}
	addDocTab = new DocTabUserArea(nullptr, ADD_TAB);
	C4DGadget* userAreaGadget = this->AddUserArea(1001, BFH_LEFT, 30, 14);//添加添加标签的按钮
	if (userAreaGadget != nullptr)
		this->AttachUserArea((*addDocTab), userAreaGadget);
	recDocTab = new DocTabUserArea(nullptr, REC_DOC);
	userAreaGadget = this->AddUserArea(1002, BFH_LEFT, 30, 14);//添加历史文档的按钮
	if (userAreaGadget != nullptr)
		this->AttachUserArea((*recDocTab), userAreaGadget);
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
	for (DocTabUserArea* i : doc_tab_dialog_arr) //释放之前的标签
	{
		if (i) {
			delete i;
		}
	}
	doc_tab_dialog_arr.Reset();
	BaseDocument* doc = GetFirstDocument();
	LayoutFlushGroup(1000);//刷新组
	while (doc != nullptr)//遍历全部的项目，添加到标签
	{
		Int32 Index = doc_tab_dialog_arr.GetCount();
		DocTabUserArea* doc_tab_user_area = new DocTabUserArea(doc, DOC_TAB);
		Int32 TabW = 40;
		String& DocName = doc->GetDocumentName().GetFileString();
		for (Char i : DocName) {
			if (maxon::IsAlphanumeric(i) || maxon::IsSpace(i)) {
				TabW += 12;
			}
			else {
				TabW += 15;
			}
		}
		C4DGadget* const userAreaGadget = this->AddUserArea(10000 + Index, BFH_LEFT , TabW, 14);
		if (userAreaGadget != nullptr)
			this->AttachUserArea((*doc_tab_user_area), userAreaGadget);
		iferr(doc_tab_dialog_arr.Append(doc_tab_user_area))return false;
		doc = doc->GetNext();
	}
	C4DGadget* userAreaGadget = this->AddUserArea(1001, BFH_LEFT , 30, 14);//添加添加标签的按钮
	if (userAreaGadget != nullptr)
		this->AttachUserArea((*addDocTab), userAreaGadget);
	userAreaGadget = this->AddUserArea(1002, BFH_LEFT, 30, 14);//添加历史文档的按钮
	if (userAreaGadget != nullptr)
		this->AttachUserArea((*recDocTab), userAreaGadget);
	LayoutChanged(1000);
	return true;
}