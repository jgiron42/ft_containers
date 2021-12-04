//
// Created by jgiron on 12/4/21.
//

#include <cstring>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <deque>
#include <errno.h>
#define BUFFER_SIZE 1

bool stop = false;

void handler(int)
{
	stop = true;
}

int main(int argc, char **argv)
{
	char buf1[BUFFER_SIZE + 1];
	char buf2[BUFFER_SIZE + 1];
	int fd1, fd2;
	int ret1 = 1, ret2 = 1;
	std::deque<char> d1, d2;
	int			pos = 0;

	signal(SIGTERM, &handler);
	buf1[BUFFER_SIZE] = 0;
	buf2[BUFFER_SIZE] = 0;
	if (argc == 3)
	{
		if ((fd1 = open(argv[1], O_RDONLY | O_NONBLOCK)) == -1 || (fd2 = open(argv[2], O_RDONLY | O_NONBLOCK)) == -1)
		{
			perror("open");
			return (1);
		}
		while ( !(ret1 <= 0 && ret2 <= 0 && stop))
		{
			ret1 = read(fd1, buf1, BUFFER_SIZE);
			ret2 = read(fd2, buf2, BUFFER_SIZE);
//			std::cout << ret1 << " " << ret2 << std::endl;
			for (int i = 0; i < ret1; i++)
					d1.push_back(buf1[i]);
			for (int i = 0; i < ret2; i++)
					d2.push_back(buf2[i]);
			while (!d2.empty() && !d1.empty()) {
				if (d1.front() == d2.front()) {
					if (d1.front() == 3) {
						std::cout << "no diff detected" << std::endl;
						return (0);
					}
					d1.pop_front();
					d2.pop_front();
					pos++;
				}
				else
				{
					if ((d2.size() >= 100 && d1.size() >= 100)) {
						std::cout << "files differ!!! (pos: " << pos << ")" << std::endl;
						std::cout << "file1:" << std::endl;
						for (int i = 0; i < 100 && i < d1.size(); i++)
							std::cout << d1[i];
						std::cout << std::endl;
						std::cout << "=====================================================" << std::endl;
						std::cout << "file2:" << std::endl;
						for (int i = 0; i < 100 && i < d1.size(); i++)
							std::cout << d2[i];
						std::cout << std::endl;
						return (1);
					}
					break;
				}
			}
		}
		if (ret1 != ret2)
			return (1);
	}
}