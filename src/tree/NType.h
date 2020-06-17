#ifndef NTYPE_H
#define NTYPE_H

#include "tree/Node.h"
#include "tree/NIdentifier.h"

struct NType : NExpression {
	NType(){}
	virtual ~NType() = default;

	virtual std::string to_string() = 0;

	// Note: NType is not visible. It just contains a type to compare with another
	virtual void accept(TreeVisitor & visitor) final {}

	virtual bool compare(NType & type) = 0;
};

// Identifier type
struct NIdType : NType {
	NIdentifier & id;

	NIdType(NIdentifier & id) : id(id) {}
	virtual ~NIdType() = default;

	virtual std::string to_string() override {
		return id.to_string();
	}

	bool compare(NType * type) override {
		NIdType * id_type = dynamic_cast<NIdType*>(type);
		return id_type && id_type->id.compare(id);
	}
};

#endif