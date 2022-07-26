#ifndef WEB_H
#define WEB_H

#include "stdio.h"
#include "stdlib.h"
#include "sha256.h"
#include <sys/stat.h>

#define WEB_MAX_QUERY_STRING 1024
//maximum permissible size of post data is 10 MB;
//TODO(GI): What is the right maximum size to use?
#define WEB_MAX_POST_DATA 10485760

#define TEC_MOST_SIG_BIT 128
#define TEC_2ND_MOST_SIG_BIT 64

#ifndef MAX_PATH
#define MAX_PATH 260
// MAX_PATH including NULL character
#define MAX_PATH_INC MAX_PATH + 1
#endif

#define web_print_header() printf("Content-Type: text/html; charset=utf-8\r\n\r\n")

#define web_print_relocate_header(X) printf("Status:308 Permanent Redirect\nLocation: %s\r\n\r\n", X)

/*web_get_query_string:
	a query string typically looks something like this:
	var1=abc&var2=def&var3=1%2B1
	this function will retrieve the query string
	it will split the query string where there is a '&'
	it will convert the escaped symbols e.g. %2B -> '+'
	this does modify the original string, make a copy if needed
*/
char** web_get_query_string();

/*web_get_post_string:
	this does the same thing as web_get_query_string
	but gets data sent with POST method
*/
char** web_get_post_string();

/*web_get_from_query_string:
	returns a string which contains the value of variable var_name
	this requires you first used web_get_query_string()
	returns NULL if var_name was not found
	char *qss: this is the output of web_get_query_string
	char *var_name: name of variable you are seeking the value of
*/
char* web_get_from_query_string(char **qss, char *var_name);
#define web_get_from_post_string(X, Y) web_get_from_query_string(X, Y)

/*tec_buf_begins:
	returns 1 if the buffer begins with string str
	returns 0 in all other cases, including errors and str being longer than buffer
*/
int tec_buf_begins(char *buffer, char *str);

/*tec_buf_cmp:
	compares 2 buffers with each other from 'begin' bytes in until and including 'end'
	do ensure the buffers are long enough
	returns 1 if they are equal
	returns 0 in all other cases (including any or both are NULL pointers)
*/
int			tec_buf_cmp(char *buf1, char *buf2, int begin, int end);

/*tec_buf_find_str:
*/
int			tec_buf_find_str(char *buffer, int buffer_size, char *to_find);

/*tec_string_length:
	returns the length of a string in bytes
	check tec_string_utf8_length to know the number of codepoints
	unlike strlen, this function does not segfault when you give it a NULL pointer
	instead it returns zero because, well, you gave it an empty string ;-)
*/
int tec_string_length(char *str);

/*tec_string_find_char:
	returns index of first instance of a character in a string
	returns -1 if not found
*/
int tec_string_find_char(char *s, char to_find);

/*tec_string_find_str:
	look for an instance of to_find in string
	returns index in string where to_find was found
	returns -1 when nothing was found
*/
int			tec_string_find_str(char *string, char *to_find);

/*tec_string_shift:
	removes an ascii char or unicode codepoint at front of string
	assumes a valid utf8 string
*/
void tec_string_shift(char *str);

/*tec_string_split:

	splits a string where char c does occur
	num will give you the number of times char c did occur in the string
	num is also index of last string in returned char**
	CAUTION: there is one more string than occurences of char c, this might include empty strings

	the string str will be modified so make sure to make a copy if needed

	note that some character pointers may be NULL pointers
	if 2 or more characters c are right next to each other

	free the returned char ** when done

	returns NULL in case of error
*/
char** tec_string_split(char *str, char c, int *num);

/*tec_string_to_int:
	converts a string to an integer
	string may not contain anything in front of number except '-' or '+'
	does not safeguard against overflow
*/
int tec_string_to_int(char *s);

/*tec_string_copy:
	safer alternative to strcpy or even strncpy
	int size is the size of the destination
	these functions guarantee that at most size - 1 bytes will be written to destination plus a NULL character
	this prevents buffer overflows
	this guarantees you get a NULL terminated string in destination (unlike strncpy)
	this function will not cut off the copying in the middle of a UTF8 codepoint when out of space
	returns 1 if all was copied right
	returns 2 if source could not be fully copied
	returns 0 in case of error

	these functions assume char *source is a correctly formatted UTF8 string
*/
int			tec_string_copy(char *destination, char *source, int size);

/*tec_string_concatenate:
tec_string_cat:
	concatenates two strings
	will guarantee a NULL pointer at end of destination even if source is too large to fit
	this function will not cut off the copying in the middle of a UTF8 codepoint when out of space
	returns 0 in case of error
	returns 1 if everything went right
	returns 2 if source was too large
*/
int			tec_string_concatenate(char *destination, char *source, int size);
#define tec_string_cat(X, Y, Z) tec_string_concatenate(X, Y, Z)

/*tec_char_is_white_space:
	returns 1 if c is a white space character (e.g. space)
	returns 0 otherwise
	assumes 7 bit ascii character
	there are more white space characters within unicode
	they are not so commonly used and could not all be considered in just an 8 bit character
*/
int			tec_char_is_white_space(char c);

/*tec_string_sha256(char *string, char *hash)
	takes a string and performs a hash SHA256
	char *hash: buffer in which the hash will be stored
			ensure it is at least 32 bytes long
*/
#define tec_string_sha256(X, Y) if(Y){T_SHA256_CTX ctx;t_sha256_init(&ctx);t_sha256_update(&ctx, (unsigned char *) X, tec_string_length(X) );t_sha256_final(&ctx, Y);}

/*tec_file_get_contents:
	this will return the raw contents of a file in a NULL terminated buffer
	returns NULL in case of error (file does not exist, no read permission, etc.)
	free the returned buffer
*/
char*		tec_file_get_contents(char *path);

/*tec_file_get_size:
	returns the size of the file in bytes
	returns -1 in case of error e.g. file does not exist
*/
int64_t		tec_file_get_size(char *path);

void w_process_query_string(char *qs);


/////////////////////////////////////////////////////////////////////////////////////
//Implementation below
/////////////////////////////////////////////////////////////////////////////////////

char** web_get_query_string(){

	char *qs = getenv("QUERY_STRING");
	if(!qs)
		return NULL;
	if(tec_string_length(qs) > WEB_MAX_QUERY_STRING){
		return NULL;
	}

	int i = 0;
	int num = 0;
	char **qss = tec_string_split(qs, '&', &num);

	while(i <= num){
		w_process_query_string(qss[i]);
		i += 1;
	}

	return qss;

}//web_get_query_string*/

char** web_get_post_string(){

	char *cont_len = getenv("CONTENT_LENGTH");
	if(!cont_len){
		return NULL;
	}
	int len = tec_string_to_int(cont_len);
	if(len > WEB_MAX_POST_DATA){
		return NULL;
	}
	len += 1;

	char *buffer = (char *) malloc(sizeof(char) * len );
	fread(buffer, sizeof(char), len, stdin);

	int i = 0;
	int num = 0;
	char **qss = tec_string_split(buffer, '&', &num);
	while(i <= num){
		w_process_query_string(qss[i]);
		i += 1;
	}

	return qss;

}//web_get_post_string*/

char* web_get_from_query_string(char **qss, char *var_name){

	if(!qss)
		return NULL;

	int i = 0;
	while(qss[i]){
		if(tec_buf_begins(qss[i], var_name)){
			char *str = qss[i];
			str += tec_string_length(var_name);
			str += 1;       //skip over '='
			return str;
		}
		i += 1;
	}

	return NULL;

}//web_get_from_query_string*/

int tec_buf_begins(char *buffer, char *str){

	if(!buffer)
		return 0;
	if(!str)
		return 0;

	while(*str && *buffer == *str){
		str += 1;
		buffer += 1;
	}

	if(*str){
		return 0;
	}

	return 1;

}//tec_buf_begins*/

int tec_buf_cmp(char *buf1, char *buf2, int begin, int end){

	if(!buf1 || !buf2)
		return 0;
	if(end < begin)
		return 0;

	if(end - begin < 32){	//32 is a bit arbitrary, must be worth it

		while(begin <= end){
			if(buf1[begin] != buf2[begin]){
				return 0;
			}
			begin += 1;
		}
		return 1;

	}

#if __x86_64__

	int64_t addr = (int64_t) (buf1 + begin);
	int64_t *addr_buf1 = NULL;
	int64_t *addr_buf1_end = (int64_t *) (buf1 + end);
	int64_t *addr_buf2 = NULL;

	// 64 bit computer
	// ensure buffers are on 8-byte boundary before using speed-up trick
	while( addr&0x0000000000000007 && begin <= end){

		if(buf1[begin] != buf2[begin]){
			return 0;
		}
		begin += 1;
		addr += 1;
	}

	addr_buf1 = (int64_t *) (buf1 + begin);
	addr_buf2 = (int64_t *) (buf2 + begin);
	while(addr_buf1 < addr_buf1_end){

		if( *addr_buf1 != *addr_buf2 ){
			return 0;
		}
		addr_buf1 += 1;
		addr_buf2 += 1;

	}

	addr_buf1 -= 1;
	addr_buf2 -= 1;
	char *ch_buf1 = (char *) addr_buf1;
	char *ch_buf2 = (char *) addr_buf2;
	while(ch_buf1 <= &(buf1[end]) ){

		if( *ch_buf1 != *ch_buf2 ){
			return 0;
		}

		ch_buf1 += 1;
		ch_buf2 += 1;

	}

#else

	int32_t addr = (int32_t) (buf1 + begin);
	int32_t *addr_buf1 = NULL;
	int32_t *addr_buf1_end = (int32_t *) (buf1 + end);
	int32_t *addr_buf2 = NULL;

	// 32 bit computer
	// ensure buffers are on 4-byte boundary before using speed-up trick
	while( addr&0x00000003 && begin <= end){

		if(buf1[begin] != buf2[begin]){
			return 0;
		}
		begin += 1;
		addr += 1;
	}

	addr_buf1 = (int32_t *) (buf1 + begin);
	addr_buf2 = (int32_t *) (buf2 + begin);
	while(addr_buf1 < addr_buf1_end){

		if( *addr_buf1 != *addr_buf2 ){
			return 0;
		}
		addr_buf1 += 1;
		addr_buf2 += 1;

	}

	addr_buf1 -= 1;
	addr_buf2 -= 1;
	char *ch_buf1 = (char *) addr_buf1;
	char *ch_buf2 = (char *) addr_buf2;
	while(ch_buf1 <= &(buf1[end]) ){

		if( *ch_buf1 != *ch_buf2 ){
			return 0;
		}

		ch_buf1 += 1;
		ch_buf2 += 1;

	}

#endif

	return 1;

}//tec_buf_cmp*/

int tec_buf_find_str(char *buffer, int buffer_size, char *to_find){

	if(!buffer || !to_find)
		return -1;
	if(buffer_size < 1)
		return -1;

	int len = tec_string_length(to_find);

	if(!len)
		return -1;
	if(len > buffer_size)
		return -1;

	int index = 0;
	int i = 0;
	int j = 0;

	// I don't need to check the full length of string
	// only as far as there is still enough space for it to contain to_find
	int len_loop = buffer_size - len + 1;
	while(index < len_loop){
		while(index < len_loop && buffer[index] != to_find[0]){
			index += 1;
		}
		if(buffer[index] == to_find[0]){
			i = index;
			j = 0;
			while(i < buffer_size && j < len && buffer[i] == to_find[j]){
				i += 1;
				j += 1;
			}
			if(j == len){
				return index;
			}
		}
		index += 1;
	}

	return -1;

}//tec_buf_find_str*/

int tec_string_length(char *str){

	if(!str)
		return 0;
	if(!*str)
		return 0;

	int len = 0;

#if __x86_64__

	int64_t *str_i = (int64_t *) str;
	int64_t addr = (int64_t) str_i;

	// 64 bit computer
	// ensure str is on 8-byte boundary before using speed-up trick
	while( addr&0x0000000000000007 && *str ){
		len += 1;
		str += 1;
		addr = (int64_t) str;
	}

	if(!*str){
		return len;
	}

	// check for NULL characters, 8 bytes at a time
	// https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
	str_i = (int64_t *) str;
	while( ( *str_i - 0x0101010101010101 ) & *str_i & 0x8080808080808080 ){
		len += 8;
		str_i += 1;
	}

	str = (char *) str_i;
	while(*str){
		len += 1;
		str += 1;
	}

#else

	int32_t *str32_i = str;
	int32_t addr32 = (int32_t) str32_i;

	// 32 bit computer
	// ensure str is on 4-byte boundary before using speed-up trick
	while( addr32&0x00000003 && *str ){
		len += 1;
		str += 1;
		addr32 = (int32_t) str;
	}

	if(!*str){
		return len;
	}

	// check for NULL characters, 4 bytes at a time
	// https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
	str32_i = (int32_t *) str;
	while( ( *str32_i - 0x01010101 ) & *str32_i & 0x80808080 ){
		len += 4;
		str32_i += 1;
	}

	str = (char *) str32_i;
	while(*str){
		len += 1;
		str += 1;
	}

#endif

	return len;

}//tec_string_length*/

int tec_string_find_char(char *s, char to_find){

	if(!s)
		return -1;

	int i = 0;
	while(s[i] && to_find != s[i]){
		i += 1;
	}

	if(s[i]){
		return i;
	}else{
		return -1;
	}

}//tec_string_find_char*/

int tec_string_find_str(char *string, char *to_find){

	if(!string || !to_find)
		return -1;

	int len = tec_string_length(string);
	int len2 = tec_string_length(to_find);

	if(!len || !len2)
		return - 1;
	if(len2 > len)
		return -1;

	int index = 0;
	int i = 0;
	int j = 0;

	// I don't need to check the full length of string
	// only as far as there is still enough space for it to contain to_find
	int len_loop = len - len2 + 1;
	while(index < len_loop){
		while(index < len_loop && string[index] != to_find[0]){
			index += 1;
		}
		if(string[index] == to_find[0]){
			i = index;
			j = 0;
			while(i < len && j < len2 && string[i] == to_find[j]){
				i += 1;
				j += 1;
			}
			if(j == len2){
				return index;
			}
		}
		index += 1;
	}

	return -1;

}//tec_string_find_str*/

void tec_string_shift(char *str){

	if(!str)
		return;

	int len = tec_string_length(str);
	int i = 1;
	int j = 1;

	if( (str[i-j] & TEC_MOST_SIG_BIT) && (str[i-j] & TEC_2ND_MOST_SIG_BIT) ){
		j += 1;
		i += 1;
		while( (str[i] & TEC_MOST_SIG_BIT) && !(str[i] & TEC_2ND_MOST_SIG_BIT) ){
			j += 1;
			i += 1;
		}
	}

	while(i < len){
		str[ i - j ] = str[i];
		i += 1;
	}
	str[i - j] = '\0';

}//tec_string_shift*/

char** tec_string_split(char *str, char c, int *num){

	if(!str)
		return NULL;
	if(!c)
		return NULL;

	// we start by assuming that there will be no more than 32 instances of c
	int original_limit = 32;
	int limit = original_limit;
	char **table[26];
	int table_index = 0;
	int tmp_num = 0;
	*num = 0;

	char **res = (char **) malloc(sizeof(char *) * limit);
	if(!res){
		return NULL;
	}
	table[table_index] = res;

	res[0] = str;
	while(*str){
		if(*str == c){
			tmp_num += 1;
			*num += 1;
			if(tmp_num == limit){
				limit *= 2;
				table_index += 1;
				res = (char **) malloc(sizeof(char *) * limit);
				if(!res){
					return NULL;
				}
				table[table_index] = res;
				tmp_num = 0;
			}
			*str = '\0';
			str += 1;
			if(*str){
				res[tmp_num] = str;
			}else{
				//Note(GI) this deals with case where char c is last character in the string
				res[tmp_num] = NULL;
			}
		}else{
			str += 1;
		}
	}

	if(*num < original_limit){
		res[(*num)+1] = NULL;
		return res;
	}

	char **real_res = (char **) malloc(sizeof(char *) * (*num + 2));
	int ti = 0;
	int n = 0;
	int n2 = 0;
	limit = original_limit;
	while(ti <= table_index){

		res = table[ti];
		n2 = 0;
		while(n2 < limit && n <= *num){
			real_res[n] = res[n2];
			n2 += 1;
			n += 1;
		}

		free(res);
		limit *= 2;
		ti += 1;

	}
	real_res[(*num) + 1] = NULL;

	return real_res;

}//tec_string_split*/

int tec_string_to_int(char *s){

	if(!s)
		return 0;

	int sign = 1;
	int result = 0;

	while(tec_char_is_white_space(*s)){
		s += 1;
	}
	if(*s == '-'){
		sign = -1;
		s += 1;
	}else{
		if(*s == '+'){
			s += 1;
		}
	}

	while(*s){
		if(*s > '9' || *s < '0'){
			return result * sign;
		}
		result *= 10;
		result += *s - '0';
		s += 1;
	}

	return result * sign;

}//tec_string_to_int*/

int tec_string_copy(char *destination, char *source, int size){

	if(!destination)
		return 0;
	if(!source){
		*destination = 0;
		return 1;
	}
	if(size <= 0)
		return 0;

	size -= 1;

	int i = 0;
	while(*source && i < size){
		destination[i] = *source;
		source += 1;
		i += 1;
	}

	// we don't want to cut off the copying in the middle of a UTF8 codepoint
	// firstly check whether the next byte of source is either not present or the start of a codepoint
	if(*source && (*source & TEC_MOST_SIG_BIT) && !(*source & TEC_2ND_MOST_SIG_BIT) ){
		i -= 1;
		// this while loop goes back while we have the 2nd, 3rd or 4th byte of a UTF8 codepoint
		while( (destination[i] & TEC_MOST_SIG_BIT) && !(destination[i] & TEC_2ND_MOST_SIG_BIT) ){
			i -= 1;
		}
		// this goes back from the head of a UTF8 codepoint
		if( (destination[i] & TEC_MOST_SIG_BIT) && (destination[i] & TEC_2ND_MOST_SIG_BIT) ){
			destination[i] = 0;
		}else{
			// should never happen, this would be invalid UTF8
			destination[i] = 0;
			return 0;
		}
	}

	destination[i] = '\0';

	if(*source){
		return 2;
	}else{
		return 1;
	}

}//tec_string_copy*/

int tec_string_concatenate(char *destination, char *source, int size){

	if(!destination)
		return 0;
	if(!source)
		return 0;
	if(!*source)
		return 1;
	if(size < 2)
		return 0;

	int len = tec_string_length(destination);
	if(len > size)
		return 0;
	if(len == size && !*source)
		return 1;

	destination += len;
	size -= 1;

	while(*source && len < size){
		*destination = *source;
		len += 1;
		destination += 1;
		source += 1;
	}

	// we don't want to cut off the copying in the middle of a UTF8 codepoint
	// firstly check whether the next byte of source is either not present or the start of a codepoint
	if(*source && (*source & TEC_MOST_SIG_BIT) && !(*source & TEC_2ND_MOST_SIG_BIT) ){
		destination -= 1;
		// this while loop goes back while we have the 2nd, 3rd or 4th byte of a UTF8 codepoint
		while( (*destination & TEC_MOST_SIG_BIT) && !(*destination & TEC_2ND_MOST_SIG_BIT) ){
			destination -= 1;
		}
		// this goes back from the head of a UTF8 codepoint
		if( (*destination & TEC_MOST_SIG_BIT) && (*destination & TEC_2ND_MOST_SIG_BIT) ){
			*destination = 0;
		}else{
			// should never happen, this would be invalid UTF8
			*destination = 0;
			return 0;
		}
	}else{
		*destination = 0;
	}

	if(*source)
		return 2;
	return 1;

}//tec_string_concatenate*/

int tec_char_is_white_space(char c){

	if(c == 32 || c == 9 || c == 10 || c == 11 || c == 12 || c == 13)
		return 1;
	return 0;

}//tec_char_is_white_space*/

char* tec_file_get_contents(char *path){

	if(!path)
		return NULL;

	int64_t size = tec_file_get_size(path);
	if(size == -1)
		return NULL;

	size += 1;
	FILE *fp = fopen(path, "rb");
	if(!fp){
		return NULL;
	}
	char *buffer = (char *) malloc(sizeof(char) * size);
	fread(buffer, sizeof(char), size, fp);
	fclose(fp);
	buffer[size] = 0;
	return buffer;

}//tec_file_get_contents*/

int64_t tec_file_get_size(char *path){

	if(!path)
		return -1;

#ifdef __linux
	struct stat st;
	int error = stat(path, &st);
	if(!error){
		return (int64_t) st.st_size;
	}else{
		return -1;
	}
#endif

#ifdef __WIN32
	BOOL test = FALSE;
	int64_t size;
	HANDLE fp = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	test = GetFileSizeEx(fp, (PLARGE_INTEGER) &size);
	CloseHandle(fp);
	if(test){
		return size;
	}
#endif

	return -1;

}//tec_file_get_size*/

void w_process_query_string(char *qs){

	//first replacing a '+' with a space
	char *tmp = qs;
	int n = tec_string_find_char(tmp, '+');
	while( n != -1 ){
		tmp += n;
		*tmp = ' ';
		n = tec_string_find_char(tmp, '+');
	}

	int i = 0;
	tmp = qs;
	while(qs[i]){
		while(qs[i] && qs[i] != '%'){
			i += 1;
		}
		if(qs[i] == '%'){
			tmp = &(qs[i]);
			int hex = 0;
			i += 1;
			int j = 1;
			while(j >= 0){
				if(qs[i] >= 'A' && qs[i] <= 'F'){
					hex += (qs[i] - 'A') + 10;
				}
				if(qs[i] >= '0' && qs[i] <= '9'){
					hex += (qs[i] - '0');
				}
				if(j){
					hex *= 16;
				}
				j -= 1;
				i += 1;
			}
			tmp[0] = hex;
			tmp += 1;
			tec_string_shift(tmp);
			tec_string_shift(tmp);
			i -= 2;
		}
	}

}//w_process_query_string*/

#endif

