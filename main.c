/*
 * tiny program that prints out the type returned by calling
 * PyFunction_GetAnnotations on a PyCallable func.
 *
 * This can be used to show the difference in behaviour between Python3.9 and
 * Python 3.10
 *
 * Note: does a poor job of error handling and memory management in the effort
 * of conciseness. Don't emulate this code!
 *
 * Daniel McCarney
 * @cpu
 * 2021-01-02
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>

int main(int argc, char *argv[]) {
  FILE *script_file = fopen("./test.py", "rt");

  Py_Initialize();
  PyObject *main = PyImport_AddModule("__main__");
  PyObject *globals = PyModule_GetDict(main);

  PyObject *result = PyRun_File(script_file, "test.py", Py_file_input, globals, globals);
  if(!result) {
    fprintf(stderr, "PyRun_File(...) errored unexpectedly\n");
    return 1;
  }

  PyObject *func = PyObject_GetAttrString(main, "foo");
  if(!func || !PyCallable_Check(func)) {
    fprintf(stderr, "PyObject_GetAttrString(main, \"foo\") errored unexpectedly\n");
    return 2;
  }

  PyObject *annotations = PyFunction_GetAnnotations(func);
  if(!annotations) {
    fprintf(stderr, "PyFunction_GetAnnotations(func) errored unexpectedly\n");
    return 3;
  }

  /*
   * With Python <3.10 we expect a PyDict. On 3.10 we get a PyTuple.
   */
  if(PyDict_Check(annotations)) {
    printf("PyFunction_GetAnnotations returned PyDict\n");
  } else if(PyTuple_Check(annotations)) {
    printf("PyFunction_GetAnnotations returned PyTuple\n");
  } else  {
    printf("PyFunction_GetAnnotations returned unknown PyObject\n");
  }
  return 0;
}
