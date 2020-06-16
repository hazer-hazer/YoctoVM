#ifndef FUNCTION_H
#define FUNCTION_H

#include <functional>
#include <vector>
#include <map>

#include "core/Object.h"
#include "tree/NFuncDecl.h"

class TreeVisitor;
class DataObject;

struct NFuncDecl;
struct Arg;
struct NExpression;

typedef std::vector<NExpression*> Arguments;

typedef std::map<std::string, DataObject*> BuiltinFuncArgs;
typedef std::function<DataObject*(BuiltinFuncArgs)> BuiltinFuncBody;

class Function : public Object {
public:
	Function() {}
	virtual ~Function() = default;

	virtual DataObject * apply(TreeVisitor & visitor, const Arguments & args) = 0;

	virtual std::string to_string() override;
};

class UserFunction : public Function {
public:
	UserFunction(NFuncDecl & func_decl) : func_decl(func_decl) {}
	virtual ~UserFunction() = default;

	// bool operator ==(const NFuncDecl & func_decl);
	
	virtual DataObject * apply(TreeVisitor & visitor, const Arguments & args) override;

	virtual std::string to_string() override;
private:
	NFuncDecl & func_decl;
};

class BuiltinFunction : public Function {
public:
	BuiltinFunction(const std::vector<std::string> & params,
					const BuiltinFuncBody & body)
		: params(params), body(body) {}
	virtual ~BuiltinFunction() = default;

	virtual DataObject * apply(TreeVisitor & visitor, const Arguments & args) override;

	virtual std::string to_string() override;

private:
	std::vector<std::string> params;
	BuiltinFuncBody body;
};

#endif