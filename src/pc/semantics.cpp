#include "include/semantics.hpp"
using namespace std;

// void semanticAnalyser::analyseTree(treeNode *t) {
//     if (t == nullptr) return;
//     int enter = 0;
//     switch (t->getNodeKind()) {
//         case DK_Func:
//             st.addSymbol(t, DK_Func);
//             st.enterScope();
//             enter = 1;
//             st.addSymbol(t, DK_Var);
//             if (t->getChild().size() > 2) {
//                 treeNode *para = t->getChild().at(1);
//                 if (para->getNodeKind() != TK_Id) {
//                     while (para != nullptr) {
//                         st.addSymbol(para, DK_Var);
//                         para = para->getSibling();
//                     }
//                 }
//             }
//             break;
//         case DK_Const: st.addSymbol(t, DK_Const); break;
//         case DK_Var: st.addSymbol(t, DK_Var); break;
//         case DK_Proc:
//             st.addSymbol(t, DK_Proc);
//             st.enterScope();
//             enter = 1;
//             if (t->getChild().size() > 2) {
//                 treeNode *para = t->getChild().at(1);
//                 if (para->getNodeKind() != TK_Id) {
//                     while (para != nullptr) {
//                         st.addSymbol(para, DK_Var);
//                         para = para->getSibling();
//                     }
//                 }
//             }
//             break;
//         case DK_Type: st.addSymbol(t, DK_Type); break;
//         case SK_Assign:
//             if (t->getChild().size() > 0 && t->getChild().at(0)->getNodeKind() == TK_Id) {
//                 check_decl(t->getChild().at(0)->getIdName(), DK_Var, t->getLine());
//             }
//             break;
//         default: break;
//     }
//     int childNum = t->getChild().size();
//     if (childNum > 1 && (t->getNodeKind() == DK_Func || t->getNodeKind() == DK_Proc)) {
//         if (t->getChild().at(childNum - 1)->getNodeKind() == DK_Block) {
//             analyseTree(t->getChild().at(childNum - 1));
//         }
//     } else {
//         for (int i = 0; i < childNum; i++) { analyseTree(t->getChild().at(i)); }
//     }

//     if (enter == 1) { st.leaveScope(); }
//     if (t->getSibling() != nullptr) { analyseTree(t->getSibling()); }
// }
void semanticAnalyser::check_decl(string name, int kind, int line) {
    // TableItem *item = st.findSymbol(name, kind);
    // if (item == nullptr) {
    //     // cout<<"UNDEFINED"<<endl;
    //     err.add(UNDEFINED, name, line);
    // }
}