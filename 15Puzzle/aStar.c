/* This program reads a state from stdin and prints out its successors.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>

#define  MAX_LINE_LENGTH 999 

typedef struct node {
   state_t data;
   int hValue;
   struct node *next;
} StateNode;

typedef struct nodeVisited {
   state_t data;
   struct nodeVisited *next;
} VisitList;

StateNode insert(StateNode *head, state_t newData, int tempHValue){


    if(head == NULL){
        head->data = newData;
        head->hValue = tempHValue;
        head->next = NULL;
    }
    else if (tempHValue <= head->hValue)
    {
        StateNode aux = malloc(sizeof(StateNode));
        aux->data = newData;
        aux->hValue = tempHValue;
        
        aux->next = head;

        return aux;
    }
    else{
        head->next = insert(head->next, newData, tempHValue);
    }
    
}

StateNode delete(StateNode *head, state_t deletedState){
    
    if(head = NULL){
        return NULL;
    }

    else if(head->data == deletedState){
        return head->next;
    }
    
    else{
        return delete(head->next, deletedState);
    }
}

VisitList insertV(VisitList *head, state_t newVisited){
    if(head == NULL){
        head->data = newVisited;
        head->next = NULL;
    }
    else{
        head->next = insert(head, newVisited);
    }
}

int is_in_list(VisitList *head, state_t statekey){
    if (head == NULL){
        return 0;
    }
    else if(head->data == statekey){
        return 1
    }
    else{
        return is_in_list(head->next, statekey);
    }
}




int heuristic(state_t state){
    int value = 0;
    char numbers[];
    sprint_state(char numbers,const size_t MAX_LINE_LENGTH, &state);
    
    for(int i = 1;i <= 16; i++){
        if(numbers[i-1] != 'b'){
            int quotient = (i-1) / 4;
            int rest = i - (quotient * 4);
            
            switch(i-1){
                case 1:
                    int adding_value = abs(quotient) + abs(rest - 1);
                    value += adding_value;
                    break;
                
                case 2:
                    int adding_value = abs(quotient) + abs(rest - 2);
                    value += adding_value;
                    break;
                    
                case 3:
                    int adding_value = abs(quotient) + abs(rest - 3);
                    value += adding_value;
                    break;

                case 4:
                    int adding_value = abs(quotient -1) + abs(rest);
                    value += adding_value;
                    break;

                case 5:
                    int adding_value = abs(quotient -1) + abs(rest -1);
                    value += adding_value;
                    break;

                case 6:
                    int adding_value = abs(quotient -1) + abs(rest -2);
                    value += adding_value;
                    break;

                case 7:
                    int adding_value = abs(quotient -1) + abs(rest-3);
                    value += adding_value;
                    break;

                case 8:
                    int adding_value = abs(quotient -2) + abs(rest);
                    value += adding_value;
                    break;

                case 9:
                    int adding_value = abs(quotient -2) + abs(rest-1);
                    value += adding_value;
                    break;

                case 10:
                    int adding_value = abs(quotient -2) + abs(rest-2);
                    value += adding_value;
                    break;

                case 11:
                    int adding_value = abs(quotient -2) + abs(rest-3);
                    value += adding_value;
                    break;
                
                case 12:
                    int adding_value = abs(quotient -3) + abs(rest);
                    value += adding_value;
                    break;
                
                case 13:
                    int adding_value = abs(quotient -3) + abs(rest-1);
                    value += adding_value;
                    break;
                
                case 14:
                    int adding_value = abs(quotient -3) + abs(rest-2);
                    value += adding_value;
                    break;
                
                case 15:
                    int adding_value = abs(quotient -3) + abs(rest-3);
                    value += adding_value;
                    break;
                
                
            }
        }
    }
    return value;
}

int testGoal(StateNode *head){
    if(is_goal(head->data)){
        return 1;
    }
    else{
        return 0;
    }
}

void a_Star(state_t startState){

    StateNode *head = malloc(sizeof(StateNode));
    head->data = startState;
    head->hValue = heuristic(startState);
    head->next = NULL;

    VisitList *headVisit = malloc(sizeof(VisitList));
    headVisit->data = startState;
    head->next = NULL;

    state_t state;
    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int ruleid ; // an iterator returns a number identifying a rule
    int childCount;

    while(!testGoal(head)){

        state = head->data;

        init_fwd_iter( &iter, &state );  // initialize the child iterator 
        while( ( ruleid = next_ruleid( &iter ) ) >= 0 ) {

	        apply_fwd_rule( ruleid, &state, &child );

            head = insert(head, child, heuristic(child)+get_fwd_rule_cost(ruleid)));
            headVisit = insertV(headVisit, child);

        }
        head = delete(head, state);
    }

    printf("sucess");

}



int main( int argc, char **argv )
{
// VARIABLES FOR INPUT
    char str[ MAX_LINE_LENGTH +1 ] ;
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.



// READ A LINE OF INPUT FROM stdin
    printf("Please enter a state followed by ENTER: ");
    if ( fgets(str, sizeof str, stdin) == NULL ) {
	printf("Error: empty input line.\n");
	return 0; 
    }

// CONVERT THE STRING TO A STATE
    nchars = read_state( str, &state );
    if (nchars <= 0) {
	printf("Error: invalid state entered.\n");
	return 0; 
    }

    printf("The state you entered is: ");
    print_state( stdout, &state );
    printf("\n");

    a_Star(state);

    return 0;
} // end main




