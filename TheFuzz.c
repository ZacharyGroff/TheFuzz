#include <Python.h>

static
PyObject *JaroSimilarity(PyObject *self, PyObject *args);

static
PyObject *JaccardIndex(PyObject *self, PyObject *args);

int
max(int sourceLen, int targetLen)
{
  if (sourceLen >= targetLen)
    return sourceLen;
  else
    return targetLen;
}

int
min(int sourceLen, int targetLen)
{
  if (sourceLen <= targetLen)
    return sourceLen;
  else
    return targetLen;
}

double
CJaroSimilarity(char source[], char target[])
{		
  if (source == NULL || target == NULL)
		return 0.0;
    
  if (source == target)
    return 1.0;
        
  int sourceLen = strlen(source);
  int targetLen = strlen(target);
    
  if (sourceLen == 0 && targetLen != 0)
    return 0.0;
        
  if (targetLen == 0 && sourceLen != 0)
    return 0.0;
        
  double matchDistance = (max(sourceLen, targetLen) / 2) - 1;
  double matches = 0.0;
  double transpositions = 0.0;

  int sourceMatches[sourceLen];
  int targetMatches[targetLen];

  memset(sourceMatches, 0, sizeof sourceMatches);
  memset(targetMatches, 0, sizeof targetMatches);
    
  for (int i = 0; i < sourceLen; i++)
  {
    int matchDistanceStart = max(0, i - matchDistance);
    int matchDistanceEnd = min(i + matchDistance + 1, targetLen);
        
    for (int j = matchDistanceStart; j < matchDistanceEnd; j++)
    {
      if(targetMatches[j])
        continue;
      if(source[i] != target[j])
        continue;
                
      sourceMatches[i] = 1;
      targetMatches[j] = 1;
      matches++;
            
      break;
    }
  }
    
  if (matches <= 0)
    return 0.0;
    
  int i = 0;
  for (int j = 0; j < sourceLen; j++)
  {
    if (!sourceMatches[j])
      continue;
        
    while (!targetMatches[i])
      i++;
            
    if (source[j] != target[i])
      transpositions++;
            
    i++;
  }

  double sourceLenDouble = (double) sourceLen;
  double targetLenDouble = (double) targetLen;

  return ((matches / sourceLenDouble) + (matches / targetLenDouble) + ((matches - (transpositions / 2)) / matches)) / 3;
}

double
CJaccardIndex(char source[], char target[])
{
    int sourceLen = strlen(source);
    int targetLen = strlen(target);
    
    char * longer = (sourceLen >= targetLen) ? source : target;
    char * shorter = (sourceLen >= targetLen) ? target : source;
    
    int longerLen = strlen(longer);
    int shorterLen = strlen(shorter);
    
    unsigned int bitmap[127];
    memset(bitmap, 0, sizeof bitmap);
    
    unsigned int interSectBitMap[127];
    memset(interSectBitMap, 0, sizeof interSectBitMap);
    
    int intersections = 0;
    int unions = 0;
    
    for (int i = 0; i < longerLen; i++)
        interSectBitMap[(int) longer[i]] = 1;
    
    for (int i = 0; i < shorterLen; i++)
    {
       intersections += (interSectBitMap[(int) shorter[i]] & 1);
       interSectBitMap[(int) shorter[i]] = 0;
    }
    
    for (int i = 0; i < longerLen; i++)
    {
        unions += (bitmap[(int) longer[i]] ^ 1);
        bitmap[(int) longer[i]] = 1;
    }
    
    for (int i = 0; i < shorterLen; i++)
    {
        unions += (bitmap[(int) shorter[i]] ^ 1);
        bitmap[(int) shorter[i]] = 1;
    }
		return (double) intersections / unions;
}

static PyObject*
JaccardIndex(PyObject *self, PyObject *args)
{
	char *source;
	char *target;
		
	if (!PyArg_ParseTuple(args, "ss", &source, &target))
		return NULL;

	return Py_BuildValue("d", CJaccardIndex(source, target));
}

static PyObject*
JaroSimilarity(PyObject *self, PyObject *args)
{
	char *source;
	char *target;
		
	if (!PyArg_ParseTuple(args, "ss", &source, &target))
		return NULL;

	return Py_BuildValue("d", CJaroSimilarity(source, target));
}

static PyObject*
Version(PyObject* self)
{
	return Py_BuildValue("s", "Version 1.0");
}

static PyMethodDef
Functions[] = {
	{"JaroSimilarity", JaroSimilarity, METH_VARARGS, "Calculates Jaro Similarity between two strings"},
	{"JaccardIndex", JaccardIndex, METH_VARARGS, "Calculates set intersection over set union for two strings"},
	{"Version", (PyCFunction)Version, METH_NOARGS, "Returns version number"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef
TheFuzz = {
	PyModuleDef_HEAD_INIT,
	"TheFuzz",
	"TheFuzz Module",
	-1,
	Functions
};

PyMODINIT_FUNC PyInit_TheFuzz(void)
{
	return PyModule_Create(&TheFuzz);
}
