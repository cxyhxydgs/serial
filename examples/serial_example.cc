/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <sys/time.h>
// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;
using std::vector;

void my_sleep(unsigned long milliseconds)
{
#ifdef _WIN32
  Sleep(milliseconds); // 100 ms
#else
  usleep(milliseconds * 1000); // 100 ms
#endif
}

void enumerate_ports()
{
  vector<serial::PortInfo> devices_found = serial::list_ports();

  vector<serial::PortInfo>::iterator iter = devices_found.begin();

  while (iter != devices_found.end())
  {
    serial::PortInfo device = *iter++;

    printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
           device.hardware_id.c_str());
  }
}

void print_usage()
{
  cerr << "Usage: test_serial {-e|<serial port address>} ";
  cerr << "<baudrate> [test string]" << endl;
}
int func;

int run(int argc, char **argv)
{
  // if (argc < 2)
  // {
  //   print_usage();
  //   return 0;
  // }
  // Argument 1 is the serial port or enumerate flag
  string port("/dev/ttyAMA1");

  // if (port == "-e")
  // {
  //   enumerate_ports();
  //   return 0;
  // }
  // else if (argc < 3)
  // {
  //   print_usage();
  //   return 1;
  // }

  // Argument 2 is the baudrate
  unsigned long baud = 921600;
// #if defined(WIN32) && !defined(__MINGW32__)
//   sscanf_s(argv[2], "%lu", &baud);
// #else
//   sscanf(argv[2], "%lu", &baud);
// #endif

  // port, baudrate, timeout in milliseconds
  serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

  cout << "Is the serial port open?";
  if (my_serial.isOpen())
    cout << " Yes." << endl;
  else
    cout << " No." << endl;

  // Get the Test string
  // int count = 0;
  // string test_string;
  // if (argc == 4) {
  //   test_string = argv[3];
  // } else {
  //   test_string = "Testing.";
  // }

  // Test the timeout, there should be 1 second between prints
  // Test the timeout at 250ms
  // int timesize;//时间长短
  // int  sum = 0;
  double fps = 0;
  // double fps_t = 0;
  // double fps_t = 0.5;
  struct timeval now_t;
  struct timeval last_t;
  double timeusec;
  unsigned char launch_code[] = {0xab};
  std::vector<uint8_t> buffer(20);
  my_serial.setTimeout(serial::Timeout::max(), 50, 0, 250, 0);
  while (1)
  {
    size_t number;
    number = my_serial.read(buffer.data(), 20);
    for (size_t i = 0; i < number; ++i)
    {
      if (i == buffer[i])
      {
        printf("ea 90");
      }
      else
      {
        printf("0x90");
      }
      // if (i == 2)
      // {
      //   buffer[i] = 0xea;
      //   printf("ea 90\n");
      // }
      // else
      // {
      //   printf("\n ");
      // }
      printf("%x ", buffer[i]);
    }
    // while (1) {
    //   fork();
    //   printf("*****\n");
    // };
    
    if (number != 20)
    {
      my_serial.write(launch_code, 1);
      printf("0XAB\n");
    }
    else
    {
      gettimeofday(&now_t, NULL); //The current time
      // gettimeofday(&last_t, NULL);
      double stamp_now, stamp_last;
      stamp_now = (double)(now_t.tv_sec) + (double)(now_t.tv_usec) / 1000000.0;
      stamp_last = (double)(last_t.tv_sec) + (double)(last_t.tv_usec) / 1000000.0;
      
      timeusec = stamp_now - stamp_last;
      // cout << "time : " << timeusec << endl;
    
      double fps_now = 1.0 / timeusec;
      fps = 0.8 * fps + 0.2 * fps_now; //The current fps
      printf("fps =  %lf", fps);
      last_t = now_t;
    }

    printf("\n ");
  }
  
  return 0;
}

int main(int argc, char **argv)
{
  try
  {
    return run(argc, argv);
  }
  catch (exception &e)
  {
    cerr << "Unhandled Exception: " << e.what() << endl;
  }
}