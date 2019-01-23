#include "stdafx.h"
#include "ResourceErrors.h"

const char * StringForResourceError(int error)
{
	switch (error)
	{
	case RESOURCE_ERROR_NO_ERROR:
		return "Success";
	case RESOURCE_ERROR_NOT_SUPPORTED:
		return "Not Supported";
	case RESOURCE_ERROR_INCORRECT_FILE_TYPE:
		return "Incorrect File Type";
	case RESOURCE_ERROR_ERROR_LOADING_FILE:
		return "Error Loading File";
	case RESOURCE_ERROR_ERROR_SAVING_FILE:
		return "Error Saving File";
	case RESOURCE_ERROR_UNKOWN_ERROR:
		return "Unkown Error";
	case RESOURCE_ERROR_NOT_LOADED:
		return "Resource not initialized";
	case RESOURCE_ERROR_ALREADY_LOADED:
		return "Resource Already initialized";
	default:
		return 0;
	}
}	
