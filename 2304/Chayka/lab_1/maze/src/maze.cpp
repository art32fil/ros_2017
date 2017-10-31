#include "maze/maze.h"
#include <iostream>
#include <cstdlib>
char Maze::a[][N]={
                                {'~','~','~','~','~','~','~','~','~','~'},
                                {'|',' ','|',' ',' ',' ',' ',' ',' ','|'},
                                {'|',' ','|',' ','~','~','~','~',' ','|'},
                                {'|',' ','|',' ',' ',' ','|',' ',' ','|'},
                                {'|',' ','|','~','~',' ','|',' ','~','|'},
                                {'|',' ',' ',' ','|',' ','|',' ',' ','|'},
                                {'|',' ','|',' ','|',' ','|','|',' ','|'},
                                {'|',' ','|',' ','|',' ','|','|',' ','|'},
                                {'|',' ','|',' ',' ',' ','|','|','X','|'},
                                {'~','~','~','~','~','~','~','~','~','~'}
                               };

char Maze::cat[][N]={
                                {' ',' ','/','\\','_','_','_','/','\\',' '},
                                {' ','(',' ',' ','o',' ','o',' ',' ',')'},
                                {' ','/',' ',' ',' ','*',' ',' ',' ','\\'},
                                {' ','\\','_','_','\\','_','/','_','_','/'},
                                {' ',' ',' ','/',' ',' ',' ','\\',' ',' '},
                                {' ',' ','/',' ','_','_','_',' ','\\',' '},
                                {' ',' ','\\','/','_','_','_','\\','/',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ','m','e','o','w','!',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
                               };



Maze::Maze():Sx(1),Sy(1){

}

void Maze::draw(){
  std::system("clear");
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
      if(i!=Sx || j!=Sy)
			  std::cout<<a[i][j]<<" ";
      else
        std::cout<<"& ";
    }
		std::cout<<std::endl;
	}
}

void Maze::drawFin(){
  std::system("clear");
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			  std::cout<<cat[i][j];
    }
		std::cout<<std::endl;
	}
}

void Maze::fin(){
  Sx = Sy = 1;
  drawFin();
}

bool Maze::moveLeft(){
	if(a[Sx][Sy-1] == ' '){
		Sy--;
    draw();
		return true;
	}
  if(a[Sx][Sy-1] == 'X'){
    fin();
    return true;
  }
	return false;
}

bool Maze::moveUp(){
	if(a[Sx-1][Sy] == ' '){
		Sx--;
    draw();
		return true;
	}
  if(a[Sx-1][Sy] == 'X'){
    fin();
    return true;
  }
	return false;
}

bool Maze::moveDown(){
	if(a[Sx+1][Sy] == ' '){
		Sx++;
    draw();
		return true;
	}
  if(a[Sx+1][Sy] == 'X'){
    fin();
    return true;
  }
	return false;
}

bool Maze::moveRight(){
	if(a[Sx][Sy+1] == ' '){
		Sy++;
    draw();
		return true;
	}
  if(a[Sx][Sy+1] == 'X'){
    fin();
    return true;
  }
	return false;
}
