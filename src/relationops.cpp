#include "../header/parser.h"
#include "../header/relationops.h"
#include "../header/relation_loader.h"
#include "../header/helper_functions.h"
#include "../header/daindex.h"
#include "../header/results.h"

#include <iostream>

void queryExecute(Query *qr, relationArray *relArray)
{
	Query *orderedQuery = queryReorder(qr);												// Reorders predicates in query for optimization purposes
	tempResults *tRes = new tempResults;

	std::vector<predicates*>::iterator it;

	for( it = orderedQuery->p.begin(); it != orderedQuery->p.end(); it++)
		if( (*it)->type == JOIN)														// Each predicate is either a join or a filter
		{
			// join
		}
		else
		{
			// filter
		}

	// TODO: Create Final Results using tempResults;

	// TODO: checksum view

	delete orderedQuery;

	return /*results*/;
}


Query *queryReorder(Query *qr)
{
	Query *newQr = new Query;

	newQr->p = qr->p;
	newQr->relations = qr->relations;
	newQr->checksums = qr->checksums;

	return newQr;
}

void relation_filter(predicates *pred, relationArray *rArray, tempResults *tr)
{

	int relationId = pred->relation1;
	int columnId = pred->column1;
	uint64_t filter = pred->filter;

	Relations * currentRelation = rArray->relations.at(relationId);						// Fetch relation

	uint64_t *size;
	uint64_t *rowids = tempResultsLookup(tr, relationId, size);							// Also check if relation exists on tempResults

	std::vector<uint64_t> results;														// Result vector

	if(rowids != NULL )																	// If relation exists on tempResults
	{

		for(uint64_t row=0; row < *size; row++)
		{
			uint64_t rid = rowids[row];

			switch (pred->type) {
				case EQ_FILTER:
					if( filter == currentRelation->relation[columnId][rid])
						results.push_back(rid);
					break;

				case GT_FILTER:
					if( filter <= currentRelation->relation[columnId][rid])
						results.push_back(rid);
					break;

				case LT_FILTER:
					if( filter >= currentRelation->relation[columnId][rid])
						results.push_back(rid);
					break;

				default:
					//This should never happen
					break;
			}
		}
		// TODO: tempResultsUpdate(results,relation1,tr);
	}
	else
	{
		for(uint64_t row=0; row < currentRelation->size; row++)
		{

			switch (pred->type) {
				case EQ_FILTER:
					if( filter == currentRelation->relation[columnId][row])
						results.push_back(row);
					break;

				case GT_FILTER:
					if( filter <= currentRelation->relation[columnId][row])
						results.push_back(row);
					break;

				case LT_FILTER:
					if( filter >= currentRelation->relation[columnId][row])
						results.push_back(row);
					break;

				default:
					//This should never happen
					break;
			}
		}
		// TODO: tempResultsAdd(results,relation1,tr);
	}

	// TODO: return results;

}


void relation_join(predicates *pred, relationArray *rArray)
{
	int relationId1 = pred->relation1;
	int relationId2 = pred->relation2;
	int columnId1   = pred->column1;
	int columnId2   = pred->column2;

	Relations * currentRelation1 = rArray->relations.at(relationId1);
	Relations * currentRelation2 = rArray->relations.at(relationId2);
	uint64_t *rowids;
	// TODO: check temp_results

	// TODO: check fringe case if relation1 = relation2

	uint64_t *rowID1 = createRowID(currentRelation1->size);
	uint64_t *rowID2 = createRowID(currentRelation1->size);

	Table_Info *tableInfo1 = init_table_info(rowID1,currentRelation1->relation[columnId1],currentRelation1->size);
	Table_Info *tableInfo2 = init_table_info(rowID2,currentRelation2->relation[columnId2],currentRelation2->size);

	daIndex **indx;
	result *res;

	if(rowID1 < rowID2)
	{
		indx = DAIndexArrayCreate(tableInfo1->bck_array);
		res = getResults(tableInfo1,tableInfo2,indx);
	}
	else
	{
		indx = DAIndexArrayCreate(tableInfo2->bck_array);
		res = getResults(tableInfo2,tableInfo1,indx);
	}

	// TODO: update temp_results

	return /*res*/;
}


uint64_t *createRowID(uint64_t rSize)
{
	uint64_t *rowID = new uint64_t[rSize];
	for(uint64_t i=0; i < rSize; i++)	rowID[i] = i;
	return rowID;
}


uint64_t *tempResultsLookup(tempResults *tpr, int relationId, uint64_t *size)
{
	if( tpr == NULL )	return NULL;

	if( tpr->res.size() == 0 )	return NULL;

	std::vector<tempResultArray>::iterator it1;

	for(it1 = tpr->res.begin(); it1 != tpr->res.end(); it1++)
	{
		size = &(*it1).size;

		for( uint64_t j=0; j<(*it1).relationID.size(); j++)
		{
			if( (*it1).relationID.at(j) == relationId )	return (*it1).rowID.at(j);
		}

	}

	return NULL;
}

int tempResultsUpdate(std::vector<uint64_t> &results, int relationId, int columnId, tempResults *tpr)
{
	std::vector<tempResultArray>::iterator it;

	for(it = tpr->res.begin(); it != tpr->res.end(); it++)
		for(uint64_t i=0; i< (*it).relationID.size(); i++)
		{
			// edw
		}

}



void jointest()
{
	cout << "ok" << std::endl;

	uint64_t * rowID1 = new uint64_t[3];
	uint64_t * rowID2 = new uint64_t[5];

	rowID1[0] = 1;
	rowID1[1] = 2;
	rowID1[2] = 3;

	rowID2[0] = 1;
	rowID2[1] = 2;
	rowID2[2] = 3;
	rowID2[3] = 4;
	rowID2[4] = 5;

	uint64_t *arr1 = new uint64_t[3];
	uint64_t *arr2 = new uint64_t[5];

	arr1[0] = 5;
	arr1[1] = 1;
	arr1[2] = 9;

	arr2[0] = 7;
	arr2[1] = 6;
	arr2[2] = 1;
	arr2[3] = 9;
	arr2[4] = 1;

	Table_Info *tableInfo1 = init_table_info(rowID1, arr1, 3);
	Table_Info *tableInfo2 = init_table_info(rowID2, arr2, 5);

	daIndex **indx = DAIndexArrayCreate(tableInfo2->bck_array);

	for(int i=0; i<16; i++)
	{
		cout << "bucket" << i << endl;
		for(int j=0;j<indx[i]->bucket->size;j++)
			cout << indx[i]->bucket->table[j] << endl;
		cout << endl;
	}

	result *res = getResults(tableInfo2,tableInfo1,indx);

	print_results(res);

}


