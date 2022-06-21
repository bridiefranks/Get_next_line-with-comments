/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfranks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 18:41:47 by bfranks           #+#    #+#             */
/*   Updated: 2022/06/17 20:25:37 by bfranks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* The purpose of this function is to, using the read function, read a text file one line at a 
 * time (one line per function call of get_next_line) by using its file descriptor. Depending on 
 * the allocated BUFFER_SIZE, the end of a given line may or may not be reached with one function
 *  call of read: it may need 2 function calls, or 20, or 100, to reach the '\n'. Or, the buffer 
 *  size might be longer than the line, or far longer and encompass many lines, in which case 
 *  the code needs to be able to somehow go back, locate the first '\n', return that which comes 
 *  before it (ie the first line), and REMEMBER where it was up to for subsequent function calls 
 *  and the return of subsequent lines. (So, it may take many calls of 'read' to read a whole 
 *  line, but it's one call of get_next_line per line.)
 * As such, the difficulty isn't when BUFFER_SIZE is shorter than the line- in that case you 
 * could just go along until a '\n' is encountered (sort of). The difficulty is the instances 
 * where the BUFFER_SIZE is longer than the line, and the line needs to be extracted from the 
 * buffer and the position in the file, at the start of the next line, remembered for the next call.
 * The get_next_line is the main 'controlling' function, and so working through the code will 
 * start there, as that is the function that is called. However, the other functions are listed 
 * above it so that they are seen when the computer 'reads' top to bottom; therefore when 
 * get_next_line calls these functions, the computer knows where to find them! (And so they 
 * don't need to be included in the header file, unlike the functions in get_next_line_utils.c)..
 *
 * The general overview of the code is as such: get_next_line receives the file descriptor, which
 *  refers to the particular file (or standard input) we are reading from. The BUFFER_SIZE is 
 *  also given to the function, but this is put in at compilation time not run time. However, it 
 *  can of course be re-compiled with a different value and so we have to remember BUFFER_SIZE may be any value.
 * get_next_line allocates some memory space for 'buffer', according to how big the BUFFER_SIZE 
 * has been set to. 'Buffer' will store the whole read, ie the whole buffer, which may or may 
 * not be longer than the line we are wanting to return (buffer may or may not encompass a '\n').
 * The variable 'line' is then set to equal the return value of find_new_line. 
 * The purpose of this function (find_new_line) is to carry out repeated calls of read (or 
 * perhaps just one) until a '\n' has been found. If this is the first function call of 
 * get_next_line, this is all that happens in find_new_line. However, if it is not the first 
 * call, there is quite likely 'leftover' data remembered from the previous call: ie, for example
 *  if the BUFFER_SIZE were larger than the line, everything that came after the '\n' in the 
 *  buffer was stored in the memory space pointed to by the static char pointer 'backup'. (As it 
 *  is static, it is retained even after each function call has finished). So, in find_new_line, 
 *  whatever was left after the '\n' in the previous function call then gets joined onto the 
 *  buffer of the current function call, and this whole string is returned to get_next_line.
 * Next, get_next_line sends this un-trimmed line off to extract_new_line, to extract out the 
 * current whole line (which may include leftover from the previous call, if the buffer extended 
 * past the '\n'). The 'line' pointer now points to the trimmed line which we will ultimately 
 * return in get_next_line. (As extract_new_line is called within get_next_line, and as the 
 * pointer 'line' is declared in get_next_line not extract_new_line, altering the contents of 
 * what line points to, in extract_new_line, also alters it correspondingly in get_next_line: they
 *  are referring to the same pointer, as declared in get_next_line.)
 * So, extract_new_line trims the line so as to be just one whole line and no left-over, this 
 * line is still pointed to by 'line'. The function however returns 'leftover' which in 
 * get_next_line is set to 'backup' (note that a function can provide more functionality than 
 * just returning a value: what it does within its code may also be relevant e.g. trimming the 
 * string that line points to). This backup contains the remaining characters following this 
 * most recent '\n', to then be included in the NEXT function call's next line. Remembering 
 * 'backup' is static and will therefore retain its value for the next function call. 
 * Lastly, get_next_line returns 'line', which as described points to the current whole line. 
 * If get_next_line is called again, the 'backup' has been remembered and will be included within
 *  the next line returned by get_next_line. (This is assuming there IS backup, ie there are 
 *  leftover characters in the buffer, that came after the '\n' in the last function call. But, 
 *  this isn't necessariily the case- the buffer could have ended exactly on a '\n', in which 
 *  case there's no leftover characters to be remembered and joined on to the next buffer). 
 * Note the function ft_strdup is in this file not the utils file simply because norminette says 
 * the file is too long if put in utils. See my libft_with_comments repo for explanation of code,
 * including for the functions in utils.*/
#include "get_next_line.h"

char	*ft_strdup(const char *s1)
{
	size_t			size;
	char			*ptr;
	unsigned int	i;

	size = ft_strlen(s1);
	ptr = malloc((size + 1) * sizeof(char));
	i = 0;
	if (!ptr)
		return (NULL);
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

static char	*extract_new_line(char *line)
{
	char	*leftover;
	size_t	i;

	i = 0;
	/* the counter is initialised to zero, as is standard...*/
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	/* while neither the end of the line, nor a '\n', has been reached, keep moving the 
	 * counter through the line ((or the string that 'line' is pointing to)).*/
	if (line[i] == '\0' || line[1] == '\0')
		return (NULL);
	/* If the thing that caused the break of the while loop was reaching a '\0' (so if 
	 * line[i] == '\0'), or, if the situation was that line[0] was '\n' and immediately after
	 *  that was null (so if line[1] == '\0'), return null. The first condition of the if 
	 *  statement would be met if we reached the end of the string pointed to by 'line', 
	 *  without having passed a '\n'. This would occur only on the last line of the file. The
	 *   second condition of the if statement would be met if either the file ended in a new 
	 *   line, or if the BUFFER_SIZE happened to land exactly on the '\n' on a function call 
	 *   of read (find_next_line)- a terminating '\0' would then have been appended and hence
	 *    you'd get a '\n' followed by a '\0'. Or, I suppose, the second condition of the if 
	 *    statement could be met if both line[0] and line[1] were '\0', but I'm not sure if 
	 *    that would ever happen...
	 * Anyhow, in both these situations, null is returned. This results in backup being equal
	 *  to null in get_next_line, which has implications for the following function call: if 
	 *  we did in fact reach the end of the file, read would return 0 --> breaks while loop 
	 *  in find_new_line --> backup returned (which is null, as discussed.) Whereas if we 
	 *  weren't at the end of the file yet (if there are more lines after the '\n' still), 
	 *  read would continue to read the file into the buffer, and hence the code would 
	 *  continue.*/ 
	leftover = ft_substr(line, i + 1, ft_strlen(line) - i);
	/* ft_substr is used to extract everything out of line which comes after the '\n'. 
	 * Assuming a '\n' has caused the break of the while loop (otherwise the if statement 
	 * would have been true, and null returned), position i + 1 in the string pointed to by 
	 * line is the first character following the '\n'. To find the end of the string, from 
	 * which to make a substring, we use ft_strlen to find the length (and length - 1 is 
	 * the index number for the last character in the string). Thus, 'leftover' is now 
	 * pointing to the string of those characters in the line which came after '\n'.*/
	if (*leftover == '\0')
	{
		free(leftover);
		leftover = NULL;
		/* if the dereferenced value which leftover is pointing to is '\0', ie all that 
		 * came after the '\n' was a '\0', we want to first of all free leftover (because,
		 *  in ft_substr, the substring is mallocd and therefore needs to be freed) and 
		 *  then set it to equal null. This is because leftover is returned in 
		 *  extract_new_line, and we want null to be returned in this instance, ultimately
		 *   resulting in get_next_line returning null in the following function call 
		 *   (once the current line has been returned first, in this function call, of 
		 *   course). (Also, if something is freed, its pointer no longer points to 
		 *   anything (obviously) but if we want to then do something else with this 
		 *   pointer e.g. return it, we want it to point at something, so as to be able 
		 *   to keep track of it. So we make it null.) So the point is, if the last 
		 *   character of the file is a '\n', this will result in null being returned 
		 *   from extract_new_line and null being returned on the last function call of 
		 *   get_next_line.*/
	}
	line[i + 1] = '\0';
	/* finally, in situations when the last character of the line was not a '\n', which 
	 * means there is leftover, a terminating null is put at i + 1, remembering i is where 
	 * a '\n' was encountered and so we want line to be terminated ('\0') immediately after. 
	 * So, you can stick a '\0' anywhere you want, halfway through a string, and in doing 
	 * that make the string end there. Result being, the line pointed to by 'line' has now 
	 * been trimmed to end at the '\n'. This is the final, complete next line to be returned 
	 * by get_next_line.*/
	return (leftover);
	/* The leftover that came after the '\n' (if any) is then returned to get_next_line, 
	 * to become the new 'backup'.*/
}

static char	*find_new_line(int fd, char *buffer, char *backup)
{
	int		bytes_read;
	char	*temp;

	bytes_read = 1;
	/* set bytes_read to 1, so that the folowing while loop is true (kind of arbitrary)*/
	while (1)
		/*1 is always true, so the while loop continues forever until something causes 
		 * it to break, which is guaranteed to happen*/
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		/* the read function reads up to BUFFER_SIZE bytes from the file with file 
		 * descriptor fd, into the buffer pointed to by 'buffer'. It returns the amount 
		 * of bytes read, or 0 if the end of the file has been reached, or -1 with error.
		 *  Wherever read stops reading (ie when it has reached BUFFER_SIZE, or before 
		 *  then if it has run out of bytes to read), on the next function call of read, 
		 *  it will begin from that spot, not from the start again! That is, it is a 
		 *  feature of the function 'read' that the file offset is incremented by the 
		 *  number of bytes read.*/ 
		if (bytes_read == -1)
			return (NULL);
		/*if there is an error with the read function, it returns -1, and hence, 
		 * bytes_read will be -1. This then causes find_new_line to return null, making 
		 * 'line' in get_next_line equal null. This then fulfills the if statement within
		 *  get_next_line (if (!line)), resulting in the return of line (which is in 
		 *  this case null, so could also be written as return (NULL)). As per subject pdf.*/ 
		else if (bytes_read == 0)
			break ;
		/* whereas, if read returns 0 (and therefore bytes_read is 0) the while loop is 
		 * broken and 'backup' returned. This occurs if the end of the file has been 
		 * reached. In this situation, if the end of the file has been reached, what is 
		 * pointed to by 'backup' is all that is remaining in the file, and will be 
		 * returned, to then later be returned from get_next_line.*/ 
		buffer[bytes_read] = '\0';
		/*but if neither of these if statements are true (if no error or not yet at the 
		 * end of the file), a terminating null will be put on the end of the string 
		 * pointed to by buffer*/
		if (!backup)
			backup = ft_strdup("");
		/* if it is the first function call, or if the previous call ended exactly on a 
		 * '\n' with no leftover buffer remaining, backup will be null (remember it 
		 * starts as null as static variables are automatically initialised as 0). In 
		 * this situation, we want to set backup to actually point at something, rather 
		 * than being equal to null and not pointing to anything. This is because, in 2 
		 * lines' time, ft_strjoin will want to join what it is pointing to (via temp..) 
		 * with what buffer is pointing to, and will seg fault if backup isn't pointing 
		 * to anything. So, we use ft_strdup to make it point to an empty string (which 
		 * is still something.)*/ 
		temp = backup;
		/* we make the pointer temp point to the same place that backup is pointing to, 
		 * because in the next line, backup will be set to point at a different location 
		 * (that which is malloced in ft_strjoin) and so we keep track of the original 
		 * memory location it was pointing to, using temp, so that we can later free it. 
		 * (We can't free it yet obviously as it will be used in ft_strjoin).*/
		backup = ft_strjoin(temp, buffer);
		/*next we join together what is pointed to by temp (ie what backup is currently 
		 * pointing to; the leftover from the previous function call) and the buffer from
		 *  this call, into one string using ft_strjoin. The pointer 'backup' is then 
		 *  set to point at the memory location of this string.*/
		free(temp);
		/* the memory area that temp is pointing to (ie the old 'backup') is then freed, 
		 * as the old backup pointed to a memory location that was been malloced, ie is 
		 * on the heap not the stack. And hence needs to be freed.*/
		if (ft_strchr(buffer, '\n'))
			break ;
		/* If a '\n' character is found within the buffer (found using ft_strchr), the 
		 * while loop breaks (note ft_strchr finds the FIRST instance of '\n', and so we 
		 * are only going to find the FIRST '\n' we come across). If not, as the while 
		 * loop is always true, it will repeat again from the top: read will be called 
		 * again, a new buffer created and joined to the backup (which now includes the 
		 * buffer from the LAST call of read) to become the new backup. In this way, 
		 * read will be called as many times as needed until a '\n' is found in the buffer
		 *  and the while loop breaks.*/ 
	}
	return (backup);
	/* And backup is returned to get_next_line, where 'line' is set to point to it (reminder 
	 * backup currently points to all the leftover from last call of get_nextline, plus the 
	 * buffer of this call, which contains as many buffers from repeated calls of read, as 
	 * required to reach (or go past, potentially) a '\n'.)*/
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup;

	if ((fd < 0) || (BUFFER_SIZE <= 0))
		return (NULL);
	/* If there is no file descriptor, or if the BUFFER_SIZE for some reason has been set 
	 * to <= 0 (ie if there are weird errors) return null as per project page)*/
	buffer = malloc((BUFFER_SIZE + 1) * (sizeof(char)));
	/* allocate enough memory to hold the BUFFER_SIZE plus one for the terminating '\0', 
	 * pointed to by 'buffer'*/ 
	if (!buffer)
		return (NULL);
	/*if the memory allocation was unsuccessful, return null (ie was an error)*/
	line = find_new_line(fd, buffer, backup);
	/* set the pointer 'line' to point to the returned value of find_new_line, which is 
	 * passed the file descriptor, the pointer 'buffer' which is pointing to empty allocated 
	 * space, and static pointer 'backup' which for the FIRST function call has a value of 
	 * null. This is because static variables are automatically initialised as zero. Note 
	 * that 'backup' isn't POINTING to a null character, but rather, the pointer itself has a
	 *  value of NULL (currently). Backup will also equal null if the previoius function call
	 *   had its read land exactly on the end of a line, ie there was no leftover after the 
	 *   '\n'. However, if this function call is not the first, and there was leftover from 
	 *   last function call, backup will then be pointing to this leftover. (now scroll up 
	 *   and look at find_new_line....)
	 * So, 'line' now points to any leftover from the previous call (that is in fact part of 
	 * the current line), joined onto the rest of the line of the current call, plus any 
	 * leftover from THIS call (if BUFFER_SIZE was larger than the line length, or if the 
	 * last call of read caused the buffer to 'overhang' the end of the line) to be then 
	 * transferred to the next line in the next call.*/ 
	free(buffer);
	/* just good practise to free that which has been malloced, to prevent memory leakage*/
	if (!line)
		return (line);
	/* If line is false, ie has the value of null, return line, which in this case is null.. 
	 * This would be the case if in find_new_line, the first if statement was true --> returns
	 *  null, and thus 'line' in get_next_line would have the value null. The if statement 
	 *  would be true if bytes_read == -1, ie, if there was an error. So having this section 
	 *  of code in get_next_line is just a way of channelling the result of an error in the 
	 *  secondary function through to the main calling function (as just returning null in 
	 *  find_new_line isn't enough; it needs to cause get_next_line to return null as well.*/
	backup = extract_new_line(line);
	/* Now, extract_new_line is called, and given the pointer 'line', so it can extract from 
	 * it the next line, while also returning the leftover buffer characters to be retained 
	 * for next time. This leftover is saved in the static 'backup' (or really, saved in the 
	 * memory pointed to by backup). Scroll up to look at extract_new_line...
	 * So, backup now points to the leftover characters of the buffer, to be recalled in the 
	 * next function call. Or, if there is no leftover, backup once again has the value of 
	 * null (as per extract_next_line). Also in extract_next_line, the string pointed to by 
	 * line has now been trimmed to only contain the current line and nothing else. This is 
	 * what we are wanting to return.*/
	return (line);
	/* And finally, the next line is returned.*/
}
