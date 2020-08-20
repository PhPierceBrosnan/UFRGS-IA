/* This program reads a state from stdin and prints out its successors.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <iostream>
#include <queue>
#include <unordered_set>
#include <stack>

#define  MAX_LINE_LENGTH 999 
#define HAVE_BWD_MOVES




struct StateNode{
    state_t data;
    int hValue;
    int cost;

    public:
        StateNode(state_t st, int hV, int c){
            data = st;
            hValue = hV;
            cost = c;
        }

};

struct Comparator
{
    bool operator()(const StateNode& lhs, const StateNode& rhs)
    {
        return (lhs.hValue + lhs.cost) > (rhs.cost + rhs.hValue);
    }
};

inline bool operator == (state_t const& lhs, state_t const& rhs)
{
    return !compare_states(&lhs, &rhs);
}

namespace std {
  template <>
  struct hash<state_t>
  {
    size_t operator()(const state_t & t) const
    {
      return hash_state(&t);
    }
  };
}





int heuristic(state_t state){
    int value = 0;
    char numbers[MAX_LINE_LENGTH +1];
    char *numbers_splitted;
    int adding_value;
    int quotient;
    int rest;
    int quotientC;
    int restC;
    int converted;
    sprint_state(numbers, MAX_LINE_LENGTH +1, &state);
    
  
    numbers_splitted = strtok(numbers, " ");

    if(strcmp(numbers_splitted, "B") != 0){
            
            converted = atoi(numbers_splitted);

            quotient = 0 / 4;
            rest = 0 - (quotient * 4);

            quotientC = converted / 4;
            restC = converted - quotientC * 4;

            adding_value = abs(quotient - quotientC) + abs(rest - restC);
            
            value += adding_value;

        }


    for(int i = 1;i < 16; i++){  
        numbers_splitted = strtok(NULL, " ");
        if(strcmp(numbers_splitted, "B") != 0){
            
            converted = atoi(numbers_splitted);

            quotient = i / 4;
            rest = i - (quotient * 4);

            quotientC = converted / 4;
            restC = converted - quotientC * 4;

            adding_value = abs(quotient - quotientC) + abs(rest - restC);
            
            value += adding_value;

        }
    }
    return value;
}

int testGoal(const StateNode *head){
    if(is_goal(&(head->data))){
        return 1;
    }
    else{
        return 0;
    }
}

void a_Star(state_t startState, int *nodesExpanded){

    

    std::priority_queue<StateNode, std::vector<StateNode>, Comparator> fringe;
    std::unordered_set<state_t> visitedList;

    //starting fringe with start state
    fringe.push(StateNode(startState, heuristic(startState), 0));

    //new visited list
    visitedList.emplace(startState);

    state_t state;
    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int ruleid ; // an iterator returns a number identifying a rule
    int childCount;
    int heu;

    
    //int i = 0;
    while(!testGoal(&fringe.top())){
   // while(i < 5){
        //i++;

        *nodesExpanded = *nodesExpanded + 1;

        state = fringe.top().data;
        fringe.pop();

        //print_state(stdout, &state);
        //printf(" ");
        //heu = heuristic(state);
        //printf("%d", heu);
        //printf("\n");

        init_fwd_iter( &iter, &state );  // initialize the child iterator 
        while( ( ruleid = next_ruleid( &iter ) ) >= 0 ) {

	        apply_fwd_rule( ruleid, &state, &child );

            
            if(visitedList.count(child) == 0){
                fringe.push(StateNode(child, heuristic(child), 1+fringe.top().cost));
                visitedList.emplace(child);
            }
        }
        
    }

 

}



int main( int argc, char **argv )
{
    clock_t begin_time = clock();
    int nodesExpanded;
    int initSteps = 4;
    int maxSteps = 10;
    int numInstances = 10;
    ruleid_iterator_t bwdIter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int bwdruleid ; // an iterator returns a number identifying a rule
    srand (time(NULL));

    state_t child;


// VARIABLES FOR INPUT
    char str[ MAX_LINE_LENGTH +1 ] ;
    
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    
    

// CONVERT THE STRING TO A STATE
    read_state( "b 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15", &state );
    


    for(int i = initSteps;i <= maxSteps; i++){

        clock_t begin_time = clock();
        nodesExpanded = 0;

        for(int j = 0; j < numInstances; j++){
            
            int steps = 0;
            while(steps < i){

                
                steps++;

                state_t children[4];
                int arrayIndex = 0;

                init_bwd_iter( &bwdIter, &state ); 
                while( ( bwdruleid = next_ruleid( &bwdIter ) ) >= 0 ) {

	                apply_bwd_rule( bwdruleid, &state, &children[arrayIndex]);
                    arrayIndex++;

                    

                }
                state = children[rand() % arrayIndex];

                

            }

            
            a_Star(state, &nodesExpanded);
            
        }

        std::cout << "Número médio de nodos expandidos com profundidade " << i << ": " << nodesExpanded / numInstances << "\n";
        std::cout << "Tempo médio com profundidade " << i << ": " << float( clock () - begin_time ) /  CLOCKS_PER_SEC / numInstances << "\n";
    }
    

    
    
    std::cout << "Nodes: " << nodesExpanded;

    return 0;
} // end main




