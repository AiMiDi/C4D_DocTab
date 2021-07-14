#include "main.h"
#include "doctab.h"

Bool PluginStart()
{
	if (!RegisterCommandPlugin(ID_DOC_TAB, "DocTab"_s, 0, AutoBitmap("icon.tif"_s), "Doc tab for C4D"_s, NewObjClear(DocTab))) {
		return false;
	}
  return true;
}

void PluginEnd()
{
}

Bool PluginMessage(Int32 id, void *data)
{
	switch (id)
	{
	case (C4DPL_INIT_SYS):
	{
		if (!g_resource.Init()) {
			return false;	
		}
		return true;
		break;
	}
	}
   return false;
}

