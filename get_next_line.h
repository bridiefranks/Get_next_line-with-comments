/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfranks <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 17:50:01 by bfranks           #+#    #+#             */
/*   Updated: 2022/06/17 20:32:36 by bfranks          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strdup(const char *s1);
char	*get_next_line(int fd);
size_t	ft_strlen(const char *c);
char	*ft_strchr(const char *s, int c);

#endif

/* the 
 * 	   #ifndef GET_NEXT_LINE_H
 * 	   #define GET_NEXT_LINE_H
 * 	   ...
 * 	   #endif
 * stuff stops a header file from being included multiple times (which can be bad in terms of 
 * redefining static variables, recursive header files etc.) During compilation, if it is the 
 * first time the header file has been included, the preprocessor will see that the file hasn't 
 * been defined, so will then define it and do the stuff in between it and the '#endif'. Otherwise,
 *  if it is not the first time it is encountered, the preprocessor will see that it has already 
 *  been defined ie '#ifndef...' is false, and so will skip to #endif.*/  
