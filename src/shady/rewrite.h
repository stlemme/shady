#ifndef SHADY_REWRITE_H
#define SHADY_REWRITE_H

#include "shady/ir.h"

typedef struct Rewriter_ Rewriter;

typedef const Node* (*RewriteFn)(Rewriter*, const Node*);

const Node* rewrite_node(Rewriter*, const Node*);
const Node* rewrite_node_with_fn(Rewriter*, const Node*, RewriteFn);

/// Applies the rewriter to all nodes in the collection
Nodes rewrite_nodes(Rewriter*, Nodes);
Nodes rewrite_nodes_with_fn(Rewriter* rewriter, Nodes values, RewriteFn fn);

Strings import_strings(IrArena*, Strings);

struct Rewriter_ {
    RewriteFn rewrite_fn;
    struct {
        RewriteFn rewrite_type;
        RewriteFn rewrite_value;
        RewriteFn rewrite_instruction;
        RewriteFn rewrite_terminator;
        RewriteFn rewrite_decl;
        RewriteFn rewrite_anon_lambda;
        RewriteFn rewrite_basic_block;
        RewriteFn rewrite_annotation;
    } rewrite_field_type;
    IrArena* src_arena;
    IrArena* dst_arena;
    Module* src_module;
    Module* dst_module;
    struct {
        bool search_map;
        bool write_map;
    } config;
    struct Dict* map;
    struct Dict* decls_map;
};

Rewriter create_rewriter(Module* src, Module* dst, RewriteFn fn);
Rewriter create_importer(Module* src, Module* dst);
Rewriter create_substituter(Module* arena);
void destroy_rewriter(Rewriter*);

void rewrite_module(Rewriter*);

/// Rewrites a node using the rewriter to provide the node and type operands
const Node* recreate_node_identity(Rewriter*, const Node*);

/// Rewrites a constant / function header
Node* recreate_decl_header_identity(Rewriter*, const Node*);
void  recreate_decl_body_identity(Rewriter*, const Node*, Node*);

/// Rewrites a variable under a new identity
const Node* recreate_variable(Rewriter* rewriter, const Node* old);
Nodes recreate_variables(Rewriter* rewriter, Nodes old);

/// Looks up if the node was already processed
const Node* search_processed(const Rewriter*, const Node*);
/// Same as search_processed but asserts if it fails to find a mapping
const Node* find_processed(const Rewriter*, const Node*);
void register_processed(Rewriter*, const Node*, const Node*);
void register_processed_list(Rewriter*, Nodes, Nodes);
void clear_processed_non_decls(Rewriter*);

#endif
