#include "ProcessImage.h"
#include "stdio.h"

JSAMPLE * image_buffer = NULL;
//unsigned char *mem = NULL;
//unsigned long mem_size = 0;


typedef struct tagRGBQUAD
{
BYTE rgbBlue;
BYTE rgbGreen;
BYTE rgbRed;
BYTE rgbReserved;
}RGBQUAD;


typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;



typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;  //specifies the number of bytes required by the struct
    DWORD biWidth;  //specifies width in pixels
    DWORD biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//specifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    DWORD biXPelsPerMeter;  //number of pixels per meter in x axis
    DWORD biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by the bitmap
    DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;


typedef struct tagBITMAPINFO
{
BITMAPINFOHEADER bmiHeader;
RGBQUAD bmiColors[1];
}BITMAPINFO;


BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height )
{
	// first make sure the parameters are valid
	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	// find the number of padding bytes

	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
	BYTE* newbuf = malloc(width*height*3);

	// now we loop trough all bytes of the original buffer,
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	int y;
	int x;
	for ( y = 0; y < height; y++ )
		for ( x = 0; x < 3 * width; x+=3 )
		{
			newpos = y * 3 * width + x;
			bufpos = ( height - y - 1 ) * psw + x;

			newbuf[newpos] = Buffer[bufpos + 2];
			newbuf[newpos + 1] = Buffer[bufpos+1];
			newbuf[newpos + 2] = Buffer[bufpos];
		}

	return newbuf;
}


int write_jpeg_file( unsigned char *filename )
{

struct jpeg_compress_struct cinfo;
struct jpeg_error_mgr jerr;
printf("start jpeg processing!\n");
/* this is a pointer to one row of image data */
JSAMPROW row_pointer[1];
FILE *outfile = fopen( filename, "wb" );

if ( !outfile )
{
printf("Error opening output jpeg file %s\n!", filename );
return -1;
}
printf("created file!");
cinfo.err = jpeg_std_error( &jerr );
jpeg_create_compress(&cinfo);
jpeg_stdio_dest(&cinfo, outfile);
//jpeg_mem_dest(&cinfo, &mem, &mem_size);

/* Setting the parameters of the output file here */
cinfo.image_width = 1920;
cinfo.image_height = 1080;
cinfo.input_components = 3;
cinfo.in_color_space = JCS_RGB;
/* default compression parameters, we shouldn't be worried about these */
//printf("parameters set!");
jpeg_set_defaults( &cinfo );
cinfo.num_components = 3;
//cinfo.data_precision = 4;
cinfo.dct_method = JDCT_FLOAT; //Try JDCT_IFAST
jpeg_set_quality(&cinfo, 85, TRUE);
/* Now do the compression .. */
jpeg_start_compress( &cinfo, TRUE );
/* like reading a file, this time write one row at a time */
//printf("start writing!");
BYTE * newbuf =  ConvertBMPToRGBBuffer (image_buffer, cinfo.image_width, cinfo.image_height );

while( cinfo.next_scanline < cinfo.image_height )
{
row_pointer[0] = &newbuf[ cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
jpeg_write_scanlines( &cinfo, row_pointer, 1 );
}

//printf("finninshed writing!");
/* similar to read file, clean up after we're done compressing */
jpeg_finish_compress( &cinfo );
jpeg_destroy_compress( &cinfo );


fclose( outfile );
free(newbuf);
//free(mem);
/* success code is 1! */
return 1;
}



int ConvertImage(unsigned char *img, unsigned char *img_arr, int image_width, int image_height, int bpp ){

//BITMAPFILEHEADER bfh;
//BITMAPINFO bi;
//BITMAPINFOHEADER *bih;
//FILE *input;
//int image_height;
//int image_width;
//int bpp;
//
//input = fopen( img, "rb" ); // Open existing file
//
//// Read bitmap file header
////fread( &bfh, sizeof(BITMAPFILEHEADER),1, input );
//fread( &bfh,1, 14, input );
//
//// Read bitmap info header
////fread( &bi, sizeof(BITMAPINFO),1, input );
//fread( &bi, 1, 40, input );
//
//bih = &bi.bmiHeader;
//image_height = bih->biHeight;
//image_width = bih->biWidth;
//bpp = 3;
//int data_size = image_width * image_height * bpp; // Compute image data size

// Allocate image buffer; this is the buffer write_JPEG_file() will use
image_buffer = img_arr;//(JSAMPLE *)malloc( data_size );

// Read image pixel data from file
//fread( image_buffer, 1, data_size, input );

//fclose( input );

write_jpeg_file( img);

//printf("MEM SIZE = %d", sizeof(mem));
////output_mem = mem;
////*output_size = mem_size;
//free(image_buffer);//       <----------------------------------------------------------------
return 0;
}
