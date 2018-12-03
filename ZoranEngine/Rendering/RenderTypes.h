#pragma once
/*
* RenderDataType represents different ways to render color, r_8 means the renderdata is 1 byte that contains only the red channel
*/
enum RenderDataType
{
	Render_Data_Type_R_8,
	Render_Data_Type_RG_16,
	Render_Data_Type_RA_16,
	Render_Data_Type_RGB_24,
	Render_Data_Type_RGBA_32,
	Render_Data_Type_BGRA_32,
};

/*
* Format in what the data represent so char vs float
*/
enum RenderDataFormat
{
	Render_Data_Format_Float,
	Render_Data_Format_Byte,
	Render_Data_Format_Unsigned_Byte
};