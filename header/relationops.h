#ifndef __RELATIONOPS_H__
#define __RELATIONOPS_H__

#include "../header/parser.h"
#include "../header/relation_loader.h"
#include "../header/results.h"

typedef struct tempResultArray{
	std::vector<uint64_t *> rowID;
	std::vector<int> relationID;														// IDs of relations in tempResultArray
	uint64_t size;																		// Number of rowids in each vector element
}tempResultArray;

typedef struct tempResults{
	std::vector<tempResultArray> res;
}tempResults;


tempResults *queryExecute(Query *, relationArray *,std::vector<Relations*>& ,std::vector<uint64_t>&);

void queryReorder(Query *);															// Reorders predicates in a query

void filtered_relation(predicates* ,relationArray* );
uint64_t *createRowID(uint64_t);
void deleteTR(tempResults** );
void relation_join(predicates *, relationArray *, tempResults *);
uint64_t *tempResultsLookup(tempResults *,int, uint64_t *);
uint64_t getChecksum(tempResultArray* ,relationArray* ,std::vector<checksum_views*> );

int tempResultsFilterUpdate(std::vector<uint64_t> &,int,tempResults *);
int tempResultsAdd(std::vector<uint64_t> &,int,tempResults *);
uint64_t **convert_to_arrays(result *,uint64_t &ts);

void tempResultsJoinUpdate(uint64_t **,int ,int, int, int, uint64_t, tempResults *);
void jointest(void);
void specialCase(relationArray *, tempResults *, int, int ,int);
Query *editQuery(Query *);

void copy_filtered(predicates *,relationArray *,std::vector<Relations*>& ,std::vector<uint64_t>& );
void replace_filtered(relationArray* ,std::vector<Relations*>& ,std::vector<uint64_t>& );


// Debug stuff

void printJoinResults(uint64_t **, relationArray *, int, int, int);
void queryPrint(Query *);
void printTPR(tempResults *, relationArray *);

#endif