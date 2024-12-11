# include <unistd.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
// #include "../Includes/decoder.h"

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

typedef union s_pixel
{
	u32	pixel;
	struct
	{
		u8	b;
		u8	r;
		u8	g;
	};
	//Pixel = 4 Bytes = 1 Byte = 8 Bites
}		t_pixel;

typedef struct s_position
{
	size_t	x;
	size_t	y;
}			t_position;

/*
	Increases the HEIGHT to check, wether the pixel has the same
	Color
*/
bool	check_8_8_matching(t_position initial_found, struct file_content raw_file, int *absolut)
{
	int	i;
	t_pixel		pixel_header;

	printf("I'm in check_8_8_matching");
	pixel_header.b = 127;
	pixel_header.r = 188;
	pixel_header.g = 217;

	i = 0;
	//First Part of L - Shape
	while (i < 6)
	{
		// raw_file.data + initial_found.y;
		(*absolut) = initial_found.y;
		if (pixel_header.b != raw_file.data[*absolut] \
			|| pixel_header.r != raw_file.data[*absolut] \
			|| pixel_header.g != raw_file.data[*absolut])
		{
			return (false);
		}
		i++;
	}
	i = 0;
	while (i < 5)
	{
		if (pixel_header.b != raw_file.data[*absolut] \
			|| pixel_header.r != raw_file.data[*absolut] \
			|| pixel_header.g != raw_file.data[*absolut])
		{
			return (false);
		}
		(*absolut)++;
		i++;
	}
	//Here Last Top-right-Corner Pixel
	return (true);
}

/*
	Returns true if current Pixel matches the color of Header-File Pixel
*/
int	compare_brg_cannels()
{
	return (-1);
}

/*
	header pixels is always (127, 188, 217) in BGR
*/
size_t	findRightCornerPixel(size_t WIDTH, size_t HEIGHT, struct file_content raw_file)
{
	bool		hp; //Flag for HeaderPixel
	int			i;
	int			k;
	int			absolut; //Total Amount of Pixels in hole image raw_file.size
	// int		initial_found[2];
	t_position	initial_found;
	t_pixel		pixel_header;

	hp = false;
	pixel_header.b = 127;
	pixel_header.r = 188;
	pixel_header.g = 217;
	absolut = 0;
	// u32 data_offset = 0;
	i = 0;
	//raw_file.data[absolut] or raw_file.data + data_offset ?
	while ((size_t)i < HEIGHT)
	{
		k = 0;
		while ((size_t)k < WIDTH)
		{
			if (pixel_header.b == raw_file.data[absolut] \
				&& pixel_header.r == raw_file.data[absolut] \
				&& pixel_header.g == raw_file.data[absolut])
			{
				/*
					flag colorFound = true + Gehe eine Reihe hoch + Speicher "first_found_Position"
					Wieder ColorFound ? --> weiter, wenn nicht dann wieder zu "first found Position"
				*/
				initial_found.x = WIDTH;
				initial_found.y = HEIGHT;
				hp = true;
				int	lenofmsh;
				lenofmsh = check_8_8_matching(initial_found, raw_file, &absolut);//TODO: checks if Pixels at HEIGHT + 1 still matches
				if (lenofmsh > 0)
					return (lenofmsh);
			}
			k++;
			absolut++;
		}
		i++;
		absolut++;
	}
	//
	printf("[In findRightCornerPixel]: Nothing found\n");
	return (-1);
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
	int		len;

	(void)decoded_msg;
	(void)amountOfPixels;
	(void)i;
	(void)k;
	(void)file_content;
	printf("In decode_msg Funktion\n");
	len = findRightCornerPixel(WIDTH, HEIGHT, file_content);
	if (len == -1)
		return (NULL);
	// decoded_msg = (char *)malloc(sizeof(char) * (len + 1));
	// if (!decode_msg)
	// {
	// 	fprintf(STDERR_FILENO, "[Error]: Failed to Allocate string in %s at line: %d\n", __FILE__, __LINE__);
	// 	return (NULL);
	// }
	// //TODO: HEIGHT & WIDTH of Msg (Data without Header)
	// /*
	// 	HEIGHT"
	// 	Amount of Pixel of the msg without Header
	// */
	// // amountOfPixels = file_content.data - sizeof(bmp_header) / file_content.data[0];
	// i = 0;
	// while (i < HEIGHT)
	// {
	// 	k = 0;
	// 	while (k < WIDTH)
	// 	{
	// 		/*
	// 			Hier auf Pixel-Ebene: Hier Fnc die Pixel uebersetzt und speichert
	// 			FNC:
	// 			TODO: decode_pixel();
	// 		*/
	// 		k++;
	// 	}
	// 	i++;
	// }
	// // TODO: return (decoded_msg);
	return ("alo");
}

struct file_content read_entire_file(char* filename)
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

	decoded_msg = decode_msg(file_content, header->width, header->height);
	if (!decoded_msg)
	{
		printf("Header Could not be found!\n");
	}
	// printf("decoded_msg: %s\n", decoded_msg);
	return 0;
}
