#ifndef HEADERS_H_
#define HEADERS_H_

/* OpenCV Headers*/
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

/* Local directory Headers*/
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
//#define _GNU_SOURCE

/*General C Headers*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>     
#include <assert.h>

#include <netinet/in.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>


#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <fcntl.h>


/*General C++ Headers*/

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <deque>
#include <map>
#include <list>

#include <algorithm>
#include <cmath>
#include <limits>
#include <iomanip>
#include <jsoncpp/json/json.h>
#include <termios.h>

#include <chrono>
#include <thread>
#include <mutex>    

/*General Headers used in this project (backup)*/
/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <map>
#include <string>
*/

#endif

