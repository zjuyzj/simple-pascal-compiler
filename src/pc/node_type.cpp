#include "include/node_type.hpp"
#include "include/symbol_table.hpp"

SymbolTable sym_tab;

TypeDefNode::TypeDefNode(std::string id, TypeAttrNode* t) : identifier(id), type(t) {
    sym_tab.addSymbol(id, t);
}

int TypeAttrNode::get_length(void) {
    switch (this->root_type) {
        case basic: return this->basic_attr->get_length();
        case ordinal: return this->ord_attr->get_length();
        case structured: return this->struct_attr->get_length();
        case pointer: return BASIC_PTR_LEN;
        case type_identifier: return sym_tab.findSymbol(this->type_id)->get_length();
    }
    return -1;
}

int TypeAttrNode::get_offset(void) {
    // TODO
    return -1;
}
int TypeAttrNode::get_offset(std::vector<int> indexs) {
    // TODO
    return -1;
}
int TypeAttrNode::get_offset(std::string member) {
    // TODO
    return -1;
}

bool TypeAttrNode::is_type_equ(TypeAttrNode* type, bool use_struct) {
    if (!use_struct)
        return this == type;
    if (type->root_type != this->root_type)
        return false;
    switch (this->root_type) {
        case basic: return this->basic_attr->is_type_equ(type);
        case ordinal: return this->ord_attr->is_type_equ(type);
        case structured: return this->struct_attr->is_type_equ(type);
        case pointer: return type->root_type == pointer;
        case type_identifier: {
            TypeAttrNode* lut_this = sym_tab.findSymbol(this->type_id);
            TypeAttrNode* lut_type = sym_tab.findSymbol(type->type_id);
            return lut_this->is_type_equ(lut_type);
        }
    }
    return false;
}

basic_type_kind BasicAttrNode::get_type(void) {
    return this->type;
}

int BasicAttrNode::get_length(void) {
    switch (this->type) {
        case boolean: return BASIC_BOOL_LEN;
        case integer: return BASIC_INT_LEN2;
        case real: return BASIC_REAL_LEN;
        case character: return BASIC_CHAR_LEN;
        case pointer: return BASIC_PTR_LEN;
    }
    return -1;
}

int BasicAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool BasicAttrNode::is_type_equ(TypeAttrNode* type) {
    if (type->root_type != basic)
        return false;
    return this->is_type_equ(type->basic_attr);
}
bool BasicAttrNode::is_type_equ(BasicAttrNode* type) {
    return type->type == this->type;
}

OrdAttrNode::OrdAttrNode(int lower_bound, int upper_bound)
        : is_subrange(true),
          subrange_attr(new SubrangeAttrNode(lower_bound, upper_bound)),
          enum_attr(nullptr) {}
OrdAttrNode::OrdAttrNode(char lower_bound, char upper_bound)
        : is_subrange(true),
          subrange_attr(new SubrangeAttrNode(lower_bound, upper_bound)),
          enum_attr(nullptr) {}
OrdAttrNode::OrdAttrNode(std::vector<std::string> ids)
        : is_subrange(false), subrange_attr(nullptr), enum_attr(new EnumAttrNode(ids)) {}
OrdAttrNode::~OrdAttrNode() {
    if (this->subrange_attr != nullptr)
        delete this->subrange_attr;
    if (this->enum_attr != nullptr)
        delete this->enum_attr;
}

int OrdAttrNode::get_length(void) {
    return this->is_subrange ? this->subrange_attr->get_length() : this->enum_attr->get_length();
}

int OrdAttrNode::get_size(void) {
    return this->is_subrange ? this->subrange_attr->get_size() : this->enum_attr->get_size();
}

int OrdAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool OrdAttrNode::is_type_equ(TypeAttrNode* type) {
    if (type->root_type != ordinal)
        return false;
    return this->is_type_equ(type->ord_attr);
}
bool OrdAttrNode::is_type_equ(OrdAttrNode* type) {
    if (type->is_subrange && this->is_subrange)
        return this->subrange_attr->is_type_equ(type->subrange_attr);
    if (!type->is_subrange && !this->is_subrange)
        return this->enum_attr->is_type_equ(type->enum_attr);
    return false;
}

int SubrangeAttrNode::get_length(void) {
    return 2 * BASIC_INT_LEN2 + (this->is_int_bound ? BASIC_INT_LEN2 : BASIC_CHAR_LEN);
}

int SubrangeAttrNode::get_size(void) {
    return this->bounds[1] - this->bounds[0] + 1;
}

int SubrangeAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool SubrangeAttrNode::is_type_equ(TypeAttrNode* type) {
    if (type->root_type != ordinal)
        return false;
    return this->is_type_equ(type->ord_attr);
}
bool SubrangeAttrNode::is_type_equ(OrdAttrNode* type) {
    if (!type->is_subrange)
        return false;
    return this->is_type_equ(type->subrange_attr);
}
bool SubrangeAttrNode::is_type_equ(SubrangeAttrNode* type) {
    if (type->is_int_bound != this->is_int_bound)
        return false;
    return type->bounds[0] == this->bounds[0] && type->bounds[1] == this->bounds[1];
}

void EnumAttrNode::addItem(std::string id) {
    this->identifiers.push_back(id);
}

int EnumAttrNode::get_length(void) {
    return BASIC_INT_LEN2 * this->identifiers.size();
}

int EnumAttrNode::get_size(void) {
    return this->identifiers.size();
}

int EnumAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool EnumAttrNode::is_type_equ(TypeAttrNode* type) {
    if (type->root_type != ordinal)
        return false;
    return this->is_type_equ(type->ord_attr);
}
bool EnumAttrNode::is_type_equ(OrdAttrNode* type) {
    if (type->is_subrange)
        return false;
    return this->is_type_equ(type->enum_attr);
}
bool EnumAttrNode::is_type_equ(EnumAttrNode* type) {
    if (type->identifiers.size() != this->identifiers.size())
        return false;
    for (int i = 0; i < this->identifiers.size(); i++)
        if (type->identifiers.at(i) != this->identifiers.at(i))
            return false;
    return true;
}

StructAttrNode::StructAttrNode(std::vector<TypeAttrNode*> it, TypeAttrNode* et)
        : type(array), array_attr(new ArrayAttrNode(it, et)), record_attr(nullptr) {}
StructAttrNode::~StructAttrNode() {
    if (this->array_attr != nullptr)
        delete this->array_attr;
    if (this->record_attr != nullptr)
        delete this->record_attr;
}

int StructAttrNode::get_length(void) {
    switch (this->type) {
        case array: return this->array_attr->get_length();
        case record: return this->record_attr->get_length();
    }
    return -1;
}

int StructAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool StructAttrNode::is_type_equ(TypeAttrNode* type) {
    switch (this->type) {
        case array: return this->array_attr->is_type_equ(type);
        case record: return this->record_attr->is_type_equ(type);
    }
    return false;
}

int ArrayAttrNode::get_dim() {
    return this->index_type.size();
}

int ArrayAttrNode::get_length(void) {
    int result = this->element_type->get_length();
    for (TypeAttrNode* type : this->index_type) {
        if (type->root_type != ordinal)
            return -1;
        result *= type->ord_attr->get_size();
    }
    return result;
}

int ArrayAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool ArrayAttrNode::is_type_equ(TypeAttrNode* type) {
    if (type->root_type != structured)
        return false;
    return this->is_type_equ(type->struct_attr);
}
bool ArrayAttrNode::is_type_equ(StructAttrNode* type) {
    if (type->type != array)
        return false;
    return this->is_type_equ(type->array_attr);
}
bool ArrayAttrNode::is_type_equ(ArrayAttrNode* type) {
    if (type->get_dim() != this->get_dim())
        return false;
    if (!this->element_type->is_type_equ(type->element_type))
        return false;
    for (int i = 0; i < this->index_type.size(); i++)
        if (!this->index_type.at(i)->is_type_equ(type->index_type.at(i)))
            return false;
    return true;
}

int RecordAttrNode::get_dim() {
    return this->identifiers.size();
}

int RecordAttrNode::get_length(void) {
    int result = 0;
    for (TypeAttrNode* type : this->types) result += type->get_length();
    return result;
}

int RecordAttrNode::get_offset(void) {
    // TODO
    return -1;
}

bool RecordAttrNode::is_type_equ(TypeAttrNode* type) {
    if (type->root_type != structured)
        return false;
    return this->is_type_equ(type->struct_attr);
}
bool RecordAttrNode::is_type_equ(StructAttrNode* type) {
    if (type->type != record)
        return false;
    return this->is_type_equ(type->record_attr);
}
bool RecordAttrNode::is_type_equ(RecordAttrNode* type) {
    if (type->get_dim() != this->get_dim())
        return false;
    for (int i = 0; i < this->identifiers.size(); i++) {
        if (this->identifiers.at(i) != type->identifiers.at(i))
            return false;
        if (!this->types.at(i)->is_type_equ(type->types.at(i)))
            return false;
    }
    return true;
}