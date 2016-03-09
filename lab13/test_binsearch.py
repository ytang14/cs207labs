
import numpy as np
import unittest

from binsearch import binary_search

class MyTest(unittest.TestCase):
    
    def test_binsearch(self):
        input = list(range(10))
        self.assertEqual(binary_search(input,5), 5)
    def test_Nan(self):
        input = [np.nan]*5
        with self.assertRaises(TypeError):
            binary_search(input,2)
    def test_numeric(self):
        input=['a']*5
        with self.assertRaises(TypeError):
            binary_search(input,3)
    def test_empty(self):
        self.assertEqual(binary_search([],1),-1)
        
    def test_boundary(self):
        input = list(range(10))
        self.assertEqual(binary_search(input,1,3,1),-1)
    def test_needle_oversize(self):
        input = list(range(10))
        self.assertEqual(binary_search(input,11),-1)
        self.assertEqual(binary_search(input,-1),-1)
    def test_extremes(self):
        input = list(range(10))
        self.assertEqual(binary_search(input,9),9)
        self.assertEqual(binary_search(input,0),0)
    def test_multiple(self):
        input = [3]*10
        self.assertEqual(binary_search(input,3),0)
    def test_overflow(self):
        input = list(range(10))
        self.assertEqual(binary_search(input,np.inf+1),-1)
    def test_sort(self):
        input  = [2,1,3,4,0,23,18]
        with self.assertRaises(AssertionError):
            binary_search(input,2)
        
if __name__ == '__main__':
    unittest.main()
    
    
    