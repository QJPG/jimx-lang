#include "../include/runtime.h"

int main(int argc, char* argv[]) {

	//Tests
	RuntimeState state;
	state.Constants.resize(1);
	state.Constants[0].Type = Value::INT;
	state.Constants[0].ValuePointer = new int(1);

	state.Buffer.push_back(ldc_b); state.Buffer.push_back(0);
	state.Buffer.push_back(ldc_b); state.Buffer.push_back(0);
	state.Buffer.push_back(iadd);
	state.Buffer.push_back(ist_rsx);
	state.Buffer.push_back(iload_eax);
	state.Buffer.push_back(imov);

	Jimx jimx;
	jimx.SetDefaultWorker(&state);

	return 0;
}
