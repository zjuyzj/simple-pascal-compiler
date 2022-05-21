#include "include/node_var.hpp"
#include "include/symbol_table.hpp"

bool VarDefNode::is_legal() {
    return symbol_table.findTypeSymbol(type->getName()) != nullptr;
}

bool VarDefNode::gen_sym_tab(int ord) {
    return symbol_table.addSymbol(name, this, ord);
}

int VarDefNode::get_length(void) {
    TypeAttrNode *type_def = symbol_table.findTypeSymbol(type->getName());
    if (type_def == nullptr) return -1;
    return type_def->get_length();
}

std::string VarDefNode::gen_asm_def(void) {
    TypeAttrNode *type_def = symbol_table.findTypeSymbol(type->getName());
    if (type_def == nullptr) return "";
    return type_def->gen_asm_def();
}

std::string VarDefNode::gen_viz_code() {
    std::string result = vizNode(uid, "VarDefNode\n" + name);
    result += vizChildEdge(uid, type->getUid());
    result += type->gen_viz_code();
    return result;
}

bool VarDefListNode::gen_sym_tab(void) {
    bool result       = true;
    int  lvars_length = 0;
    int  ord          = 0;
    for (VarDefNode *var : var_defs) {
        result &= var->gen_sym_tab(ord++);
        lvars_length += var->get_length();
    }
    ar_lvars_length.push_back(lvars_length);
    return result;
}

void VarDefListNode::addVarDef(IdListNode *ids, TypeAttrNode *type) {
    for (IdNode *id : ids->getIdList()) addVarDef(new VarDefNode(id->getName(), type));
}

std::string VarDefListNode::gen_asm_def(void) {
    std::string asm_def = "";
    for (VarDefNode *var : var_defs) asm_def += var->gen_asm_def();
    return asm_def;
}