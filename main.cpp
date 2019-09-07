
#include <iostream>
using namespace std;

enum occupied {empty, ex, zero};


void switchTurn(occupied &turn)
{
    if (turn == ex) turn = zero;
    else turn = ex;
    return;
}

occupied other(occupied oc)
{
    if (oc == ex) return zero;
    else return ex;
}

class Square
{
public:
    occupied resident;
    Square()
    {
        resident = empty;
    }
    
    Square (occupied type)
    {
        resident = type;
    }
    
    Square (Square &s)
    {
        resident = s.resident;
    }
    
    void print()
    {
        if (resident == ex) cout<<"x ";
        else if (resident == zero) cout<<"o ";
        else cout<<". ";
        return;
    }
    
};

class Location
{
public:
    int row, column;
    Location() {}
    Location (int f, int r)
    {
        row = f; column = r;
    }
     bool operator !=(Location l)
    {
        return !(row == l.row && column == l.column);
    }
};

class Grid
{
public:
    Square grid[3][3];
    occupied turn;
    
    
    
    Grid()
    {
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
            grid [i][j] = Square();
            }
        }
        turn = ex;
    }
    
    Grid(Grid &g)
    {
        turn = g.turn;
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                grid [i][j] = g.grid[i][j];
            }
        }
    }
    
    void print()
    {
        for (int i=0; i<3; i++)
        {
            cout<<i<<" ";
            for (int j=0; j<3; j++)
            {
                grid[i][j].print();
            }
            cout<<endl;
        }
        cout<<"  ";
        for (int j=0; j<3; j++) {cout<<j<<" ";}
        cout<<endl;
        if (turn == ex) cout<<"x ";
        else cout<<"o ";
        cout<<"to play"<<endl;
    }
    
    bool gameOver()
    {
        bool row[3], column[3], diagNE, diagNW;
        for (int i=0; i<3; i++) {row[i] = true; column[i] = true;}
        diagNE = true; diagNW = true;
        occupied check = other(turn);
        for (int i = 0; i < 3; i++) //i denotes the row we're checking
        {
            for (int j = 0; j<3 && row[i]; j++)
            {
                row[i] = (grid[i][j].resident == check);
            }
            if (row[i]) return true;
        }
        
        for (int i = 0; i < 3; i++) //i denotes the column we're checking
        {
            for (int j = 0; j<3 && column[i]; j++)
            {
                column[i] = (grid[j][i].resident == check);
            }
            if (column[i]) return true;
        }
        
        for (int i=0; i<3 && diagNW; i++)
        {
            diagNW = (grid[i][i].resident == check);
        }
        if (diagNW) return true;
        
        for (int i=0; i<3 && diagNE; i++)
        {
            diagNE = (grid[i][2-i].resident == check);
        }
        if (diagNE) return true;
        return false;
    }
    
    bool plant(int rank, int column)
    {
        if(rank < 3 && rank >= 0 && column >= 0 && column < 3 && grid[rank][column].resident == empty)
        {
            grid[rank][column].resident = turn;
            switchTurn(turn);
            return true;
        }
        else
            return false;
    }
    
    bool full()
    {
        bool answer = true;
        for (int i = 0; i<3 && answer; i++)
        {
            for (int j=0; j<3 && (answer); j++)
            {
                answer = (grid[i][j].resident != empty);
            }
        }
        return answer;
    }
    
    bool draw()
    {
        return full()&&(!gameOver());
    }
    
    Location winning(int n)
    {
        if (n==1)
        {
            for (int i=0; i<3; i++)
            {
                for (int j=0; j<3; j++)
                {
                    Grid analysis(*this);
                    if (analysis.plant(i, j))
                    {if (analysis.gameOver()) return Location(i, j);} //return Location(i, j)?
                }
            }
            return Location(-1, -1); //return Location(-1, -1)
        }
        
        bool answer = false;
        
        for (int i=0; i<3 && !answer; i++)
        {
            for (int j=0; j<3 && !answer; j++)
            {
                Grid analysis(*this);
                if (analysis.plant(i, j))
                {
                    bool confirm = true;
                    if (analysis.draw()) confirm = false;
                    for (int p=0; p<3 && confirm; p++)
                    {
                        for (int q=0; q<3 && confirm; q++)
                        {
                            Grid an2(analysis);
                            if (an2.plant(p, q))
                            {
                                if (an2.gameOver() )
                                {
                                    confirm = false;
                                }
                                else
                                {
                                    bool force = false;
                                    for (int count = 1; count < n && !force; count++)
                                    {
                                        force = (an2.winning(count) != Location(-1, -1));
                                    }
                                    confirm = force;
                                }
                            }
                        }
                    
                    }
                    answer = confirm;
                    if (confirm) return Location(i, j); //if answer is true, we are set to leave the i, j loop. return Location(i, j)
                }
            }
        }
        return Location(-1, -1); //if you get here, there is no win. return Location(-1, -1)
    }
    
    Location winning() //Location winning()
    {
        for (int n=1; n<4; n++)
        {
            if (winning(n) != Location(-1, -1)) return winning(n); //Location(i, j) if it is valid, else continue...
        }
        return Location(-1, -1); //return Location(-1, -1)
    }
    
    bool losing(Location l) //call this only when you're sure l is empty.
    {
        Grid an(*this);
        an.plant(l.row, l.column);
        return (an.winning() != Location(-1, -1));
    }
    
    int waysToLose()
    {
        int n =0;
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                if (grid[i][j].resident == empty)
                {
                    if (losing(Location(i, j))) n++;
                }
            }
        }
        return n;
    }
    
    void playComputerMove() //called when there is at least one non-losing move available.
    {
        Location winner = winning();
        if (winner != Location(-1, -1)) {plant(winner.row, winner.column); print(); return;}
        
        //at this point, we've established that there isn't any win.
        
        int losers = 0; Location l;
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                Grid an(*this);
                Grid ana(*this);
                
                if (an.plant(i, j))
                {
                    if (ana.losing(Location(i, j))) {continue;}
                    int challenger = an.waysToLose();
                    if (challenger >= losers)
                    {
                        l = Location(i, j);
                        losers = challenger;
                    }
                }
            }
        }
        plant(l.row, l.column); print();  return;
    }
    
    void playHumanMove()
    {
        int i = -1, j = -1;
        while (!plant(i, j))
        {
            cout<<"Enter valid move"<<endl;
            cin>>i>>j;
        }
        print(); return;
    }
};

int main()
{
    cout<<"Play as x or o?"<<endl;
    char response;
    cin>>response;
    occupied human;
    if (response == 'o') human = zero;
    else human = ex;
    Grid game;
    if (response == 'x') game.print();
    if (response == 'o')
        game.playComputerMove();
    while (!(game.gameOver() || game.draw()) && game.turn == human)
    {
        game.playHumanMove();
        while (!(game.gameOver() || game.draw()) && game.turn != human)
        {
            if (game.winning() != Location(-1, -1)) {cout<<"Winning"<<endl;}
            game.playComputerMove();
            
        }
    }
    if (game.gameOver()) {cout<<"Game Over"<<endl; return 0;}
    if (game.draw()) {cout<<"Draw."<<endl; return 0;}
}
