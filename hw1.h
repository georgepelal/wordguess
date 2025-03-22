#ifndef _HW1
#define _HW1

typedef enum {
	OK, 
	INVALID_LEN, 
	INVALID_TURNS, 
	INVALID_ARGS
} errorCodeT;

/* 
 * getWord
 * Each call returns the next word in the given file. 
 *
 * Parameters: file name
 * Returns: next word found in given file, or NULL at EOF.
 *
 * Note: If this function is called after the end of the file has been reached, 
 * it will NOT read the words from the beginning, but just return NULL.
 */
char *getWord(char *filename);

/* 
 * printErrorMsg
 * Prints an error message corresponding to the supplied error code, 
 * followed by usage info.
 * 
 * Parameterss: an error code.
 */
void printErrorMsg(errorCodeT code);

#endif
