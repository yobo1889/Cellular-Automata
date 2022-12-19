#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

using namespace std;

emp::web::Document doc{"target"};


class CAAnimator : public emp::web::Animate { 
    
    // creates a canvas
    emp::web::Canvas canvas{700, 500, "canvas"};

    std::vector<std::vector<float>> cells; //a vector of vector that represetns each cube in our matrix
    
    float dead = 0.5; //color value for a dead cell
    float alive = 0.2; //color value for a live cell

    

    
    public:

        CAAnimator() {
            doc << canvas;
            doc << GetToggleButton("Toggle"); //creates a start button
            doc << GetStepButton("Step"); //creates a step buttton
            cells.resize(15, std::vector<float>(10, dead)); //sets the dimension and value of each cell
            initialState("Glider");
        
            


        }
 
    //checks whether the cell is a corner cell
    bool isCorner(int x, int y){
        if((x==0&&y==0)||(x==0&&y==9)||(x==14&&y==0)||(x==14&y==9)){
            return true;
        }
        return false;
     }
    //checks whether the cell is along the edge of the matrix
     bool isEdge(int x, int y){
        int cell [30][30];
        std::fill(cell[0], cell[0] + 30 * 30, 0);
        for(int i = 0;i<15;i++){
            cell[14][i] = 1;
            cell[i][0] = 1;
            cell[i][9] = 1;
        } 
        for(int j = 0;j<10;j++){
            cell[0][j] = 1;
        } 
        if(cell[x][y] == 1){
            return true;
        }
        return false;
     }
    // checks whether the specified cell coordinate is within our matrix
     bool cellExists(int x, int y){
        int cell [30][30];
        std::fill(cell[0], cell[0] + 30 * 30, 0);
        for(int i = 0;i<15;i++){
         for(int j = 0;j<10;j++){
            cell[i][j] = 1;
         }
        } 
        if(cell[x][y] == 1){
            return true;
        }
        return false;
     }
    //a struct object which contains the x and y coordinates of every neighbour of a cell
     struct Neighbors{
        int x;
        int y;
     };
     
     Neighbors neighbor[10]; //an array that stores about 10 neighbors of a cell(we only eight tho)

     //populates the above Neighbors struct with the neighbor of cells[x][y]
     void findNeighbor(int x, int y){
        for(int i = 0;i<10;i++){
                neighbor[i].x = 100;
                neighbor[i].y = 100;
        }
        int n = 0;
        for(int k = -1;k<2;k++){
          for(int j = -1;j<2;j++){
           if(!(k==0&&j==0)){
            if(cellExists(x+(1*k),y+(1*j))){
                neighbor[n].x = x+(1*k);
                neighbor[n].y = y+(1*j);
                n++;
            }
           }
          }
        }
        if (isEdge(x,y)){
            
            if (x == 0){
                for(int i = -1;i<2;i++){
                  neighbor[n].x = 14;
                  neighbor[n].y = y-i;
                  n++; 
                }
            }
            if (x == 14){
                for(int i = -1;i<2;i++){
                  neighbor[n].x = 0;
                  neighbor[n].y = y-i;
                  n++; 
                }
            }
            if (y == 0){
                for(int i = -1;i<2;i++){
                  neighbor[n].x = x-i;
                  neighbor[n].y = 9;
                  n++; 
                }
            }
            if (y == 9){
                for(int i = -1;i<2;i++){
                  neighbor[n].x = x-i;
                  neighbor[n].y = 0;
                  n++; 
                }
            }
            if(isCorner(x,y)){
                if((x==0)&(y==0)){
                    neighbor[n].x = 14;
                    neighbor[n].y = 9;
                    n++; 
                }
                if((x==14)&(y==0)){
                    neighbor[n].x = 0;
                    neighbor[n].y = 9;
                    n++;
                }
                if((x==0)&(y==9)){  
                    neighbor[n].x = 14;
                    neighbor[n].y = 0;
                    n++; 
                }
                if((x==14)&(y==9)){
                    neighbor[n].x = 0;
                    neighbor[n].y = 0;
                    n++;
                }
                
            }
        }
    }
        //returns the count of live neighbors by seeing their value(alive if alive, dead if dead)
        int countLiveNeighbor(std::vector<std::vector<float>> oldCells, int x, int y){
            int liveNeighbors = 0;
            float shade = alive;
            findNeighbor(x,y);
            for(int i = 0;i<10;i++){
             if(cellExists(neighbor[i].x,neighbor[i].y)){
                if(oldCells[neighbor[i].x][neighbor[i].y]==shade){
                    liveNeighbors++;
                }
             }
            }
            return liveNeighbors;
        }

        //flushes our cells vector with its initial value of dead
        void reset(){
            for (int x = 0; x < cells.size();x++ ){
              for (int y = 0; y < cells[x].size();y++){
                cells[x][y] = dead;
              }
            }
        }

        //sets the initial live cells for a stable glider
        void initialState(string type){
            if(type=="Glider"){
                cells[3][1] = alive;
                cells[1][2] = alive;
                cells[3][2] = alive;
                cells[2][3] = alive;
                cells[3][3] = alive;
            }
            if(type=="Explosion"){
                cells[6][3] = alive;
                cells[6][4] = alive;
                cells[6][5] = alive;
                cells[6][6] = alive;
                cells[6][7] = alive; 
                cells[10][3] = alive;
                cells[10][4] = alive;
                cells[10][5] = alive;
                cells[10][6] = alive;
                cells[10][7] = alive; 
                cells[8][3] = alive;
                cells[8][7] = alive; 
            }
        }
        

        //executed whenever the step button is pressed 
        void DoFrame() {
            
            canvas.Clear();
            update();
            for (int x = 0; x < cells.size();x++ ){
              for (int y = 0; y < cells[x].size();y++){
                canvas.Rect(40 * x, 40 * y, 40, 40, emp::ColorHSV(0, 0, cells[x][y]), "black");//updates the color of every square according to it's cell value
              }
            }
            
        }
        
        //checks whether the game of life requirement is met and updates the color
        //of the neighbors
        void update(){
           
           std::vector<std::vector<float>> newCells;
           newCells.resize(15, std::vector<float>(10, dead));// new cell vector that stores the previous cells vector value sinc we are going to reset it in this method
           for (int x = 0; x < cells.size();x++ ){
              for (int y = 0; y < cells[x].size();y++){
                newCells[x][y] = cells[x][y];
              }
           }
           reset();
           for (int x = 0; x < newCells.size();x++ ){
              for (int y = 0; y < newCells[x].size();y++){
                int liveN = countLiveNeighbor(newCells,x,y);
                if(newCells[x][y]==alive){
                    if(liveN==2||liveN==3){
                        cells[x][y] = alive;//if a live cell has 2 or 3 live neighbors, it stays alive
                    }
                    else{
                        cells[x][y] = dead;//if a live cell has less than 2 live neghbor, it becomes dead
                    }
                }
                else{
                    if(liveN==3){
                        cells[x][y] = alive;//if a dead cell has exactly 3 live neighbors, it becomes live
                    }
                }
              }
            } 
        }

};

CAAnimator animator;// our main instance of our CAAnimator object

int main() {

     animator.Step(); //calls the doFrame method and spawns the buttons into action
    
}
