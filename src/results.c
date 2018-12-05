#include "stdio.h"
#include "results.h"
#include "daindex.h"
#include <stdlib.h>
#include <iostream>

#define N 4


/*
*	Iterates through column of non Indexed Array nonIndexed for matches on
*	Indexed Array T.
*	Results are stored in a tuple with tuple.key being the nonIndexed columns Id
*	and tuple.payload being the Indexed columns Id
*/

result* getResults(Table_Info *T,Table_Info* nonIndexed,daIndex **Index){

	int key,payload;

	result *r = (result*)malloc(sizeof(result));

	if(r == NULL){
		fprintf(stderr,"Error allocating space for result struct \n");
		exit(0);
	}


	uint64_t result_size = (1024*1024)/sizeof(toumble);
	uint64_t counter = 0;

	r->results_array = (toumble*)malloc(sizeof(toumble)*result_size);
	r->next = NULL;
	r->size = 0;

	uint64_t r_key;
	uint64_t LSB;
	uint64_t hash2_value;
	uint64_t chain_pos;

	uint64_t mask = (1 << N) - 1;


	result *head = r;

	for(int i = 0 ; i < nonIndexed->rows ;i++){
		payload = nonIndexed->R_Payload[i];
		key = nonIndexed->R_Id[i];


		LSB = payload & mask; // Least Significant Bytes kept to go directly to the bucket in question

		hash2_value = payload % HASHFUNC_RANGE;//Position in bucket array to find results

		chain_pos = Index[LSB]->bucket->table[hash2_value];

		if(chain_pos == 0) continue;

		while(chain_pos != 0)
		{
			r_key = chain_pos - 1;		//item in chain

			if(T->bck_array->bck[LSB]->tuplesArray[chain_pos]->payload == payload)
			{

				if(counter == result_size-1)
					{
						counter = 0;
						result *temp = (result*)malloc(sizeof(result));

						if(temp == NULL){
							fprintf(stderr,"Error allocating space for result struct\n");
							exit(0);
						}

						temp->results_array = (toumble*)malloc(sizeof(toumble)*result_size);

						if(temp->results_array == NULL){
							fprintf(stderr,"Error allocating space for results array\n");
							exit(0);
						}

						temp->size = 0;
						temp->next = NULL;
						r->next = temp;
						r = temp;
					}

				r->results_array[counter].key = key; //non Indexed Column key/rowId
				r->results_array[counter].payload = T->R_Id[r_key];	//Indexed Column key/rowId

				counter++;
				r->size++;
			}

			chain_pos = Index[LSB]->chain->array[chain_pos];


		}

	}
	return head;
}

/*
*	Prints kinda prettily the results
*
*/

void print_results(result* r){
	int loop = r->size;
	int i = 0;

	printf("\n");
	printf("|	Table A Row Id  	|	Table B Row Id 		|\n");

	while(loop >= 0 ){

		if((loop == 0) && (r->next != NULL)){

			r = r->next;
			loop = r->size;
			i = 0;

		}else if((loop == 0) && (r->next == NULL)){

			loop--;

		}else{

			std::cout << "|	 	"<< r->results_array[i].key <<"		|	 	"<< r->results_array[i].payload << "		|\n";
			loop--;
			i++;

		}
	}
	printf("\n");
}



void destroy_results(result** r){
	result *temp;


	while((*r) != NULL){
		free((*r)->results_array);
		temp = (*r);
		(*r) = (*r)->next;
		free(temp);
	}

}