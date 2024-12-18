#pragma once

#define TOKEN_UNDEFINED 0
#define TOKEN_PROTO 1
#define TOKEN_LET 2
#define TOKEN_ADD 3
#define TOKEN_SUB 4
#define TOKEN_MUL 5
#define TOKEN_DIV 6
#define TOKEN_MOD 7
#define TOKEN_MOV 8
#define TOKEN_RET 9

#include <string>

enum IR_Error {
	ErrOk,
	ErrUndefinedToken,
	ErrUnexpectedToken,
	ErrSyntax,
	Err,
	ErrNoProject,
};

struct Token {
	typedef unsigned int TokenType;

	TokenType Type = TOKEN_UNDEFINED;
	std::string Kind;
};

struct Project {
	
};

class ParserIR {
	typedef const char* CString;

	private:
		Project* PointerExternalProject = NULL;

	public:
		IR_Error InitParser(Project& fromProject);
		IR_Error BuildFrom(CString fromSource);
		IR_Error FreeProject();
};
