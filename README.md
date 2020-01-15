# <center> Minimal-Basic-Interpreter</center>

<center> Shanghai Jiao Tong University </center>

<center>ITAMI SHO</center>

## 0. Introduction

​	In 1975, Bill Gates and Paul Allen started the company that would become Microsoft by writing
a BASIC interpreter for the ﬁrst microcomputer, the Altair 8800 developed by the MITS corporation of Albuquerque, New Mexico. By making it possible for users to write programs for a microcomputer without having to code in machine language, the Altair and its implementation of BASIC helped to start the personal computer revolution.

## 1. Purpose

In this project, your mission is to build a minimal BASIC interpreter. You need to accomplish the following objectives: 

* To increase your familiarity with expression trees and class inheritance. 
* To give you a better sense of how programming languages work. Learning how an interpreter operates—particularly one that you build yourself—provides useful insights into the programming process. 
* To oﬀer you the chance to adapt an existing program into one that solves a diﬀerent but related task. The majority of programming that people do in the industry consists of modifying existing systems rather than creating them from scratch. 

## 2.Function

Statements implemented in the minimal version of BASIC
REM: This statement is used for comments. Any text on the line after the keyword REM is ignored. 
**LET**:  This statement is BASIC’s assignment statement. The LET keyword is followed by a variable name, an equal sign, and an expression. As in C++, the eﬀect of this statement is to assign the value of the expression to the variable, replacing any previous value. In BASIC, assignment is not an operator and may not be nested inside other expressions.
**PRINT**:  In minimal BASIC, the PRINT statement has the form: PRINT exp where exp is an expression. The eﬀect of this statement is to print the value of the expression on the console and then print a newline character so that the output from the next PRINT statement begins on a new line.
**INPUT**:  In the minimal version of the BASIC interpreter, the INPUT statement has the form: INPUT var where var is a variable read in from the user. The eﬀect of this statement is to print a prompt consisting of the string " ? " and then to read in a value to be stored in the variable.



**Commands to control the BASIC interpreter**
**RUN**:  This command starts program execution beginning at the lowest-numbered line. Unless the ﬂow is changed by GOTO and IF commands, statements are executed in line-number order. Execution ends when the program hits the END statement or continues past the last statement in the program. 
**LIST**:  This command lists the steps in the program in numerical sequence.
**CLEAR**: This command deletes the program so the user can start entering a new one.
**HELP**:  This command provides a simple help message describing your interpreter.
**QUIT**:  Typing QUIT exits from the BASIC interpreter by calling exit(0).

## 3.Contents

main.cpp
basicWindow.cpp
basicWindow.h
program.h
program.cpp
console.h
console.cpp
statement.h
statement.cpp
exp.h
exp.cpp
evalstate.h
evalstate.cpp



## 4. Outline design

* Flow chart



## 5. Function Implementation

basicWindow:定义basicWindow类，用于构造program类和console类; 主要用于前端UI, connect 各个函数
console:定义console类, 用于在前端接受用户指令，以及反馈
program:定义program类，包含statement类; 主要用于解析用户的每一条指令，存储所有命令，将处理的信息返回给console
statement: 定义statement类, 包含Expression类和Evalstate类; 用于分类不同指令，将合法的expression变成后缀式并组合后返回
expression: 定义expression类, ConstantExp类, CompondExp类, IdentiﬁerExp类,
evalstate: 定义evalstate类, 用于存储变量以及变量值
