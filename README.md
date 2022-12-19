
# Cellular Automata
## Overview
This program simulates Celular Automata. It uses graphical animation to demonstrate a glider and an exploision pattern

## Built with: 
Visual Studio, C++

## Prerequisites: 

g++ compiler

## Usage: 

.\compile-run.sh     -           To compile and launch the webserver

The default configuration is the Glider pattern, if you want to see
the explosion pattern go the constructor of the CAAnimate object and change
the line from 'initialState("Glider")' to 'initialeState("Explosion")'


## Getting Started: 

As you run the program, a web server will be launched and you can access the graphical animation by going to http://127.0.0.1:8000

You can click the step button to see a step by step change of live and dead cells or You can click the start button to keep seeing the changes in the cells.

## Credit

The update rule implemented in the update() method is from the book
"Complexity" by Melanie Mitchell.

The initial state for the glider pattern is adopted from Prof. Anya Vostinar's example

The initial state for the explosion pattern is taken from teohrt at https://github.com/teohrt/Cellular-Automata/blob/master/explosion.txt 

the sh script was adopted from Prof. Anya Vostinar
  

 



  




