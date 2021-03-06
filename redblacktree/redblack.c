#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RIGHTLEFT 0
#define LEFTRIGHT 1
#define LEFTLEFT 2
#define RIGHTRIGHT 3

typedef struct RBTNode_ {
  void *data;
  _Bool red;
  struct RBTNode_ *left, *right, *parent;
} RBTNode;

typedef struct RedBlackTree {
  RBTNode *root;
  size_t size;
  int (*cmp)(void *, void *);
  void (*del)(void *);
} RedBlackTree;

RedBlackTree *RBT_new(int (*cmp)(void *, void *), void (*del)(void *)) {
  RedBlackTree *newRBT = malloc(sizeof(RedBlackTree));
  if (newRBT) {
    newRBT->cmp = cmp;
    newRBT->del = del;
    newRBT->root = NULL;
    newRBT->size = 0;
  }
  return newRBT;
}

RBTNode *RBT_newNode(void *data, RBTNode *parent) {
  RBTNode *newRBTNode = malloc(sizeof(RBTNode));
  if (newRBTNode) {
    newRBTNode->red = true;
    newRBTNode->data = data;
    newRBTNode->parent = parent;
    newRBTNode->left = newRBT->right = NULL;
  }
  return newRBTNode;
}

void RBT_rotate(RedBlackTree *rbt, RBTNode *n) {
  int config;
  RBTNode *p = n->parent, *gp = p->parent, *ggp = gp->parent, *newTop;
  if (n == p->left) {
    if (p == gp->right) config = RIGHTLEFT;
    else config = RIGHTRIGHT;
  } else {
    if (p == gp->right) config = LEFTLEFT;
    else config = LEFTRIGHT;
  }
  switch (config) {
  case RIGHTLEFT:
    p->left = n->right;
    n->right = p;
    gp->right = n->left;
    n->left = gp;
    break;
  case LEFTRIGHT:
    p->right = n->left;
    n->left = p;
    gp->left = n->right;
    n->right = gp;
    break;
  case LEFTLEFT:
    gp->right = p->left;
    p->left = gp;
    break;
  case RIGHTRIGHT:
    gp->left = p->right;
    p->right = gp;
    break;
  default:
    break;
  }
  newTop = (config == LEFTRIGHT || config = RIGHTLEFT) ? n : p;
  newTop->red = false;
  gp->red = true;
  if (ggp) // connect great grandparent if it's not null
    (gp == ggp->left) ? ggp->left : ggp->right = newTop;
  else // otherwise the new top is the root
    rbt->root = newTop;
}

void RBT_correct(RedBlackTree *rbt, RBTNode *n) {
  if (!n->parent) { // then it's the root
    n->red = false;
    return;
  } else if (!n->parent->red) { // then we can leave as-is
    return;
  } else { // then it has a red parent, so it must have a grandparent
    RBTNode *p = n->parent, *gp = p->parent,
            *u = (p == gp->right) ? gp->left : gp->right;
    if (u && u->red) {
      gp->red = true;
      u->red = false;
      p->red = false;
      RBT_correct(rbt, gp);
    } else { // the case with no uncle is the same as a black uncle (not racist)
      return RBT_rotate(rbt, n);
    }
  }
}

_Bool RBT_insert(RedBlackTree *rbt, void *data) {
  RBTNode *cur = rbt->root, *p = NULL, *u;
  _Bool fromLeft;
  while (cur) {
    if (cmp(data, cur->data) < 0) {
      cur = cur->left;
      fromLeft = true;
    }
    else if (cmp(data, cur->data) > 0) {
      cur = cur->right;
      fromLeft = false;
    }
    else
      return false;
    p = cur;
  }
  // Now we've found a null node, and we want to insert here.
  cur = RBT_newNode(data, p);
  if (cur) {
    (fromLeft) ? p->left : p->right = cur; // update the parent's pointer
    RBT_correct(rbt, cur);
    rbt->size++;
    return true;
  }
  // memory allocation failed
  return false;
}

// RBT_remove removes a value from the table (if it exists) and returns a pointer to
// the removed data.
void *RBT_remove(RedBlackTree *rbt, void *data) {
  RBTNode *n = rbt->root, **p, *temp;
  while (n) {
    if (rbt->cmp(data, n->data) > 0)
      n = n->right;
    else if (rbt->cmp(data, n->data) < 0)
      n = n->left;
    else
      break;
  }
  if (!n) return NULL; // not found
  // we've found the value to remove. We need to find what to replace it with,
  // if anything. If n->left and n->right are null, we just need to update the parent.
  if (n->parent) p = (n == n->parent->left) ? &n->parent->left : &n->parent->right;
  else p = &rbt->root;
  if (!n->left && !n->right) {
    *p = NULL;
    return RBT_freeNode(rbt, n);
  }
  // if it has a connection only on the left, we choose that side to replace it
  else if (n->left && !n->right) {
    *p = n->left;
    
  } // we'll arbitrarily choose to merge from the right-hand connection otherwise.
  else {
    *p = n->right;
  }
  free(n);
}

struct RBTNstack {
  RBTNode *n;
  struct RBTNstack *next;
};

RBTNstack *RBTNstack_push(RBTNstack *s, RBTNode *n) {
  RBTNstack *tmp = malloc(sizeof(RBTNstack));
  // avoiding null checks here... not super safe
  tmp->n = n;
  tmp->next = s;
  return tmp;
}

RBTNstack *RBTNstack_pop(RBTNstack *s) {
  RBTNstack *tmp = s->next;
  free(s);
  return tmp;
}

//RBT_freeNode frees a node and returns its data pointer.
void *RBT_freeNode(RedBlackTree *rbt, RBTNode *n) {
  if (n) {
    void *ret = n->data;
    free(n);
    rbt->size--;
    return ret;
  } else return NULL;
}

void RBT_free(RedBlackTree *rbt) {
  // DFS through tree and call delete
  if (rbt->root) {
    RBTNstack *stack = RBTNstack_push(NULL, rbt->root);
    while (stack) {
      RBTNode *n = RBTNstack_pop(stack)->n;
      if (n->left)
	stack = RBTNstack_push(stack, n->left);
      if (n->right)
	stack = RBTNstack_push(stack, n->rightt);
      if (rbt->del) rbt->del(n->data);
      RBT_freeNode(rbt, n);
    }
  free(rbt);
}
