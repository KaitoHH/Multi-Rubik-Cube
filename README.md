# Multi Rubik Cube
a Rubki Cube varing from Pocket Cube to V-Cube 7 implemented by C++ with OpenGL support

***
### Usage
- Mouse left button to change perspective
- Mouse move Wheel to zoom in/out
- F1/F2 - increase/decrease dimension of cube
- F5 - load instructions from file "in.txt"
- 1-3 - try diferent rubki stripe
- ESC - stop rotate or reset state
- SPACE - have a random rotate
- ENTER - have 100 random rotate

### How to edit in.txt
this file can contain several rotate instructions, each single instructions are splited by at least one space.
Here is some notes about how to create a single instruction.
- Support that blue is the front ,yellow is the top, and red is the right plane.
- a single instruction is consisted of at most 3 parts.

**fisrt part**

the first part is a single number indictating the offest ,if the offest is zero, you can just skip it.

**second part**

the second part is a signle character,which may be one of F B L R U D , namely front, behind, left, right ,up and down, meaning which plane do you want to rotate. By combing the first part, we can locate certain rotation plane. For example, "2R" means the second plane counting from right plane,while "L", equals "0L", just means the first left plane since the default offest is zero.

**last part**

the last part is one of three character: single quote,character 2,or just nothing, meaning rotation direction: anticlockwise, double rotate or clockwise.

###Example of in.txt
For example,"3R'" means anticlockwise rotate the third plane counting from the right plane. "U" means clockwise rotate the top plane. "5D" means clockwise rotate the fifth plane counting from the down plane.

Here is some example of "in.txt":
- F B L R F B L R F B L R
- U' D F' B L R' U' D
- R2 F2 B2 L2 U F2 R2 L2 B2 D'
- 3R F2 6B' 1L2 0U F2 R2 L2 B2 D'

**NOTE** that the offest MUST BE LESS than the current cube's level.

You will be a bit familiar with it if you have known some equation about cube:)

***

### Complie Environment
Visual Studio 2015 with OpenGL support.

###Bugs:
- To be reported.
- Please contact me or pull it as a request if you find any bugs, thank you:)

###Screenshot:
![image](https://github.com/KaitoHH/Multi-Rubik-Cube/blob/master/screenshot01.png)
![image](https://github.com/KaitoHH/Multi-Rubik-Cube/blob/master/screenshot02.png)
![image](https://github.com/KaitoHH/Multi-Rubik-Cube/blob/master/screenshot03.png)