# TwoPassAssembler

### Structure of an instruction :
* A typical Instruction will consist of a label, an opcode and an operand along with comment out of which comment and label is optional.
  <p align = "center" > [Label] Opcode Operand [Comment] <p>
* Labels must have the first character as an alphabet and the rest can be alphanumeric, succeeded with tilde(`~`) with no space between label name and ‘~’. 
* Opcode must also only consist of alphabets.
* Operand must have the first character as an alphabet and the rest can be alphanumeric.
* Comments are optional, but if added they must start with a backslash. ‘//’
* Each of these (label, opcode, operand, comment) must be separated using space(s).
* The leading and trailing space before and after instruction doesn’t matter.
  
  <hr>
  
### Types of Opcodes :
* Type 1: Opcode requiring 1 argument
* Type 0: Opcode requiring 0 argument

  <hr>
  
### Errors reported :
1. If invalid opcode is used.
2. If invalid no arguments are given to the opcode.
3. If the starting of the symbol (label and variable) doesn’t contain an alphabet.
4. If invalid character is detected in a symbol.
5. If no label is found before tilde(~), as in the label name is empty.
6. If a variable is used as a label.
7. If a label is redeclared.
8. If invalid jump location is given (variable name used as a jump location).
9. If a symbol is used but not defined.
10. If opcode is used as a symbol name.
11. If the end statement is missing.
12. If the location counter exceeds 8 bits.
13. If the label body is empty.
