#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

#define RMX 5
#define EAX 0
#define RSX 1
#define MM0 2
#define MM1 3
#define MM2 4

#define nop 0
#define iadd 1
#define isub 2
#define imul 3
#define idiv 4
#define imod 5
#define iload_eax 6
#define iload_rsx 7
#define iload_mm0 8
#define iload_mm1 9
#define iload_mm2 10
#define ldc_b 11 //Next One Byte
#define ldc_s 12 //Next One Byte * Next Next One Byte
#define ldc_i 13 //... * Next One Byte
#define ist_eax 14
#define ist_rsx 15
#define ist_mm0 16
#define ist_mm1 17
#define ist_mm2 18
#define imov 19

#define iload_c 7
#define xmmt_int	8
#define xmmt_long	9
#define xmmt_float	10
#define xmmt_double	11
#define proto_swap 12
#define rmmx8_set 13
#define cmp_eq 14
#define jadd 15
#define jsub 16
#define mov 17
#define ret 18

#define op_b255 0
#define op_b32b 1

#define _GenericExpressionBinNotAssign(typeA, typeB, _ptrA, _ptrB, _exp_)\
	(*static_cast<typeA*>(_ptrA)) _exp_ (*static_cast<typeB*>(_ptrB));

#define _GenericExpressionBin(typeA, typeB, _ptrA, _ptrB, _exp_)\
	(*static_cast<typeA*>(_ptrA)) = _GenericExpressionBinNotAssign(typeA, typeB, _ptrA, _ptrB, _exp_);

#define _BinaryExpression(_Type, _Sig, _PtrA, _PtrB) \
	(*static_cast<_Type*>(_PtrA)) _Sig (*static_cast<_Type*>(_PtrB));

enum Error {
	Fine,
	Oops,
};

enum RuntimeError {
	ErrorWorkerStateMissing,
	ErrorProtoBaseUndefined,
	ErrorNullReadMemoryIndex,
	ErrorByteOutOfRange,
	ErrorNotEqualEntries,
};


struct Proto255 {
	typedef unsigned char U255ByteCode;

	U255ByteCode Entry = 0;
	U255ByteCode Base = 0;
};

struct Proto32b {
	typedef short b32ByteCode;

	Proto255::U255ByteCode Entry = 0;
	b32ByteCode Base = 0;

};






struct Proto {
	typedef unsigned char ByteOperation;

	ByteOperation Operation = nop;
};

struct ByteBuffer {
	typedef unsigned char Byte;
	typedef int Entry;
	typedef std::vector<Byte> ByteVec;

	private:
		ByteVec	Data = {};

	public:
		static const Byte ByteShift = 0xFF;
		
		void Insert(Entry  input, size_t fromBit, size_t bits);
		void ReadAt(Entry& output, size_t fromBit, size_t bits);

};

struct Value {
	enum ValueType {
		INT, LONG, FLOAT, DOUBLE
	};

	ValueType Type = Value::INT;
	void* ValuePointer = NULL;

	~Value() {
		if (ValuePointer == NULL) return;

		switch (Type) {
			case INT: delete static_cast<int*>(ValuePointer); break;
			case LONG: delete static_cast<long*>(ValuePointer); break;
			case FLOAT: delete static_cast<float*>(ValuePointer); break;
			case DOUBLE: delete static_cast<double*>(ValuePointer); break;
		}
	}
};

struct RuntimeState {
	typedef std::vector<Proto> ProtoStack;
	typedef void* Pointer;
	typedef unsigned char Byte;
	typedef std::vector<unsigned char> ByteBuffer;
	typedef std::vector<Value> ConstantsBuffer;

	int		IMM0[RMX] = {0};
	long	IMM1[RMX] = {0};
	
	float	XMM0[RMX] = {0.f};
	double	XMM1[RMX] = {0.0};
	
	
	Pointer READMM[RMX] = { NULL }; //Usado para escrita de valores
	Pointer CSNTMM[RMX] = { NULL }; //Usado para leitura de valores
	
	ProtoStack Stack;
	ByteBuffer Buffer;
	ConstantsBuffer Constants;

};

struct Worker {
	enum WorkerState {
		StateIDLE,
		StateRunning,
		StateFinished,
	};

	RuntimeState*	WorkerRuntimeState = NULL;
	WorkerState		State;
	size_t			EntryReadMemoryOrder;
	size_t			EntryConstMemoryOrder;
	size_t			BufferCursor;
};

class Jimx {
	private:
		size_t RangeAccumulator = 0;
		
		Worker* MainWorker = NULL;

		void ProtoMakeBinExpression(Worker& worker, Proto::ByteOperation operation);
		void ProtoMakeLoadRegister(Worker& worker, Proto::ByteOperation operation);
		void ProtoMakeMoveValue(Worker& worker, Proto::ByteOperation operation);
		void ProtoMakeLoadConstants(Worker& worker, Proto::ByteOperation operation);
		void ProtoMakeStoreRegister(Worker& worker, Proto::ByteOperation operation);

		void ReadProto(Worker& worker);
		void PushError(Worker& worker, RuntimeError error);
		void RunWorker(Worker& worker);
		void PrintRuntime(Worker& worker);

		unsigned char GetNextByte(Worker& worker);
		unsigned short GetNextMulti(Worker& worker);
		unsigned int GetNextMMulti(Worker& worker);

	public:
		void SetDefaultWorker(RuntimeState* fromState);
};


