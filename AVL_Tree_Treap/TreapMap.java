package hw6.bst;

import hw6.OrderedMap;
import java.util.Iterator;
import java.util.Random;
import java.util.Stack;

/**
 * Map implemented as a Treap.
 *
 * @param <K> Type for keys.
 * @param <V> Type for values.
 */
public class TreapMap<K extends Comparable<K>, V> implements OrderedMap<K, V> {

  /*** Do not change variable name of 'rand'. ***/
  private static Random rand;
  /*** Do not change variable name of 'root'. ***/
  private Node<K, V> root;

  private int size;

  /**
   * Make a TreapMap.
   */

  public TreapMap() {
    rand = new Random();
    root = null;
    size = 0;
  }

  /**
   * Make a TreapMap for tessting based off a seed value passed in.
   * @param seed the seed value to be passed into the instance of random created below.
   */
  public TreapMap(long seed) {
    rand = new Random(seed);
    root = null;
    size = 0;
  }

  @Override
  public void insert(K k, V v) throws IllegalArgumentException {
    if (k == null) {
      throw new IllegalArgumentException("cannot handle null key");
    }
    root = insert(root, k, v);
    size++;
  }

  private Node<K, V> insert(Node<K, V> n, K k, V v) { //super similar to avl implementation but takes into account
    if (n == null) { //priorities and bubbling up
      return new Node<>(k, v);
    }
    int cmp = k.compareTo(n.key);
    if (cmp < 0) {
      n.left = insert(n.left, k, v);
      if (n.priority > n.left.priority) { //levels of priority are now whats most important
        n = rotateRight(n);
      }
    } else if (cmp > 0) {
      n.right = insert(n.right, k, v);
      if (n.priority > n.right.priority) {
        n = rotateLeft(n);
      }
    } else {
      throw new IllegalArgumentException("duplicate key " + k);
    }
    return n;
  }


  @Override
  public V remove(K k) throws IllegalArgumentException {
    Node<K, V> node = findForSure(k);
    V value = node.value;
    root = remove(root, node.key);
    size--;
    return value;
  }

  private Node<K, V> remove(Node<K, V> node, K key) {
    if (node == null) {
      return null;
    }
    int cmp = key.compareTo(node.key);
    if (cmp < 0) {
      node.left = remove(node.left, key);
    } else if (cmp > 0) {
      node.right = remove(node.right, key);
    } else {
      node.priority = 2147483647; //so that it bubbles all the way down if neccessary
      node = worstCaseRemove(node);
    }
    return node;
  }

  private Node<K, V> worstCaseRemove(Node<K, V> subtreeRoot) { //worstcase is encontering something i want to remove
    Node<K, V> bestChild; //a root or subtree root that is perfect binary tree i have to constantly fix it while
    if (subtreeRoot.left == null && subtreeRoot.right == null) { //moving up the child with "higher" priority
      return null;
    } else {
      bestChild = quickRemove(subtreeRoot);
    }
    // i have to check priority
    if (bestChild.priority <= subtreeRoot.priority) { // has to be < or = just in case random gen generates biggest int
      if (bestChild == subtreeRoot.left) {
        subtreeRoot = rotateRight(subtreeRoot);
        subtreeRoot.right = worstCaseRemove(subtreeRoot.right);
      } else {
        subtreeRoot = rotateLeft(subtreeRoot);
        subtreeRoot.left = worstCaseRemove(subtreeRoot.left);
      }
    } else {
      return null;
    }
    return subtreeRoot;
  }

  private Node<K, V> quickRemove(Node<K, V> subtreeRoot) { //if possible i want to replace with a node that has
    Node<K, V> bestChild; //only one child as sooon as possible but if they keep having two children i have to keep
    if (subtreeRoot.left == null) { //fixing it so it remains in order
      bestChild = subtreeRoot.right;
    } else if (subtreeRoot.right == null) {
      bestChild = subtreeRoot.left;
    } else {
      if (subtreeRoot.right.priority <= subtreeRoot.left.priority) {
        bestChild = subtreeRoot.right;
      } else {
        bestChild = subtreeRoot.left;
      }
    }
    return bestChild;
  }

  //I can really reuse all other methods as they should work the same as in a BST map so thats what i did

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

  /*** Do not change this function's name or modify its code. ***/
  @Override
  public String toString() {
    return BinaryTreePrinter.printBinaryTree(root);
  }

  private Node<K, V> rotateRight(Node<K,V> node) {
    Node<K, V> leftChild = node.left;
    node.left = leftChild.right;
    leftChild.right = node;
    return leftChild;
  }

  private Node<K, V> rotateLeft(Node<K,V> node) {
    Node<K, V> rightChild = node.right;
    node.right = rightChild.left;
    rightChild.left = node;
    return rightChild;
  }


  /**
   * Feel free to add whatever you want to the Node class (e.g. new fields).
   * Just avoid changing any existing names, deleting any existing variables,
   * or modifying the overriding methods.
   * Inner node class, each holds a key (which is what we sort the
   * BST by) as well as a value. We don't need a parent pointer as
   * long as we use recursive insert/remove helpers. Since this is
   * a node class for a Treap we also include a priority field.
   **/
  private static class Node<K, V> implements BinaryTreeNode {
    Node<K, V> left;
    Node<K, V> right;
    K key;
    V value;
    int priority;

    // Constructor to make node creation easier to read.
    Node(K k, V v) {
      // left and right default to null
      key = k;
      value = v;
      priority = generateRandomInteger();
    }

    // Use this function to generate random values
    // to use as node priorities as you insert new
    // nodes into your TreapMap.
    private int generateRandomInteger() {
      // Note: do not change this function!
      return rand.nextInt();
    }

    @Override
    public String toString() {
      return key + ":" + value + ":" + priority;
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
}
