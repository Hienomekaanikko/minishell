AST for:

cat file.txt | grep "hello" > output.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node types
typedef enum {
    NODE_COMMAND,
    NODE_PIPELINE,
    NODE_REDIRECTION
} NodeType;

// Redirection structure
typedef struct Redirect {
    char *operator;  // e.g., ">", "<"
    char *target;    // e.g., "output.txt"
} Redirect;

// Command node structure
typedef struct CommandNode {
    char *name;         // Command name (e.g., "cat", "grep")
    char **arguments;   // Array of arguments
    int arg_count;      // Number of arguments
    Redirect *redirect; // Optional redirection
} CommandNode;

// Pipeline node structure
typedef struct PipelineNode {
    struct ASTNode *left;  // Left command or pipeline
    struct ASTNode *right; // Right command or pipeline
} PipelineNode;

// Generic AST node structure
typedef struct ASTNode {
    NodeType type;  // Type of the node (command, pipeline, etc.)
    union {
        CommandNode command;
        PipelineNode pipeline;
    } data;
} ASTNode;

// Function to create a command node
ASTNode *create_command_node(const char *name, char **arguments, int arg_count, Redirect *redirect) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_COMMAND;
    node->data.command.name = strdup(name);
    node->data.command.arguments = arguments;
    node->data.command.arg_count = arg_count;
    node->data.command.redirect = redirect;
    return node;
}

// Function to create a pipeline node
ASTNode *create_pipeline_node(ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PIPELINE;
    node->data.pipeline.left = left;
    node->data.pipeline.right = right;
    return node;
}

// Function to create a redirection
Redirect *create_redirection(const char *operator, const char *target) {
    Redirect *redirect = malloc(sizeof(Redirect));
    redirect->operator = strdup(operator);
    redirect->target = strdup(target);
    return redirect;
}

int main() {
    // Create arguments for "cat"
    char *cat_args[] = {"file.txt", NULL};

    // Create the "cat" command node
    ASTNode *cat_node = create_command_node("cat", cat_args, 1, NULL);

    // Create arguments for "grep"
    char *grep_args[] = {"hello", NULL};

    // Create redirection for "grep"
    Redirect *grep_redirect = create_redirection(">", "output.txt");

    // Create the "grep" command node with redirection
    ASTNode *grep_node = create_command_node("grep", grep_args, 1, grep_redirect);

    // Create the pipeline connecting "cat" and "grep"
    ASTNode *pipeline_node = create_pipeline_node(cat_node, grep_node);

    // Print structure (for demonstration)
    printf("Pipeline:\n");
    printf("  Left: %s %s\n", pipeline_node->data.pipeline.left->data.command.name,
           pipeline_node->data.pipeline.left->data.command.arguments[0]);
    printf("  Right: %s %s > %s\n", pipeline_node->data.pipeline.right->data.command.name,
           pipeline_node->data.pipeline.right->data.command.arguments[0],
           pipeline_node->data.pipeline.right->data.command.redirect->target);

    return 0;
}


// Function to free the AST (not shown here for brevity)
