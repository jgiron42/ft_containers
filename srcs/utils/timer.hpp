#indef FT_CONTAINERS_TIMER_HPP
#define FT_CONTAINERS_TIMER_HPP

#include <sys/time.h>
#include <iostream>

template <bool automated = true>
class timer {
private:
	struct timeval begin;
	struct timeval _end;
	bool is_stopped;
public:
	std::ostream *output;
	timer() : _end(), is_stopped(false) {
		output = &std::cout;
		gettimeofday(&this->begin, NULL);
	}
	~timer()
	{
		if (automated)
			*output << *this;
	}

	void start() {
		if (!this->is_stopped)
		{
			struct timeval tmp;
			gettimeofday(&tmp, NULL);
			this->begin.tv_sec += ((tmp.tv_sec - this->_end.tv_sec) * 1000000 + (tmp.tv_usec - this->_end.tv_usec)) / 1000000;
			this->begin.tv_usec += tmp.tv_usec > this->_end.tv_usec ? 0 : tmp.tv_usec - this->_end.tv_usec;
		}
		else
			gettimeofday(&this->begin, NULL);
		this->is_stopped = false;
	}

	void stop() {
		gettimeofday(&this->_end, NULL);
		this->is_stopped = true;
	}


	friend std::ostream & operator<<(std::ostream &out, const timer &t) {
		time_t diff;
		struct timeval tmp;

		if (!t.is_stopped) {
			gettimeofday(&tmp, NULL);
//			std::cout << tmp.tv_sec << " " << t.begin.tv_sec << std::endl;
			diff = (tmp.tv_sec - t.begin.tv_sec) * 1000000 + (tmp.tv_usec - t.begin.tv_usec);
		}
		else
			diff = (t._end.tv_sec - t.begin.tv_sec) * 1000000 + (t._end.tv_usec - t.begin.tv_usec);
		out << diff;
		return(out);
	}
};

#endif // FT_CONTAINERS_TIMER_HPP