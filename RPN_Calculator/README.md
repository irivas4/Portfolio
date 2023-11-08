# Discussion

**Document all error conditions you determined and why they are error
 conditions. Do this by including the inputs that you used to test your
  program and what error conditions they exposed:**

The main error condition I encountered was when the user tried to perform an 
operation with only one or zero operands in the stack for example, the program 
would start and the user would enter 10 * or simply /. My operations method 
tries to pull the last two operands in order to execute the operation but 
an EmptyException is thrown from the top method of LinkedStack when I go to 
grab the two operands and there’s insufficient. This has to be dealt with 
gracefully, which I do using a try catch block. Additionally, when the user 
enters some input that would divide or modulo by zero,  for example, the user 
enters 12 0 / an ArithmeticException is thrown, this also has to be dealt with 
gracefully. I didn’t choose to use a try catch block in order to be more space 
efficient. Finally, when I try to use the . operation when there's nothing in the 
stack an EmptyException this almost must be handled gracefully which i did through 
a catch block.The main error condition I encountered was when the user tried to 
perform an operation with only one or zero operands in the stack for example, the 
program would start and the user would enter 10 * or simply /. My operations method 
tries to pull the last two operands in order to execute the operation but an 
EmptyException is thrown from the top method of LinkedStack when I go to grab the two 
operands and there’s insufficient. This has to be dealt with gracefully, which I do 
using a try catch block. Additionally, when the user enters some input that would 
divide or modulo by zero,  for example, the user enters 12 0 / an ArithmeticException 
is thrown, this also has to be dealt with gracefully. I didn’t choose to use a try 
catch block in order to be more space efficient. Finally, when I try to use the . 
operation when there's nothing in the stack an EmptyException this almost must be 
handled gracefully which i did through a catch block.
