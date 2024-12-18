#include "../include/runtime.h"
#include <string>


void Jimx::ProtoMakeBinExpression(Worker& worker, Proto::ByteOperation operation) {

	for (RangeAccumulator = 0; RangeAccumulator < worker.EntryConstMemoryOrder; ++RangeAccumulator) {
		if (worker.WorkerRuntimeState->CSNTMM[RangeAccumulator] == NULL)
			return PushError(worker, RuntimeError::ErrorNullReadMemoryIndex);

		switch (operation) {
			case iadd:
				if (RangeAccumulator < 1)
					worker.WorkerRuntimeState->IMM0[EAX] = *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				else
					worker.WorkerRuntimeState->IMM0[EAX] += *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				break;

			case isub:
				if (RangeAccumulator < 1)
					worker.WorkerRuntimeState->IMM0[EAX] = *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				else
					worker.WorkerRuntimeState->IMM0[EAX] -= *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				break; 

			case imul:
				if (RangeAccumulator < 1)
					worker.WorkerRuntimeState->IMM0[EAX] = *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				else
					worker.WorkerRuntimeState->IMM0[EAX] *= *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				break;

			case idiv:
				if (RangeAccumulator < 1)
					worker.WorkerRuntimeState->IMM0[EAX] = *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				else
					worker.WorkerRuntimeState->IMM0[EAX] /= *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				break;

			case imod:
				if (RangeAccumulator < 1)
					worker.WorkerRuntimeState->IMM0[EAX] = *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				else
					worker.WorkerRuntimeState->IMM0[EAX] %= *(static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]));
				break;
		}
	}
	
	for (RangeAccumulator = 0; RangeAccumulator < worker.EntryConstMemoryOrder; RangeAccumulator++) {
		worker.WorkerRuntimeState->CSNTMM[RangeAccumulator] = NULL;
	}

	worker.EntryConstMemoryOrder = 0;

}

void Jimx::ProtoMakeLoadRegister(Worker& worker, Proto::ByteOperation operation) {
	switch (operation) {
		case iload_eax:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] = &worker.WorkerRuntimeState->IMM0[EAX];
			break;

		case iload_rsx:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] = &worker.WorkerRuntimeState->IMM0[RSX];
			break;

		case iload_mm0:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] = &worker.WorkerRuntimeState->IMM0[MM0];
			break;

		case iload_mm1:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] = &worker.WorkerRuntimeState->IMM0[MM1];
			break;

		case iload_mm2:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] = &worker.WorkerRuntimeState->IMM0[MM2];
			break;
	}

	if (worker.EntryConstMemoryOrder < RMX)
		worker.EntryConstMemoryOrder++;
}

void Jimx::ProtoMakeMoveValue(Worker& worker, Proto::ByteOperation operation) {
	if (worker.EntryConstMemoryOrder - worker.EntryReadMemoryOrder != 0) {
		return PushError(worker, RuntimeError::ErrorNotEqualEntries);
	}

	for (RangeAccumulator = 0; RangeAccumulator < worker.EntryReadMemoryOrder; RangeAccumulator++) {
		switch (operation) {
			case imov: {
				*static_cast<int*>(worker.WorkerRuntimeState->READMM[RangeAccumulator]) =
					*static_cast<int*>(worker.WorkerRuntimeState->CSNTMM[RangeAccumulator]);
				break;
			}
		}
	}

	for (RangeAccumulator = 0; RangeAccumulator < worker.EntryReadMemoryOrder; RangeAccumulator++) {
		worker.WorkerRuntimeState->READMM[RangeAccumulator] = NULL;
	}

	worker.EntryReadMemoryOrder = 0;


	for (RangeAccumulator = 0; RangeAccumulator < worker.EntryConstMemoryOrder; RangeAccumulator++) {
		worker.WorkerRuntimeState->CSNTMM[RangeAccumulator] = NULL;
	}

	worker.EntryConstMemoryOrder = 0;

}

void Jimx::ProtoMakeLoadConstants(Worker& worker, Proto::ByteOperation operation) {
	switch (operation) {
		case ldc_b:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] =
				worker.WorkerRuntimeState->Constants[GetNextByte(worker)].ValuePointer;
			break;

		case ldc_s:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] =
				worker.WorkerRuntimeState->Constants[GetNextMulti(worker)].ValuePointer;
			break;

		case ldc_i:
			worker.WorkerRuntimeState->CSNTMM[worker.EntryConstMemoryOrder] =
				worker.WorkerRuntimeState->Constants[GetNextMMulti(worker)].ValuePointer;
			break;
	}

	if (worker.EntryConstMemoryOrder < RMX)
		worker.EntryConstMemoryOrder++;
}

void Jimx::ProtoMakeStoreRegister(Worker& worker, Proto::ByteOperation operation) {
	switch (operation) {
		case ist_eax:
			worker.WorkerRuntimeState->READMM[worker.EntryReadMemoryOrder] =
				&worker.WorkerRuntimeState->IMM0[EAX];
			break;

		case ist_rsx:
			worker.WorkerRuntimeState->READMM[worker.EntryReadMemoryOrder] =
				&worker.WorkerRuntimeState->IMM0[RSX];
			break;

		case ist_mm0:
			worker.WorkerRuntimeState->READMM[worker.EntryReadMemoryOrder] =
				&worker.WorkerRuntimeState->IMM0[MM0];
			break;

		case ist_mm1:
			worker.WorkerRuntimeState->READMM[worker.EntryReadMemoryOrder] =
				&worker.WorkerRuntimeState->IMM0[MM1];
			break;

		case ist_mm2:
			worker.WorkerRuntimeState->READMM[worker.EntryReadMemoryOrder] =
				&worker.WorkerRuntimeState->IMM0[MM2];
			break;
	}

	if (worker.EntryReadMemoryOrder < RMX)
		worker.EntryReadMemoryOrder++;
}

void Jimx::ReadProto(Worker& worker) {
	if (worker.WorkerRuntimeState == NULL) return PushError(worker, RuntimeError::ErrorWorkerStateMissing);

	RuntimeState::Byte Op = worker.WorkerRuntimeState->Buffer[worker.BufferCursor]; 


	switch (Op) {
		case iadd: case isub: case imul: case idiv: case imod:
			ProtoMakeBinExpression(worker, Op); break;
		
		case iload_eax: case iload_rsx: case iload_mm0: case iload_mm1: case iload_mm2:
			ProtoMakeLoadRegister(worker, Op); break;

		case ldc_b: case ldc_s: case ldc_i:
			ProtoMakeLoadConstants(worker, Op); break;

		case imov:
			ProtoMakeMoveValue(worker, Op); break;

		case ist_eax: case ist_rsx: case ist_mm0: case ist_mm1: case ist_mm2:
			ProtoMakeStoreRegister(worker, Op);
	}

	/*

	Proto& NextProto = worker.WorkerRuntimeState->Stack[worker.ReadingBufferCursor];

	switch (NextProto.Operation) {
		case iadd: case isub: case imul: case idiv: case imod:
			ProtoMakeBinExpression(worker, NextProto.Operation); break;
		case iload_eax: case iload_rsx: case iload_mm0: case iload_mm1: case iload_mm2:
			ProtoMakeLoadRegister(worker, NextProto.Operation); break;
	}

	if (worker.ReadingBufferCursor < worker.WorkerRuntimeState->Stack.size())
		worker.ReadingBufferCursor++;
	else
		worker.State = Worker::WorkerState::StateFinished;
		*/

	if (worker.BufferCursor < worker.WorkerRuntimeState->Buffer.size()) {
		worker.BufferCursor++;
	}

	else {
		worker.State = Worker::WorkerState::StateFinished;
	}
}

void Jimx::PushError(Worker& worker, RuntimeError error) {
	printf("Error: Runtime: %i\n", error);
	PrintRuntime(worker);
	exit(error);
}

void Jimx::RunWorker(Worker& worker) {
	if (worker.WorkerRuntimeState == NULL) return PushError(worker, RuntimeError::ErrorWorkerStateMissing);

	worker.State = Worker::WorkerState::StateRunning;

	bool Loop = true;

	while (Loop) {
		switch (worker.State) {
			case Worker::WorkerState::StateIDLE: break;
			case Worker::WorkerState::StateRunning: ReadProto(worker); break;
			case Worker::WorkerState::StateFinished: Loop = false; break;
		}
	}

	printf(">> %i", worker.WorkerRuntimeState->IMM0[RSX]);
}

void Jimx::PrintRuntime(Worker& worker) {
	std::string WriteLine = "";

	for (size_t i = 0; i < worker.WorkerRuntimeState->Buffer.size(); i++) {
		if (i == worker.BufferCursor) WriteLine += ">> ";
		else WriteLine += "   ";
		
		WriteLine += "| ";
		WriteLine += "p+";
		WriteLine += std::to_string(i);
		WriteLine += " ";
		WriteLine += "OP: ";
		WriteLine += std::to_string(worker.WorkerRuntimeState->Buffer[i]);
		WriteLine += "\n";
	}

	printf("%s", WriteLine.c_str());
}

unsigned short Jimx::GetNextMulti(Worker& worker) {
	if (worker.BufferCursor + 2 >= worker.WorkerRuntimeState->Buffer.size()) PushError(worker, RuntimeError::ErrorByteOutOfRange);

	return worker.WorkerRuntimeState->Buffer[++worker.BufferCursor] * worker.WorkerRuntimeState->Buffer[++worker.BufferCursor];
}

unsigned char Jimx::GetNextByte(Worker& worker) {
	if (worker.BufferCursor + 1 >= worker.WorkerRuntimeState->Buffer.size()) PushError(worker, RuntimeError::ErrorByteOutOfRange);
	
	return worker.WorkerRuntimeState->Buffer[++worker.BufferCursor];
}

unsigned int Jimx::GetNextMMulti(Worker& worker) {
	if (worker.BufferCursor + 3 >= worker.WorkerRuntimeState->Buffer.size()) PushError(worker, RuntimeError::ErrorByteOutOfRange);

	return worker.WorkerRuntimeState->Buffer[++worker.BufferCursor] * \
				worker.WorkerRuntimeState->Buffer[++worker.BufferCursor] * \
					worker.WorkerRuntimeState->Buffer[++worker.BufferCursor];
}

void Jimx::SetDefaultWorker(RuntimeState* fromState) {
	MainWorker = new Worker {};
	MainWorker->WorkerRuntimeState = fromState;
	MainWorker->State = Worker::WorkerState::StateIDLE;
	MainWorker->BufferCursor = 0;

	RunWorker(*MainWorker);
}


