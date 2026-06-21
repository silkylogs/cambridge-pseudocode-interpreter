#include "cpi.h"
#include <stdio.h>
#include <ctype.h>

void cpi_error(char *msg) {
	printf("Error: %s\n", msg);
}

char cpi_next_char(CpiData *d) {
	int ic = fgetc(d->fp);
	ic = toupper(ic);

	char c = (c == EOF) ? '\0' : (char)c;

	d->look = c;
	return d->look;
}

bool cpi_match_word(CpiData *d, char *word) {
	while (*word) {
		cpi_next_char(d);
		if (*word != d->look) {
			return false;
		}
		word++;
	}
	return true;
}

void cpi_skip_to_next_line(CpiData *d) {
	while (cpi_next_char(d) != '\n') {};
}

long cpi_match_alpha(CpiData *d) {
	if (!isalpha(cpi_next_char(d))) {
		cpi_expected("Alphabetical character");
	}
	
	return ftell(d->fp);
}

// TODO
char *cpi_match_identifier(CpiData *d) {
	d->curr_id_len = 0;
	d->curr_id_start = cpi_match_alpha(d);
	while (isalnum(cpi_next_char(d))) {
		d->curr_id_len++;
	}
	return d->curr_id_start;
}

void cpi_token_next(CpiData *d) {
	if (cpi_match_word(d, "//")) {
		cpi_skip_to_next_line(d);
	} else if (cpi_match_word(d, "FUNCTION")) {
		d->current_tok = TOK_PROCEDURE;
	}
}

void cpi_handle_procedure(CpiData *d) {
	cpi_match_identifier(d);
	cpi_match_word(d, "(");
	while (cpi_match_word(d, ")")) {
		cpi_match_word_optional(d, "BYREF");
		cpi_match_word_optional(d, "BYVAL");
		cpi_match_word(d, ":");
		cpi_match_identifier(d);
	}
}

void cpi_exec_file(CpiData *d, char *filename) {
	d->fp = fopen(filename, "r");
	if (!d->fp) { 
		cpi_error("Unable to open file");
		d->ok = false;
		return;
	}

	do {
		cpi_token_next(d);

		if (d->current_tok == TOK_PROCEDURE) {
			cpi_handle_procedure(d);
		}
	} while (d->look);

	fclose(d->fp);

	d->ok = true;
	return;
}
