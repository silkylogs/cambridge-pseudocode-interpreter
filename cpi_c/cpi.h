#ifndef CPI_H
#define CPI_H

#include <stdio.h>
#include <stdbool.h>

typedef enum CpiTokenType CpiTokenType;
enum CpiTokenType {
	TOK_PROCEDURE,
};

typedef struct CpiData CpiData;
struct CpiData {
	FILE *fp;
	bool ok;
	char look;
	CpiTokenType current_tok;

	// Identifier
	long curr_id_start;
	size_t curr_id_len;
};

void cpi_exec_file(CpiData *d, char *filename);

#endif
