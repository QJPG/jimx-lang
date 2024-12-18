#include "../include/ir.h"

IR_Error ParserIR::InitParser(Project& fromProject) {
	PointerExternalProject = &fromProject;
	
	return IR_Error::ErrOk;
}

IR_Error ParserIR::BuildFrom(CString fromSource) {

	return IR_Error::ErrOk;
}

IR_Error ParserIR::FreeProject() {
	PointerExternalProject = NULL;

	return IR_Error::ErrOk;
}
