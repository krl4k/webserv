// CLion
// Created by Foster Grisella on 4/10/21 13:03.
//

#include <iostream>

void ft_strcpy(char *dest, const char *src)
{
//	printf("dest  = %p\n", &dest);
	std::cout << "dest = " << &dest << std::endl;

	size_t i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
//	return (dest);
}

int main()
{
	char *dest = (char *)malloc(10);
//	printf("dest  = %p\n", &dest);
	std::cout << "dest = " << &dest << std::endl;

	ft_strcpy(dest, "qwertyuio\0");


//	char srcs[10];
//	ft_strcpy(srcs, dest);
	std::cout << "copy = " << dest << std::endl;
	return (0);
}