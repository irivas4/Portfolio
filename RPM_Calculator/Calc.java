package hw4;


import exceptions.EmptyException;
import java.util.Scanner;

/**
 * A program for an RPN calculator that uses a stack.
 */
public final class Calc {

  private static void decide(LinkedStack<Integer> stack, char input) {
    switch (input) {
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
        operations(stack, input);
        break;
      case '.':
        showTop(stack);
        break;
      case '?':
        System.out.println(stack.toString());
        break;
      default:
        System.out.println("Error: bad token");
        break;
    }
  }

  private static void showTop(LinkedStack<Integer> stack) {
    try {
      System.out.println(stack.top());
    } catch (EmptyException ex) {
      System.out.println("Error: nothing in the stack");
    }
  }

  private static int calculate(LinkedStack<Integer> stack, int first, int sec, char inp) {
    if (inp == '+') {
      first += sec;
    } else if (inp == '-') {
      first -= sec;
    } else if (inp == '/' || inp == '%') {
      if (sec == 0) {
        System.out.println("Error: you can not divide or modulo by 0");
        stack.push(first);
        return sec;
      } else if (inp == '/') {
        first /= sec;
      } else {
        first %= sec;
      }
    } else if (inp == '*') {
      first *= sec;
    }
    return first;
  }

  private static void operations(LinkedStack<Integer> stack, char inp) {
    LinkedStack<Integer> temp = new LinkedStack<>();
    for (int i = 0; i < 2; i++) {
      try {
        temp.push(stack.top());
      } catch (EmptyException ex) {
        System.out.println("Error: not enough operands in stack");
        if (!(temp.empty())) {
          stack.push(temp.top());
        }
        return;
      }
      stack.pop();
    }
    int result = temp.top();
    temp.pop();
    result = calculate(stack, result, temp.top(), inp);
    stack.push(result);
  }

  /**
   * The main function.
   *
   * @param args Not used.
   */
  public static void main(String[] args) {
    LinkedStack<Integer> stack = new LinkedStack<>();
    Scanner scan = new Scanner(System.in);
    char input = '0';
    int operand;
    do {
      if (scan.hasNextInt()) {
        operand = scan.nextInt();
        stack.push(operand);
      } else if (scan.hasNext()) {
        input = scan.next().charAt(0);
        if (input != '!') {
          decide(stack, input);
        }
      } else {
        System.out.println("Error: bad token");
      }
    } while (input != '!');
  }
}
