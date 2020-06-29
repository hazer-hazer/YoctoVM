#ifndef FUNCTION_H
#define FUNCTION_H

enum class FunctionType {
	Script,
	Function
};

struct Chunk;

// TODO: Add types for args

class Function : public Object {
public:
	Function();
	virtual ~Function() = default;

	Chunk chunk;

	std::string get_name() const {
		return name;
	}

private:
	std::string name;
	int argc;
};

#endif