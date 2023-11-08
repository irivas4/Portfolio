package hw6;


import hw6.bst.TreapMap;
import org.junit.jupiter.api.Test;
import java.util.Random;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * In addition to the tests in BinarySearchTreeMapTest (and in OrderedMapTest & MapTest),
 * we add tests specific to Treap.
 */
@SuppressWarnings("All")
public class TreapMapTest extends BinarySearchTreeMapTest {

  @Override
  protected Map<String, String> createMap() {
    return new TreapMap<>(3);
  }


  @Test
  public void findOutRandomNumbers() {
    Random random1 = new Random(3);
    for (int i = 0; i < 10; i++) {
      System.out.print(random1.nextInt() + ", ");
    }
    System.out.println();
    assertTrue(0 == 0);
  }
  //Needed to figure out the sequence of
  //Sequence: -1157793070, 1913984760, 1107254586, 1773446580, 254270492
  //Sequence Ordered: 1913984760, 1773446580, 1107254586, 254270492, -11577930706
  //Sequence: 2, 4, 3, 5, 1

  @Test
  public void testEmptyTreap() {
    assertEquals(0, map.size());

  }

  @Test
  public void testInsertNode() {
    map.insert("A", "1");
    assertEquals(1, map.size());
    assertEquals("1", map.get("A"));
  }

  @Test
  public void testAddMultipleNodes() {
    map.insert("A", "1");
    map.insert("B", "2");
    map.insert("C", "3");
    assertEquals(3, map.size());
  }

  @Test
  public void testPut() {
    map.insert("A", "1");
    map.put("A", "2");
    assertEquals(1, map.size());
    assertEquals("2", map.get("A"));
  }

  @Test
  public void testPutMultiple() {
    map.insert("A", "1");
    map.insert("B", "2");
    map.insert("C", "3");
    map.put("C", "4");
    assertEquals(3, map.size());
    assertEquals("4", map.get("C"));
  }

  @Test
  public void testRemoveOnlyOneNode() {
    map.insert("A", "1");
    map.remove("A");
    assertEquals(0, map.size());
  }

  @Test
  public void testRemoveMultipleNodes() {
    map.insert("A", "1");
    map.insert("B", "2");
    map.insert("C", "3");
    map.insert("D", "4");
    map.insert("E", "5");
    map.remove("A");
    map.remove("D");
    assertEquals(3, map.size());
  }

  @Test
  public void testLeftRotation() {
    map.insert("A", "1");
    map.insert("B", "2");
    map.insert("C", "3");
    map.insert("D", "4");
    map.insert("E", "5");

    String expected = "B:2:-1879439976\nA:1:-1155099828 D:4:-836442134\n" +
            "null null C:3:304908421 E:5:288278256\n";
    assertEquals(expected, map.toString());
  }

  @Test
  public void insertNoRotation() {
    map.insert("A", "1");
    map.insert("D", "4");
    map.insert("E", "5");
    map.insert("B", "2");
    map.insert("C", "3");

    String expected = "D:4:-1879439976\nA:1:-1155099828 E:5:304908421\n" +
            "null B:2:-836442134 null null\nnull null null C:3:288278256 null null null null\n";

    assertEquals(expected, map.toString());
  }

  @Test
  public void removeNoRotation() {
    map.insert("A", "1");
    map.insert("D", "4");
    map.insert("E", "5");
    map.insert("B", "2");
    map.insert("C", "3");

    map.remove("C");
    String expected = "D:4:-1879439976\nA:1:-1155099828 E:5:304908421\n" +
            "null B:2:-836442134 null null\n";
    assertEquals(expected, map.toString());
  }

  @Test
  public void insertRotateLeftLeftNull() {
    map.insert("A", "1");
    map.insert("B", "2");

    String expected = "B:2:-1879439976\nA:1:-1155099828 null\n";
    assertEquals(expected, map.toString());

  }

  @Test
  public void insertRotateRightRightNull() {
    map.insert("B", "2");
    map.insert("A", "1");

    String expected = "A:1:-1879439976\nnull B:2:-1155099828\n";
    assertEquals(expected, map.toString());
  }

  @Test
  public void insertRotateLeftLeftValue() {
    map.insert("A", "1");
    map.insert("B", "2");
    map.insert("C", "3");
    map.insert("D", "4");
    map.insert("E", "5");
    map.insert("F", "6");
    String expected = "B:2:-1879439976\nA:1:-1155099828 F:6:-1795872892\nnull null D:4:-836442134 null\n" +
            "null null null null C:3:304908421 E:5:288278256 null null\n";

    assertEquals(expected, map.toString());
  }

  @Test
  public void insertRotateRightRightValue() {
    map.insert("F", "6");
    map.insert("E", "5"); // this will produce a right rotation
    map.insert("D", "4");
    map.insert("C", "3"); //this will produce a right rotation
    map.insert("B", "2");
    map.insert("A", "1");

    String expected = "E:5:-1879439976\nA:1:-1795872892 F:6:-1155099828\nnull C:3:-836442134 null null\n" +
            "null null B:2:288278256 D:4:304908421 null null null null\n";

    assertEquals(expected, map.toString());
  }

  @Test
  public void removeRotateRight() {
    map.insert("A", "1");
    map.insert("B", "2");
    map.remove("B");

    String expected = "A:1:-1155099828\n";
    assertEquals(expected, map.toString());
  }

  @Test
  public void removeRotateLeft() {
    map.insert("B", "2");
    map.insert("A", "1");
    map.remove("A");

    String expected = "B:2:-1155099828\n";
    assertEquals(expected, map.toString());
  }

  @Test
  public void removeRoot() {
    map.insert("A", "1");
    map.insert("B", "2");
    map.insert("C", "3");
    map.insert("D", "4");
    map.insert("E", "5");
    map.insert("F", "6");
    map.remove("B");

    String expected = "F:6:-1795872892\nA:1:-1155099828 null\n" +
            "null D:4:-836442134 null null\nnull null C:3:304908421 E:5:288278256 null null null null\n";
    assertEquals(expected, map.toString());

  }
}