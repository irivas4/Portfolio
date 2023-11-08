package hw6.bst;

import hw6.OrderedMap;
import java.util.Iterator;
import java.util.Stack;

/**
 * Map implemented as an AVL Tree.
 *
 * @param <K> Type for keys.
 * @param <V> Type for values.
 */
public class AvlTreeMap<K extends Comparable<K>, V> implements OrderedMap<K, V> {

  /*** Do not change variable name of 'root'. ***/
  private Node<K, V> root;
  private int size;

  @Override
  public void insert(K k, V v) throws IllegalArgumentException {
    if (k == null) {
      throw new IllegalArgumentException("cannot handle null key");
    }
    root = insert(root, k, v);
    size++;
  }

  private Node<K, V> insert(Node<K, V> n, K k, V v) {
    if (n == null) {
      return new Node<>(k, v);
    }
    int cmp = k.compareTo(n.key);
    if (cmp < 0) {
      n.left = insert(n.left, k, v);
    } else if (cmp > 0) {
      n.right = insert(n.right, k, v);
    } else {
      throw new IllegalArgumentException("duplicate key " + k);
    }
    n.height = 1 + max(height(n.left), height(n.right));

    return balancer(n);
  }

  @Override
  public V remove(K k) throws IllegalArgumentException {
    Node<K, V> node = findForSure(k);
    V value = node.value;
    root = remove(root, node);
    size--;
    return value;
  }

  private Node<K, V> remove(Node<K, V> subtreeRoot, Node<K, V> toRemove) {
    int cmp = subtreeRoot.key.compareTo(toRemove.key);
    if (cmp == 0) {
      return remove(subtreeRoot);
    } else if (cmp > 0) {
      subtreeRoot.left = remove(subtreeRoot.left, toRemove);
    } else {
      subtreeRoot.right = remove(subtreeRoot.right, toRemove);
    }

    subtreeRoot.height = 1 + max(height(subtreeRoot.left), height(subtreeRoot.right));

    return balancer(subtreeRoot);
  }

  private Node<K, V> remove(Node<K, V> node) { //based off BST Map but has to check BF
    // Easy if the node has 0 or 1 child.
    if (node.right == null) {
      return node.left;
    } else if (node.left == null) {
      return node.right;
    }

    // If it has two children, find the predecessor (max in left subtree),
    Node<K, V> toReplaceWith = max(node);
    // then copy its data to the given node (value change),
    node.key = toReplaceWith.key;
    node.value = toReplaceWith.value;
    // then remove the predecessor node (structural change).
    node.left = remove(node.left, toReplaceWith);

    node.height = 1 + max(height(node.left), height(node.right));

    return balancer(node); //checks balance factor and chooses appriopiate rotation if needed
  }

  //everything else is the same as in BST Map except for some minor changes
  @Override
  public void put(K k, V v) throws IllegalArgumentException {
    Node<K, V> n = findForSure(k);
    n.value = v;
  }

  @Override
  public V get(K k) throws IllegalArgumentException {
    Node<K, V> n = findForSure(k);
    return n.value;
  }

  @Override
  public boolean has(K k) {
    if (k == null) {
      return false;
    }
    return find(root, k) != null;
  }

  private Node<K, V> find(Node<K, V> node, K k) {
    if (k == null) {
      throw new IllegalArgumentException("cannot handle null key");
    }
    if (node != null) {
      int cmp = k.compareTo(node.key);
      if (cmp == 0) {
        return node;
      } else if (cmp > 0) {
        return find(node.right, k);
      } else {
        return find(node.left, k);
      }
    }
    return null;
  }

  private Node<K, V> findForSure(K k) {
    Node<K, V> n = find(root, k);
    if (n == null) {
      throw new IllegalArgumentException("cannot find key " + k);
    }
    return n;
  }

  @Override
  public int size() {
    return size;
  }

  @Override
  public Iterator<K> iterator() {
    return new InOrderIterator();
  }

  private class InOrderIterator implements Iterator<K> {
    private final Stack<Node<K, V>> stack;

    InOrderIterator() {
      stack = new Stack<>();
      pushLeft(root);
    }

    private void pushLeft(Node<K, V> curr) {
      while (curr != null) {
        stack.push(curr);
        curr = curr.left;
      }
    }

    @Override
    public boolean hasNext() {
      return !stack.isEmpty();
    }

    @Override
    public K next() {
      Node<K, V> top = stack.pop();
      pushLeft(top.right);
      return top.key;
    }
  }

  private Node<K, V> rotateRight(Node<K,V> node) {
    Node<K, V> leftChild = node.left;
    node.left = leftChild.right;
    leftChild.right = node;
    node.height = 1 + max(height(node.left), height(node.right));
    leftChild.height = 1 + max(height(leftChild.left), height(leftChild.right));
    return leftChild;
  }

  private Node<K, V> rotateLeft(Node<K,V> node) {
    Node<K, V> rightChild = node.right;
    node.right = rightChild.left;
    rightChild.left = node;
    node.height = 1 + max(height(node.left), height(node.right));
    rightChild.height = 1 + max(height(rightChild.left), height(rightChild.right));
    return rightChild;
  }

  private Node<K, V> rotateLeftRight(Node<K,V> node) {
    node.left = rotateLeft(node.left);
    return rotateRight(node);
  }

  private Node<K, V> rotateRightLeft(Node<K,V> node) {
    node.right = rotateRight(node.right);
    return rotateLeft(node);
  }

  private Node<K, V> balancer(Node<K, V> node) { //chooses correct rotation if needed also just returns back og node
    int balanceFactor = balanceFactor(node);

    if (balanceFactor > 1 && balanceFactor(node.left) >= 0) {
      return rotateRight(node);
    } else if (balanceFactor > 1 && balanceFactor(node.left) < 0) {
      return rotateLeftRight(node);
    } else if (balanceFactor < -1 && balanceFactor(node.right) <= 0) {
      return rotateLeft(node);
    } else if (balanceFactor < -1 && balanceFactor(node.right) > 0) {
      return rotateRightLeft(node);
    }
    return node;
  }

  private int balanceFactor(Node<K, V> node) {
    if (node == null) {
      return 0;
    }
    return height(node.left) - height(node.right);
  }

  private int height(Node<K, V> node) {
    if (node == null) {
      return -1;
    }
    return 1 + max(height(node.left), height(node.right));
  }

  int max(int a, int b) {
    return Math.max(a, b);
  }

  private Node<K, V> max(Node<K, V> node) {
    Node<K, V> curr = node.left;
    while (curr.right != null) {
      curr = curr.right;
    }
    return curr;
  }

  /*** Do not change this function's name or modify its code. ***/
  @Override
  public String toString() {
    return BinaryTreePrinter.printBinaryTree(root);
  }

  /**
   * Feel free to add whatever you want to the Node class (e.g. new fields).
   * Just avoid changing any existing names, deleting any existing variables,
   * or modifying the overriding methods.
   *
   * <p>Inner node class, each holds a key (which is what we sort the
   * BST by) as well as a value. We don't need a parent pointer as
   * long as we use recursive insert/remove helpers.</p>
   **/
  private static class Node<K, V> implements BinaryTreeNode {
    Node<K, V> left;
    Node<K, V> right;
    K key;
    V value;
    int height;

    // Constructor to make node creation easier to read.
    Node(K k, V v) {
      // left and right default to null
      key = k;
      value = v;
    }

    @Override
    public String toString() {
      return key + ":" + value;
    }

    @Override
    public BinaryTreeNode getLeftChild() {
      return left;
    }

    @Override
    public BinaryTreeNode getRightChild() {
      return right;
    }
  }

}
