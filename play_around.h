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


// PROPER EXAMPLE OF SIMPLE AST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *value;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(char *value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = strdup(value);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void printAST(Node *node, int level) {
    if (node == NULL) return;
    printAST(node->right, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%s\n", node->value);
    printAST(node->left, level + 1);
}

int main() {
    Node *root = createNode("root");
    root->left = createNode("left");
    root->right = createNode("right");
    root->left->left = createNode("left.left");
    root->left->right = createNode("left.right");

    printf("Abstract Syntax Tree:\n");
    printAST(root, 0);
    return 0;
}


///// MOCK AST FOR EXCECUTION BUILDER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	char *value;
	struct Node *left;
	struct Node *right;
} Node;

Node* createNode(char *value) {
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->value = strdup(value);
	newNode->left = NULL;
	newNode->right = NULL;
	return (newNode);
}

// Manually construct an AST for testing
Node* createTestAST()
{
	// Example: ls | grep foo > output.txt
	Node *pipeNode = createNode("|");               // Root: Pipe operator
	pipeNode->left = createNode("ls");              // Left child: ls command

	Node *redirectNode = createNode(">");           // Right child: Redirection operator
	redirectNode->left = createNode("grep");        // Left child of redirection: grep command
	redirectNode->left->left = createNode("foo");   // Argument to grep: foo
	redirectNode->right = createNode("output.txt"); // Target file for redirection
	pipeNode->right = redirectNode;                // Attach redirection node to pipe
	return (pipeNode);                               // Return root of the tree
}

void printAST(Node *node, int level)
{
	if (node == NULL) return;
	printAST(node->right, level + 1);
	for (int i = 0; i < level; i++) printf("    ");
	printf("%s\n", node->value);
	printAST(node->left, level + 1);
}

int main()
{
	printf("Testing manually constructed AST:\n");
	Node *testAST = createTestAST();
	printAST(testAST, 0);
    // Pass this testAST to your teammate's execution function
    // executeAST(testAST);
    return 0;
}

