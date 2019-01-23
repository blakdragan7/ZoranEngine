#pragma once
/*
*	All errors returned by Resources will be listed here
*/

#define RESOURCE_ERROR_NO_ERROR 0
#define RESOURCE_ERROR_NOT_SUPPORTED 1
#define RESOURCE_ERROR_INCORRECT_FILE_TYPE 2
#define RESOURCE_ERROR_ERROR_LOADING_FILE 3
#define RESOURCE_ERROR_ERROR_SAVING_FILE 4
#define RESOURCE_ERROR_UNKOWN_ERROR 5
#define RESOURCE_ERROR_NOT_LOADED 6
#define RESOURCE_ERROR_ALREADY_LOADED 7

extern const char* StringForResourceError(int error);