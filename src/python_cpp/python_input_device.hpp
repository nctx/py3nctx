#ifndef _PYINPT_H_
#define _PYINPT_H_

#include <algorithm> // std::copy
#include <iostream>
#include <boost/iostreams/concepts.hpp>  // boost::iostreams::source
#include <boost/iostreams/stream.hpp>

// pass some stream to c++ - e.g. stdin from python 
// https://stackoverflow.com/questions/24225442/converting-python-io-object-to-stdistream-when-using-boostpython

class PythonInputDevice
  : public boost::iostreams::source // Use convenience class.
{
public:

  explicit
  PythonInputDevice(py::object object)
    : object_(object)
  {}

  std::streamsize read(char_type* buffer, std::streamsize buffer_size) 
  {
    // Read data through the Python object's API.  The following is
    // is equivalent to:
    //   data = object_.read(buffer_size)
    py::object py_data = object_.attr("read")(buffer_size);
    std::string data = py::extract<std::string>(py_data);

    // If the string is empty, then EOF has been reached.
    if (data.empty())
    {
      return -1; // Indicate end-of-sequence, per Source concept.
    }

    // Otherwise, copy data into the buffer.
    copy(data.begin(), data.end(), buffer);
    return data.size();
  }

private:
  py::object object_;
};

#endif
