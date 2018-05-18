#pragma once
/*
* RenderDataType represents different ways to render color, r_8 means the renderdata is 1 byte that contains only the red channel
*/
enum RenderDataType
{
	TYPE_R_8,
	TYPE_RG_16,
	TYPE_RGB_24,
	TYPE_RGBA_32,
	TYPE_BGRA_32,
};

/*
* Format is what the data represent so char vs float
*/
enum RenderDataFormat
{
	FORMAT_FLOAT,
	FORMAT_BYTE,
	FORMAT_UNSIGNED_BYTE
};