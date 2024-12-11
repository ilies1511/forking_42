#include "../Includes/decoder.h"

typedef char i8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef int i32;
typedef unsigned u32;
typedef unsigned long u64;

#define PRINT_ERROR(cstring) write(STDERR_FILENO, cstring, sizeof(cstring) - 1)

#pragma pack(1)
struct bmp_header
{
	// Note: header
	i8  signature[2]; // should equal to "BM"
	u32 file_size;
	u32 unused_0;
	u32 data_offset;

	// Note: info header
	u32 info_header_size;
	u32 width; // in px
	u32 height; // in px
	u16 number_of_planes; // should be 1
	u16 bit_per_pixel; // 1, 4, 8, 16, 24 or 32
	u32 compression_type; // should be 0
	u32 compressed_image_size; // should be 0
	// Note: there are more stuff there but it is not important here
};

struct file_content
{
	i8*   data;
	u32   size;
};

typedef struct s_pixel
{
	u32	pixel[4]; //Pixel = 4 Bytes = 1 Byte = 8 Bites

}		t_pixel;

struct file_content   read_entire_file(char* filename)
{
	char* file_data = 0;
	unsigned long	file_size = 0;
	int input_file_fd = open(filename, O_RDONLY);
	if (input_file_fd >= 0)
	{
		struct stat input_file_stat = {0};
		stat(filename, &input_file_stat);
		/*
			What: function obtains information about the file pointed to by path
			Return Value -> succes: 0, Fail: -1
		*/
		file_size = input_file_stat.st_size;
		file_data = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, input_file_fd, 0);
		close(input_file_fd);
	}
	return (struct file_content){file_data, file_size};
}

/*
	1. int len = findRightCornerPixel() --> contains the length of msh
	2. Allocate char *decoded_msg = malloc(sizeof(char) * (len + 1));
	3. MAIN LOGIC
*/
char	*decode_msg(struct file_content file_content, size_t WIDTH, size_t HEIGHT)
{
	char	*decoded_msg; //Only Malloc
	int		amountOfPixels;
	int		i;
	int		k;

	(void)decode_msg;
	(void)file_content;
	printf("In decode_msg Funktion\n");

	//TODO: HEIGHT & WIDTH of Msg (Data without Header)
	/*
		HEIGHT"
		Amount of Pixel of the msg without Header
	*/
	// amountOfPixels = file_content.data - sizeof(bmp_header) / file_content.data[0];
	i = 0;
	while (i < HEIGHT)
	{
		k = 0;
		while (k < WIDTH)
		{
			/*
				Hier auf Pixel-Ebene: Hier Fnc die Pixel uebersetzt und speichert
				FNC:
				TODO: decode_pixel();
			*/
			k++;
		}
		i++;
	}
	// TODO: return (decoded_msg);
	return ("alo\n");
}

/*
	gcc -O0 -Wall -Wextra -Werror main.c -o decoder -lpthread
*/
int main(int argc, char** argv)
{
	char	*decoded_msg;

	decoded_msg = NULL;
	if (argc != 2)
	{
		PRINT_ERROR("Usage: decode <input_filename>\n");
		return 1;
	}
	struct file_content file_content = read_entire_file(argv[1]);
	if (file_content.data == NULL)
	{
		PRINT_ERROR("Failed to read file\n");
		return 1;
	}
	struct bmp_header* header = (struct bmp_header*) file_content.data;
	printf("signature: %.2s\nfile_size: %u\ndata_offset: %u\ninfo_header_size: %u\nwidth: %u\nheight: %u\nplanes: %i\nbit_per_px: %i\ncompression_type: %u\ncompression_size: %u\n", header->signature, header->file_size, header->data_offset, header->info_header_size, header->width, header->height, header->number_of_planes, header->bit_per_pixel, header->compression_type, header->compressed_image_size);
	/*
		Msg Decoder using
		struct file_content
	{
		i8*   data;
		u32   size;
	};
	*/

	// decoded_msg = decode_msg(file_content, header->width, header->height);
	// printf("decoded_msg: %s\n", decoded_msg);
	return 0;
}
