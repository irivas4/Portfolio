# Discussion

## Unit testing TreapMap

For me the main difficulty came from changing my implementation of remove from the AVLTreeMap, 
because for an AVLTreeMap when youre removing a node with two children you can simply bring up 
the maximum value of the left subtree and the Tree remains sorted. On the other hand however 
for a TREAP you have find the child with the highest priority but after you bring it up through 
a rotation you have to fix the subtreap as it might not be correctly “sorted” anymore. One then 
has to iterate through the treap either until your at a leaf or until there only one child and 
do right or left rotations to fix anything not in its correct place. A worst case would be is 
you have to fix the tree because you want to remove the root of a perfect binary tree. A bad 
case is just when im trying to remove something with two children in which case I also have to 
do a combination of rotations until its either a node or until i get to only have one child in 
which case i can connect its parent to its child and it’ll also work. For example below I wanted 
to remove 50 but first i had to do a left rotation because its right child had higher priority 
and then i had to a right and then left rotation to get it to be a leaf to actually remove it. 
Because this has to be done multiple times in different cases I felt it hardest to implement 
but was able to do it by send nodes back and forth recrusilvely between two methods that did 
rotations and removals as necessary depending on the amount of children the node has.

Priority in parentheses
      50(8)   
     /  \
  30(4) 70 (7)
       / \  
    60(3) 80(6)

      70(7)
       /  \
    50(8) 80(6)
     / \    
 30(4) 60(3) 

      70(7)
       /  \
    30(4) 80(6)
       \    
        50(8)
         \
         60(3)

      70(7)
       /  \
    30(4) 80(6)
       \    
        50(8)
         /
      60(3)

      70(7)
       /  \
    30(4) 80(6)
       \    
        60(3)

## Benchmarking

From changing out the files that are being read from, I noticed that the bstMap implementation was always 
the most efficient but closely followed by the treapMap implementation. Next theres a gap between the two 
and avlTreeMap/arrayMap with both coming in at third and fourth for different txt files. It makes since why 
iterating through an arrayMap is more costly around O(n) compared to trees which will have a cost of O(lgn) 
for the most part. I wonder why the cost of avlTree isnt less since it is a tree and it self balances and 
all the balancing operations are O(1), but this may be because it has to do so many rotations or even 
rotations for every insertion basically. This makes sense because in the bigger files is when it started 
taking a lot more time compared to other implementations, because for the smaller files there want that big 
of a gap. The reason why bstMap might be the best is because we know chances are that if given random things 
to input to the map that theyre more likely to have a height of O(lgn) than be completely degenerated. 
Therefore its likely that its methods dont cost too much. Adding onto that nodes in th BSt are a lot simpler 
and theres no rtoations or such that require time to deal with possibly making it the most efficient.