# Binary Search Tree (BST) Implementation with Delete Functionality
## Overview
This repository contains a complete implementation of a Binary Search Tree (BST) in C, with a focus on the recursive `deleteNode` function. The implementation follows core BST properties (left subtree < root < right subtree) and handles all edge cases for node deletion, including leaf nodes, nodes with single children, and nodes with two children.

## Features
- **Core BST Operations**:
  - Create new tree nodes
  - Recursive insertion (adheres to BST property)
  - Recursive deletion (handles 4 key scenarios)
  - Find minimum node in a subtree (for two-child deletion)
- **Edge Case Handling**:
  - Null tree input protection
  - Duplicate value prevention (insert returns original node)
  - Memory management (proper `free()` for deleted nodes)

## Code Structure
| File | Purpose |
|------|---------|
| `bst.c` | Core implementation (node creation, insert, delete, findMinNode) |
| `bst.h` | Header file with `TreeNode` struct definition and function prototypes |
| `main.c` | Test driver code to validate BST operations |

## Key Function Explanations
### 1. TreeNode Structure
```c
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;
```
Basic tree node structure storing integer data and pointers to left/right children.

### 2. deleteNode Function
The recursive deletion function follows a "path-finding + backtracking connection" logic:
1. **Path Finding**: Recursively traverse left/right based on value comparison
2. **Node Deletion**:
   - Leaf node: Free memory and set to NULL
   - Single child: Replace node with its child
   - Two children: Replace value with min from right subtree, then delete the min node
3. **Backtracking**: Reconnect modified subtrees to maintain BST structure

### 3. findMinNode Function
```c
TreeNode* findMinNode(TreeNode* root) {
    if (root == NULL) return NULL;
    TreeNode* cur = root;
    while (cur->left != NULL) {
        cur = cur->left;
    }
    return cur;
}
```
Iteratively finds the minimum value node in a subtree (always the leftmost node in BST).

## Compilation & Execution
### Prerequisites
- GCC compiler (or any C compiler)
- VSCode (or any code editor/IDE)

### Compilation Command
```bash
gcc main.c bst.c -o bst_implementation
```

### Execution
```bash
./bst_implementation
```

## Usage Example
```c
// Create empty tree
TreeNode* root = NULL;

// Insert nodes
root = insertBST(root, 5);
root = insertBST(root, 3);
root = insertBST(root, 7);
root = insertBST(root, 2);
root = insertBST(root, 4);

// Delete leaf node (2)
root = deleteNode(root, 2);

// Delete node with single child (3)
root = deleteNode(root, 3);

// Delete node with two children (5)
root = deleteNode(root, 5);
```

## Key Notes
1. **Recursive Logic**: The `root->left = deleteNode(...)`/`root->right = deleteNode(...)` pattern is critical - it combines path finding (function call) and backtracking connection (assignment).
2. **Memory Safety**: Always free deleted nodes to prevent memory leaks.
3. **BST Property Preservation**: Deletion of two-child nodes uses right subtree minimum to maintain BST order.
4. **Null Protection**: All functions include NULL checks to prevent segmentation faults.

## Limitations
- This implementation only supports integer values
- No balancing mechanism (BST may degrade to linked list with sorted input)
- No duplicate value handling (insert returns original node for duplicates)

## Learning Objectives
This implementation is designed to help understand:
- Recursive tree traversal and backtracking
- BST deletion logic (the most complex BST operation)
- Pointer manipulation in tree data structures
- Memory management in C for dynamic data structures

## License
This project is for educational purposes only. Feel free to modify and use for personal learning.