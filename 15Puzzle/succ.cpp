/* This program reads a state from stdin and prints out its successors.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <iostream>
#include <queue>
#include <unordered_set>
#include <stack>
#include <climits>

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

struct Comparator2
{
    bool operator()(const StateNode& lhs, const StateNode& rhs)
    {
        return (lhs.hValue + lhs.cost) < (rhs.cost + rhs.hValue);
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

int testGoal(state_t state){
    if(is_goal(&state)){
        return 1;
    }
    else{
        return 0;
    }
}





int search(std::stack<StateNode> path, std::unordered_set<state_t> visited, int bound, int *nodesExpanded){
    StateNode node = path.top();
    


    //print_state(stdout, &node.data);
    //printf("\n");

    int aux;

    *nodesExpanded = *nodesExpanded + 1;

    int estimate = node.cost + node.hValue;

    if(testGoal(node.data)){
        return -1;
    }

    if(estimate > bound){ 
        return estimate;
    }


    int min = INT_MAX;

    state_t state;
    state_t child;
    ruleid_iterator_t iter; 
    int ruleid ; 

    state = node.data;


    init_fwd_iter( &iter, &state );  // initialize the child iterator 
    while( ( ruleid = next_ruleid( &iter ) ) >= 0 ) {

	    apply_fwd_rule( ruleid, &state, &child );

        std::priority_queue<StateNode, std::vector<StateNode>, Comparator> auxQueue;
        
        if(visited.count(child) == 0){
            auxQueue.push(StateNode(child, heuristic(child), 1+ node.cost));
            visited.insert(child);
        }
        //print_state(stdout, &child);
        //printf("\n");
        while(!auxQueue.empty()){
            path.push(auxQueue.top());
            aux = search(path, visited, bound, nodesExpanded);

            if(aux == -1){
                return -1;
            }
            if(aux < min){
                min = aux;
                //std::cout << "\nMin: " << min << "\n";
            }
            auxQueue.pop();
            path.pop();
        }
        
        
    }
    
    return min;
    

}

void IDA_Star(state_t startState, int *nodesExpanded){
    

    std::stack<StateNode> path;
    std::unordered_set<state_t> visitedList;

    path.push(StateNode(startState, heuristic(startState), 0));
    visitedList.emplace(startState);

    //cost
    
    int bound = heuristic(startState);


    while(true){
        *nodesExpanded = *nodesExpanded + 1; 
  
        //std::cout << " bound " << bound << "\n";

        int aux = search(path, visitedList, bound, nodesExpanded);
        
        if (aux == -1){
            break;
        }
        else if(aux == INT_MAX){
            break;
        }
        else{
            bound = aux;

        } 
        
          
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
    //int childCount;
    //int heu;

    
    //int i = 0;
    while(!testGoal(fringe.top().data)){
   // while(i < 5){
        //i++;

        *nodesExpanded = *nodesExpanded + 1;

        state = fringe.top().data;
        fringe.pop();

      

        //print_state(stdout, &state);
        //printf("\n");
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
    int nodesExpandedA_Star;
    int nodesExpandedIDA_Star;
    int initSteps = 70;
    int maxSteps = 80;
    int numInstances = 100;
    ruleid_iterator_t bwdIter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int bwdruleid ; // an iterator returns a number identifying a rule
    srand (time(NULL));
    float totalTimeA_Star;
    float totalTimeIDA_Star;

    state_t child;


// VARIABLES FOR INPUT
    char str[ MAX_LINE_LENGTH +1 ] ;
    
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    
    

// CONVERT THE STRING TO A STATE
    read_state( "b 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15", &state );
    


    for(int i = initSteps;i <= maxSteps; i++){

        
        nodesExpandedA_Star = 0;
        nodesExpandedIDA_Star = 0;
        totalTimeA_Star = 0;
        totalTimeIDA_Star = 0;

        for(int j = 0; j < numInstances; j++){
            read_state( "b 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15", &state );
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

            //print_state(stdout, &state);
            //printf("\n");

            begin_time = clock();
            a_Star(state, &nodesExpandedA_Star);
            totalTimeA_Star += float(clock() - begin_time);
            
            begin_time = clock();
            IDA_Star(state, &nodesExpandedIDA_Star);
            totalTimeIDA_Star += float(clock() - begin_time);
            
        }
        std::cout << "\nA*: \n";
        std::cout << "Número médio de nodos expandidos com profundidade " << i << ": " << nodesExpandedA_Star / numInstances << "\n";
        std::cout << "Tempo médio com profundidade " << i << ": " << totalTimeA_Star /  CLOCKS_PER_SEC / numInstances << "\n";
        
        std::cout << "\nIDA*: \n";
        std::cout << "Número médio de nodos expandidos com profundidade " << i << ": " << nodesExpandedIDA_Star / numInstances << "\n";
        std::cout << "Tempo médio com profundidade " << i << ": " << totalTimeIDA_Star /  CLOCKS_PER_SEC / numInstances << "\n";
        
    }


    return 0;
} // end main




